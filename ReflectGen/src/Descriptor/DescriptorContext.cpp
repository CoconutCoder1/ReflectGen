#include "pch.h"
#include "DescriptorContext.h"

void DescriptorContext::outputToFile(std::ofstream& fileStream) noexcept
{
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
}

void DescriptorContext::beginPendingFunction()
{
	mIsPendingFunction = true;
}

void DescriptorContext::endPendingFunction()
{
	mIsPendingFunction = false;
}

FunctionDesc& DescriptorContext::getPendingFunctionDesc()
{
	return mFunctionDesc;
}

bool DescriptorContext::isPendingFunction() const
{
	return mIsPendingFunction;
}

/* Recursively forward declare all types within their namespaces */
void DescriptorContext::forwardDeclareTypes(std::ofstream& fileStream, const NamespaceDesc* namespaceDescPtr) noexcept
{
	bool isInsideNamespace = namespaceDescPtr != mGlobalNamespace;

	if (isInsideNamespace)
		fileStream << "namespace " << namespaceDescPtr->alias << "{\n";

	for (const auto& func : namespaceDescPtr->mChildFunctions)
	{
		fileStream << "// Function: " << func.name << " | NumArguments: " << func.numArguments << "\n";
	}

	for (const auto& type : namespaceDescPtr->mChildTypes)
	{
		fileStream << type.defType << " " << type.name << "; // Size: 0x" << std::hex << type.typeSize << " | Num Fields: " << type.fields.size() << "\n";

		for (const auto& func : type.childFunctions)
		{
			fileStream << "// Member Function: " << type.name << "::" << func.name << " | NumArguments: " << func.numArguments << "\n";
		}
	}

	for (const auto& childNamespace : namespaceDescPtr->mChildNamespaces)
		forwardDeclareTypes(fileStream, childNamespace);

	if (isInsideNamespace)
		fileStream << "} // " << namespaceDescPtr->alias << "\n";
}

/* Recursively declare type info for all types in their namespaces */
void DescriptorContext::declareTypeInfo(std::ofstream& fileStream, const NamespaceDesc* namespaceDescPtr) noexcept
{
	std::string identifierPrefix = "";
	std::string symbolPrefix = "";

	const NamespaceDesc* parentPtr = namespaceDescPtr;
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
			"const TypeInfo* detail::DTI<{}>::mTypeInfoPtr = new detail::TypeInfoImpl<{}>(\"{}\", 0x{:x});",
			symbolName, symbolName, identifierName, type.typeSize
		);

		fileStream << outputLine << "\n";
	}

	for (const auto& childNamespace : namespaceDescPtr->mChildNamespaces)
		declareTypeInfo(fileStream, childNamespace);
}