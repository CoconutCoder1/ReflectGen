#include "pch.h"
#include "ReflectTypeInfo.h"

namespace fs = std::filesystem;

#if !defined(NDEBUG)
#define DEBUG_PRINT_TYPES 0
#else
#define DEBUG_PRINT_TYPES 0
#endif

constexpr size_t NamespaceIndex_None = 0;

const std::string Token_Empty = "";

const std::string ReflectGen_SkipFileToken = "ReflectGen_DontParse";
const std::string ReflectGen_Directory = "\\gen\\";
const std::string ReflectGen_CppFileName = "Generated0.cpp";
const std::string ReflectGen_HeaderName = "ReflectTypeInfo.h";

struct TypeDesc
{
	std::string defType; /* struct, class */
	std::string name; /* typename */
	struct NamespaceDesc* parentNameSpace;
};

struct NamespaceDesc
{
	NamespaceDesc(const std::string& alias, NamespaceDesc* parentNamespacePtr)
		: alias(alias), mParentNamespace(parentNamespacePtr)
	{
		// mChildNamespaces.reserve(10);
	}

	~NamespaceDesc()
	{
		for (auto& childNamespacePtr : mChildNamespaces)
		{
			delete childNamespacePtr;
			childNamespacePtr = nullptr;
		}
		mChildNamespaces.clear();
	}

	std::string alias;
	std::vector<NamespaceDesc*> mChildNamespaces;
	std::vector<TypeDesc> mChildTypes;
	NamespaceDesc* mParentNamespace;

	bool operator==(const std::string& rhs) noexcept
	{
		return alias == rhs;
	}

	NamespaceDesc* findChild(const std::string& alias) noexcept
	{
		auto itr = std::find_if(mChildNamespaces.begin(), mChildNamespaces.end(), [alias](const NamespaceDesc* descPtr) {
			return alias == descPtr->alias;
		});

		return itr != mChildNamespaces.end() ? *itr : nullptr;
	}

	NamespaceDesc* findOrAddChild(const std::string& alias, NamespaceDesc* parent) noexcept
	{
		NamespaceDesc* result = findChild(alias);

		if (!result)
			result = mChildNamespaces.emplace_back(new NamespaceDesc(alias, parent));

		return result;
	}
};

struct TokenizedFile
{
	std::vector<std::string> tokens;
};

class ReflectGen
{
public:
	void generate(const std::string& projectRootDir) noexcept;

private:
	void parseFile(const fs::path& filePath) noexcept;
	bool isValidFile(const fs::path& filePath) noexcept;
	void findTypes(const std::string& contents) noexcept;
	void tokenizeFile(const std::string& contents, std::vector<std::string>& tokens) noexcept;
	void generateTypeFile(const std::string& fileName) noexcept;

	void forwardDeclareTypes(std::ofstream& fileStream, NamespaceDesc const* const namespaceDescPtr) noexcept;
	void declareTypeInfo(std::ofstream& fileStream, NamespaceDesc const* const namespaceDescPtr) noexcept;

	size_t getNameSpaceIndex(const std::string& name) noexcept;
	size_t getNameSpaceIndex(const std::list<std::string>& nameSpaceList) noexcept;

	void createGlobalNamespace() noexcept;

private:
	// std::vector<TypeDesc> mFoundTypes;

	/* Index 0 is reserved for no namespace */
	std::unordered_map<std::string, size_t> mNameSpaceToIndex;
	size_t mNameSpaceCount = 1;

	NamespaceDesc* mGlobalNamespace;
};

void ReflectGen::generate(const std::string& projectRootDir) noexcept
{
	/* Validate project root directory */
	if (!fs::exists(projectRootDir))
	{
		printf("Could not find the specified directory\n");
		return;
	}

	createGlobalNamespace();

	/* Iterate project root directory recursively and parse files */
	for (const auto& dir : fs::recursive_directory_iterator(projectRootDir))
	{
		if (!dir.is_directory())
		{
			parseFile(dir.path());
		}
	}

#if DEBUG_PRINT_TYPES
	printf("Number of Types: %llu\n", mFoundTypes.size());
	printf("Begin Type List:\n");
	for (const auto& foundType : mFoundTypes)
	{
		printf("\t%s %s", foundType.defType.c_str(), foundType.name.c_str());
		
		/*if (!foundType.parentNameSpace.empty())
			printf(" | namespace: %s", foundType.parentNameSpace.c_str());*/

		printf("\n");
	}
	printf("End Type List\n");
#endif

	std::string outputDirectory = projectRootDir + ReflectGen_Directory;

	fs::create_directory(outputDirectory);

	generateTypeFile(outputDirectory);
}

