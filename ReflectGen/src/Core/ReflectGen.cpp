#include "pch.h"
#include "ReflectGen.h"

#include "TokenContext.h"
#include "TokenBehavior.h"

#include "Descriptor/Type.h"
#include "Descriptor/Namespace.h"

#include "Scope/Scope.h"

#include "FNV1a.h"

const std::string Token_Empty = "";

const std::string ReflectGen_SkipFileToken = "ReflectGen_DontParse";
const std::string ReflectGen_Directory = "\\gen\\";
const std::string ReflectGen_CppFileName = "Generated0.cpp";
const std::string ReflectGen_HeaderName = "ReflectTypeInfo.h";

void ReflectGen::generate(const std::string& projectRootDir) noexcept
{
	/* Validate project root directory */
	if (!fs::exists(projectRootDir))
	{
		printf("Could not find the specified directory\n");
		return;
	}

	mContext.scopeManager = &mScopeManager;
	mContext.descriptorContext = &mDescriptorContext;

	/* Iterate project root directory recursively and parse files */
	for (const auto& dir : fs::recursive_directory_iterator(projectRootDir))
	{
		if (!dir.is_directory())
		{
			parseFile(dir.path());
		}
	}

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
	tokenizeFile(contents);

	/* Scopes stack */
	bool ignoreNextScope = false;

	/* Iterate tokens to find user-defined types. */
	while (!mTokenContext->isEnd())
	{
		parseTokens();

		mTokenContext->advance(1);
	}
}

/* Split the file into tokens for parsing */
void ReflectGen::tokenizeFile(const std::string& contents) noexcept
{
	std::string buffer = "";
	TokenList tokens;

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

	if (mTokenContext)
	{
		delete mTokenContext;
		mTokenContext = nullptr;
	}

	mTokenContext = new TokenContext(std::move(tokens));

	mContext.tokenContext = mTokenContext;
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

	mDescriptorContext.outputToFile(fileStream);

	fileStream.close();
}

void ReflectGen::parseTokens() noexcept
{
	const auto& curToken = mTokenContext->getToken(0);

	const auto& nameSpaceStack = mScopeManager.getScopeList(ScopeType_Namespace);

	uint32_t hash = fnv1a32(curToken.c_str());
	ITokenBehavior* behaviorPtr = getTokenBehavior(hash);

	if (behaviorPtr)
		behaviorPtr->handle(mContext);
}