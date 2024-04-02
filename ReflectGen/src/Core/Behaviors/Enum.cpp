#include "pch.h"
#include "Enum.h"

#include "Core/TokenContext.h"

#include "Scope/ScopeManager.h"
#include "Descriptor/DescriptorContext.h"

void EnumBehavior::handle(const ReflectContext& context)
{
	size_t nameIndex = 1;
	std::string enumName = context.tokenContext->getToken(nameIndex);

	/* enum class MyEnumClass */
	if (enumName == "class")
	{
		++nameIndex;
		enumName = context.tokenContext->getToken(nameIndex);
	}

	/* enum MyEnum; */
	const bool isForwardDecl = context.tokenContext->getToken(nameIndex + 1) == ";";

	TypeDesc desc;
	desc.name = enumName;
	desc.defType = "enum";
	desc.parentNameSpace = context.getCurrentNamespace();

	desc.parentNameSpace->mChildTypes.push_back(desc);

	/* Only push scope if this is not a forward declaration */
	if (!isForwardDecl)
		context.scopeManager->pushScope(enumName, ScopeType_Enum);
}