#include "pch.h"
#include "Class.h"

#include "Core/TokenContext.h"

#include "Scope/ScopeManager.h"
#include "Descriptor/DescriptorContext.h"

void ClassBehavior::handle(const ReflectContext& context)
{
	TokenContext* tokenContextPtr = context.tokenContext;

	const auto& className = tokenContextPtr->getToken(1);
	const auto& prevToken = tokenContextPtr->getToken(-1);

	/* friend class of a type */
	if (prevToken == "friend")
		return;

	/* class Object; */
	const bool isForwardDecl = tokenContextPtr->getToken(2) == ";";

	TypeDesc desc;
	desc.name = className;
	desc.defType = "class";
	desc.parentNameSpace = context.getCurrentNamespace();

	desc.parentNameSpace->mChildTypes.push_back(desc);

	/* Only push scope if this is not a forward declaration */
	if (!isForwardDecl)
		context.scopeManager->pushScope(className, ScopeType_Class);
}