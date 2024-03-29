#pragma once

#include "Scope/ScopeManager.h"

#include "ReflectContext.h"

#include "../Descriptor/DescriptorContext.h"

namespace fs = std::filesystem;

struct NamespaceDesc;
class TokenContext;

class ReflectGen
{
public:
	void generate(const std::string& projectRootDir) noexcept;

private:
	void parseFile(const fs::path& filePath) noexcept;
	bool isValidFile(const fs::path& filePath) noexcept;
	void findTypes(const std::string& contents) noexcept;
	void tokenizeFile(const std::string& contents) noexcept;
	void generateTypeFile(const std::string& fileName) noexcept;

	void parseTokens() noexcept;

private:
	// std::vector<TypeDesc> mFoundTypes;

	DescriptorContext mDescriptorContext;
	ScopeManager mScopeManager;
	TokenContext* mTokenContext;

	ReflectContext mContext;

	bool mIgnoreNextScope;
};