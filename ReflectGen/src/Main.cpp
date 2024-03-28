#include "pch.h"
#include "TypeInfoHeader.h"

#include <filesystem>

namespace fs = std::filesystem;

#if !defined(NDEBUG)
#define DEBUG_PRINT_TYPES 1
#else
#define DEBUG_PRINT_TYPES 0
#endif

const std::string Token_Empty = "";

const std::string ReflectGen_SkipFileToken = "ReflectGen_DontParse";
const std::string ReflectGen_Directory = "\\gen\\";
const std::string ReflectGen_CppFileName = "Generated0.cpp";
const std::string ReflectGen_HeaderName = "ReflectTypeInfo.h";

struct TypeDesc
{
	std::string defType; /* struct, class */
	std::string name; /* typename */
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
	void createTypeInfoHeader(const std::string& fileName) noexcept;
	void generateTypeFile(const std::string& fileName) noexcept;

private:
	std::list<TypeDesc> mFoundTypes;
};

void ReflectGen::generate(const std::string& projectRootDir) noexcept
{
	/* Validate project root directory */
	if (!fs::exists(projectRootDir))
	{
		printf("Could not find the specified directory\n");
		return;
	}

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
		printf("\t%s %s\n", foundType.defType.c_str(), foundType.name.c_str());
	}
	printf("End Type List\n");
#endif

	std::string outputDirectory = projectRootDir + ReflectGen_Directory;

	fs::create_directory(outputDirectory);

	createTypeInfoHeader(outputDirectory);
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

	/* Iterate tokens to find user-defined types. */
	for (int64_t i = 0; i < static_cast<int64_t>(tokens.size()); i++)
	{
		const auto& curToken = getToken(i);

		/* User-defined class */
		if (curToken == "class")
		{
			const auto& className = getToken(i + 1);
			const auto& prevToken = getToken(i - 1);

			/* friend class of a type */
			if (prevToken == "friend")
				continue;

			TypeDesc desc;
			desc.name = className;
			desc.defType = "class";

			mFoundTypes.push_back(desc);
		}
		/* User-defined struct */
		else if (curToken == "struct")
		{
			const auto& structName = getToken(i + 1);
			const auto& prevToken = getToken(i - 1);

			/* friend struct of a type */
			if (prevToken == "friend")
				continue;

			TypeDesc desc;
			desc.name = structName;
			desc.defType = "struct";

			mFoundTypes.push_back(desc);
		}
	}
}

/* Split the file into tokens for parsing */
void ReflectGen::tokenizeFile(const std::string& contents, std::vector<std::string>& tokens) noexcept
{
	std::string buffer = "";

	for (auto itr = contents.begin(); itr < contents.end(); ++itr)
	{
		if (std::isalnum(*itr))
		{
			buffer.push_back(*itr);
		}
		else if (!buffer.empty())
		{
			tokens.push_back(buffer);
			buffer.clear();
		}
	}
}

/**
 * Create type info header
 * which the project will include for accessing type info.
 */
void ReflectGen::createTypeInfoHeader(const std::string& fileName) noexcept
{
	std::ofstream fileStream(fileName + ReflectGen_HeaderName);
	fileStream.write(ReflectGen_Types_Header.c_str(), ReflectGen_Types_Header.size());
	fileStream.close();
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

	for (const auto& foundType : mFoundTypes)
	{
		fileStream << foundType.defType << " " << foundType.name << ";\n";
	}

	fileStream << "\n";

	/* Declare type info in the ReflectGen namespace */
	fileStream << "namespace reflectgen {\n\n";

	/* Declare type info for all types */
	fileStream << "/* Declare type info */\n";

	for (const auto& foundType : mFoundTypes)
	{
		std::string typeName = foundType.name;

		/* const TypeInfo* detail::DTI<Type>::mTypeInfoPtr = new detail::TypeInfoImpl<Type>("name-of-type"); */
		std::string outputLine = std::format(
			"const TypeInfo* detail::DTI<{}>::mTypeInfoPtr = new detail::TypeInfoImpl<{}>(\"{}\");",
			typeName, typeName, typeName
		);

		fileStream << outputLine << "\n";
	}

	/* End of  */
	fileStream << "\n} // reflectgen\n";

	fileStream.close();
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