void ReflectGen::parseFile(const fs::path& filePath) noexcept
{
	/* Only parse files we know are source and header files */
	if (!isValidFile(filePath))
		return;

	std::ifstream fileStream(filePath.string(), std::ios::ate);

	size_t fileSize = fileStream.tellg();
	fileStream.seekg(0);

	std::string contents;
	contents.resize(fileSize);

	fileStream.read(contents.data(), fileSize);
	fileStream.close();

	/* Check if this file is marked for skipped parsing */
	if (contents.find(ReflectGen_SkipFileToken) != std::string::npos)
		return;

	findTypes(contents);
}

bool ReflectGen::isValidFile(const fs::path& filePath) noexcept
{
	if (filePath.filename() == ReflectGen_CppFileName)
		return false;

	if (filePath.has_extension())
	{
		std::string extension = filePath.extension().string();
		
		/* Source file */
		if (extension == ".cpp" || extension == ".cc")
			return true;

		/* Header file */
		if (extension == ".h" || extension == ".hpp" || extension == ".hh")
			return true;
	}

	return false;
}

void ReflectGen::findTypes(const std::string& contents) noexcept
{
	std::vector<std::string> tokens;

	tokenizeFile(contents, tokens);

	/**
	 * Utility lambda which sanitizes the index passed
	 * before indexing the token list with it.
	 */
	static const auto getToken = [&tokens](int64_t i) -> const std::string& {
		if (i < 0 || i >= static_cast<int64_t>(tokens.size()))
			return Token_Empty;

		return tokens[i];
	};

	enum ScopeType
	{
		ScopeType_Unknown,
		ScopeType_Namespace,
		ScopeType_Multiple,
		ScopeType_Class,
		ScopeType_Struct,
		ScopeType_Count,
	};

	struct MultipleNamespace
	{
		size_t numNamespaces;
	};

	struct Scope
	{
		Scope(const std::string& name, ScopeType type, void* scopeDataPtr)
			: name(name), type(type), scopeDataPtr(scopeDataPtr)
		{
		}

		std::string name;
		ScopeType type;
		void* scopeDataPtr;
	};

	/* Scopes stack */
	std::stack<Scope> genericScopeStack;
	std::vector<Scope> scopeStack[ScopeType_Count];
	bool ignoreNextScope = false;

	static const auto pushScope = [&genericScopeStack, &scopeStack, &ignoreNextScope](const std::string& name, ScopeType type, void* dataPtr = nullptr) -> void {
		if (type != ScopeType_Unknown)
			ignoreNextScope = true;

		genericScopeStack.emplace(name, type, dataPtr);
		scopeStack[type].emplace_back(name, type, dataPtr);
	};

	static const auto popScope = [&genericScopeStack, &scopeStack]() -> void {
		ScopeType lastType = genericScopeStack.top().type;

		scopeStack[lastType].pop_back();
		genericScopeStack.pop();
	};

	size_t lastScopeStackSize = 0;

	/* Iterate tokens to find user-defined types. */
	for (int64_t i = 0; i < static_cast<int64_t>(tokens.size()); i++)
	{
		const auto& curToken = getToken(i);

		const auto& nameSpaceStack = scopeStack[ScopeType_Namespace];

		/* User-defined class */
		if (curToken == "class")
		{
			const auto& className = getToken(i + 1);
			const auto& prevToken = getToken(i - 1);

			/* friend class of a type */
			if (prevToken == "friend")
				continue;
			
			/* class Object; */
			const auto& isForwardDecl = getToken(i + 2) == ";";

			TypeDesc desc;
			desc.name = className;
			desc.defType = "class";
			
			if (!nameSpaceStack.empty())
			{
				NamespaceDesc* parentNameSpace = mGlobalNamespace;

				for (auto itr = nameSpaceStack.begin(); itr < nameSpaceStack.end(); ++itr)
				{
					parentNameSpace = parentNameSpace->findOrAddChild(itr->name, parentNameSpace);
				}

				desc.parentNameSpace = parentNameSpace;
			}
			else
				desc.parentNameSpace = mGlobalNamespace;

			desc.parentNameSpace->mChildTypes.push_back(desc);

			/* Only push scope if this is not a forward declaration */
			if (!isForwardDecl)
				pushScope(className, ScopeType_Class);
		}
		/* User-defined struct */
		else if (curToken == "struct")
		{
			const auto& structName = getToken(i + 1);
			const auto& prevToken = getToken(i - 1);

			/* friend struct of a type */
			if (prevToken == "friend")
				continue;

			/* struct Object; */
			const auto& isForwardDecl = getToken(i + 2) != "{";

			TypeDesc desc;
			desc.name = structName;
			desc.defType = "struct";

			if (!nameSpaceStack.empty())
			{
				NamespaceDesc* parentNameSpace = mGlobalNamespace;

				for (auto itr = nameSpaceStack.begin(); itr < nameSpaceStack.end(); ++itr)
				{
					parentNameSpace = parentNameSpace->findOrAddChild(itr->name, parentNameSpace);
				}

				desc.parentNameSpace = parentNameSpace;
			}
			else
				desc.parentNameSpace = mGlobalNamespace;

			desc.parentNameSpace->mChildTypes.push_back(desc);

			/* Only push scope if this is not a forward declaration */
			if (!isForwardDecl)
				pushScope(structName, ScopeType_Struct);
		}
		/* Namespace */
		else if (curToken == "namespace")
		{
			const auto& nextToken = getToken(i + 1);
			const auto& prevToken = getToken(i - 1);

			/* using namespace ??; */
			if (prevToken == "using")
				continue;

			/* namespace ?? = ??; */
			if (nextToken == "=")
				continue;

			/* Anonymous namespace */
			if (nextToken == "{")
				continue;

			size_t numNamespaces = 1;
			size_t baseIndex = i + 1;

			pushScope(nextToken, ScopeType_Namespace);

			/* namespace my::name::space */
			while (getToken(baseIndex + 1) == ":" && getToken(baseIndex + 2) == ":")
			{
				baseIndex += 3;
				pushScope(getToken(baseIndex), ScopeType_Namespace);
				numNamespaces++;
			}

			if (numNamespaces > 1)
				pushScope("MultipleNamespace", ScopeType_Multiple, reinterpret_cast<void*>(numNamespaces));
		}
		/* Start of scope */
		else if (curToken == "{")
		{
			if (!ignoreNextScope)
			{
				pushScope("UnknownScope", ScopeType_Unknown);
			}
			ignoreNextScope = false;
		}
		/* End of scope */
		else if (curToken == "}")
		{
			/* Was instructed to pop more than once */
			if (genericScopeStack.top().type == ScopeType_Multiple)
			{
				size_t numScopes = reinterpret_cast<size_t>(genericScopeStack.top().scopeDataPtr);

				for (size_t i = 0; i < std::max(1ULL, numScopes); ++i)
					popScope();
			}

			popScope();
		}

		lastScopeStackSize = genericScopeStack.size();
	}
}

