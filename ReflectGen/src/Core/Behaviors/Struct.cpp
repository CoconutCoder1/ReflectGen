#include "pch.h"
#include "Struct.h"

#include "Core/TokenContext.h"

#include "Scope/ScopeManager.h"
#include "Descriptor/DescriptorContext.h"

void StructBehavior::handle(const ReflectContext& context)
{
	TokenContext* tokenContextPtr = context.tokenContext;

	const auto& structName = tokenContextPtr->getToken(1);
	const auto& prevToken = tokenContextPtr->getToken(-1);

	/* friend struct of a type */
	if (prevToken == "friend")
		return;

	/* struct Object; */
	const bool isForwardDecl = tokenContextPtr->getToken(2) == ";";

	TypeDesc desc;
	desc.name = structName;
	desc.defType = "struct";
	desc.parentNameSpace = context.getCurrentNamespace();

	desc.parentNameSpace->mChildTypes.push_back(desc);

	/* Only push scope if this is not a forward declaration */
	if (!isForwardDecl)
		context.scopeManager->pushScope(structName, ScopeType_Struct);
}