/* Split the file into tokens for parsing */
void ReflectGen::tokenizeFile(const std::string& contents, std::vector<std::string>& tokens) noexcept
{
	std::string buffer = "";

	bool inComment = false;

	for (auto itr = contents.begin(); itr < contents.end(); ++itr)
	{
		if (inComment)
		{
			auto nextItr = itr + 1;

			if (nextItr != contents.end())
			{
				if (*itr == '*' && *nextItr == '/')
				{
					inComment = false;
					itr++;
					continue;
				}
				else if (*itr == '\n')
				{
					inComment = false;
				}
			}

			continue;
		}

		if (*itr == '/')
		{
			auto nextItr = itr + 1;

			if (nextItr != contents.end() && (*nextItr == '/' || *nextItr == '*'))
			{
				inComment = true;
				itr++;
				continue;
			}
		}

		if (std::isalnum(*itr))
		{
			buffer.push_back(*itr);
		}
		else
		{
			if (!buffer.empty())
			{
				tokens.push_back(buffer);
				buffer.clear();
			}

			if (!std::isspace(*itr) && !std::isblank(*itr))
			{
				char buf[2] = { *itr, '\0' };

				tokens.emplace_back(buf);
			}
		}
	}

	if (!buffer.empty())
	{
		tokens.push_back(buffer);
		buffer.clear();
	}
}

/**
 * Generate ReflectGen source file.
 * This file declares the type info for all found types in the project.
 */
void ReflectGen::generateTypeFile(const std::string& fileName) noexcept
{
	std::ofstream fileStream(fileName + ReflectGen_CppFileName);

	/* Do not parse this file in the future */
	fileStream << "/* " << ReflectGen_SkipFileToken << " */\n\n";

	/* Include ReflectGen header */
	fileStream << "#include \"" << ReflectGen_HeaderName << "\"\n\n";

	/* Forward declare all types */
	fileStream << "/* Forward declared types */\n";
	forwardDeclareTypes(fileStream, mGlobalNamespace);

	fileStream << "\n";

	/* Declare type info in the ReflectGen namespace */
	fileStream << "namespace reflectgen {\n\n";

	/* Declare type info for all types */
	fileStream << "/* Declare type info */\n";
	declareTypeInfo(fileStream, mGlobalNamespace);

	/* End of namespace */
	fileStream << "\n} // reflectgen\n";

	fileStream.close();
}

/* Recursively forward declare all types within their namespaces */
void ReflectGen::forwardDeclareTypes(std::ofstream& fileStream, NamespaceDesc const* const namespaceDescPtr) noexcept
{
	bool isInsideNamespace = namespaceDescPtr != mGlobalNamespace;

	if (isInsideNamespace)
		fileStream << "namespace " << namespaceDescPtr->alias << "{\n";

	for (const auto& type : namespaceDescPtr->mChildTypes)
		fileStream << type.defType << " " << type.name << ";\n";

	for (const auto& childNamespace : namespaceDescPtr->mChildNamespaces)
		forwardDeclareTypes(fileStream, childNamespace);

	if (isInsideNamespace)
		fileStream << "} // " << namespaceDescPtr->alias << "\n";
}

/* Recursively declare type info for all types in their namespaces */
void ReflectGen::declareTypeInfo(std::ofstream& fileStream, NamespaceDesc const* const namespaceDescPtr) noexcept
{
	std::string identifierPrefix = "";
	std::string symbolPrefix = "";

	NamespaceDesc const* parentPtr = namespaceDescPtr;
	while (parentPtr != mGlobalNamespace)
	{
		identifierPrefix.insert(0, parentPtr->alias + ".");
		symbolPrefix.insert(0, parentPtr->alias + "::");
		parentPtr = parentPtr->mParentNamespace;
	}

	for (const auto& type : namespaceDescPtr->mChildTypes)
	{
		std::string identifierName = identifierPrefix + type.name;
		std::string symbolName = symbolPrefix + type.name;

		std::string outputLine = std::format(
			"const TypeInfo* detail::DTI<{}>::mTypeInfoPtr = new detail::TypeInfoImpl<{}>(\"{}\");",
			symbolName, symbolName, identifierName
		);

		fileStream << outputLine << "\n";
	}

	for (const auto& childNamespace : namespaceDescPtr->mChildNamespaces)
		declareTypeInfo(fileStream, childNamespace);
}

/* Create new namespace index in the registry if it does not exist already */
size_t ReflectGen::getNameSpaceIndex(const std::string& name) noexcept
{
	if (!mNameSpaceToIndex.contains(name))
	{
		size_t newIndex = mNameSpaceCount++;
		mNameSpaceToIndex[name] = newIndex;

		return newIndex;
	}
	return mNameSpaceToIndex[name];
}

/* Appends all namespaces to one string, separated by "::" then gets the index */
size_t ReflectGen::getNameSpaceIndex(const std::list<std::string>& nameSpaceList) noexcept
{
	if (nameSpaceList.empty())
		return 0;

	size_t i = 0;
	std::string fullNameSpace = "";

	for (const auto& nameSpace : nameSpaceList)
	{
		fullNameSpace += nameSpace;

		/* Do not append "::" at the end of the last namespace */
		if (nameSpaceList.size() - 1 != i++)
			fullNameSpace += "::";
	}

	return getNameSpaceIndex(fullNameSpace);
}

void ReflectGen::createGlobalNamespace() noexcept
{
	if (mGlobalNamespace)
	{
		delete mGlobalNamespace;
		mGlobalNamespace = nullptr;
	}

	mGlobalNamespace = new NamespaceDesc("GlobalNamespace", nullptr);
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Project root directory must be provided in the command line arguments\n");
		return 1;
	}

	ReflectGen gen;
	gen.generate(argv[1]);

	return 0;
}