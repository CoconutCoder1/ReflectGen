#include "pch.h"
#include "Scope.h"

#include "CommonType.h"

#include "Core/FNV1a.h"

#include "Core/TokenContext.h"
#include "Core/TokenBehavior.h"

#include "Scope/ScopeManager.h"
#include "Descriptor/DescriptorContext.h"

#pragma warning(disable: 4996)

void BeginScopeBehavior::handle(const ReflectContext& context)
{
	ScopeManager* scopeManagerPtr = context.scopeManager;

	context.tokenContext->clearTypeAttributes();

	if (!scopeManagerPtr->shouldIgnoreScope())
	{
		scopeManagerPtr->pushScope("UnknownScope", ScopeType_Unknown);
		scopeManagerPtr->disableIgnoreScope();
	}
}

void EndScopeBehavior::handle(const ReflectContext& context)
{
	/*const Scope* scopePtr = context.scopeManager->getCurrentScope();
	if (scopePtr && scopePtr->type == ScopeType_Class)
	{
		auto nameSpace = context.getCurrentNamespace();
		auto& childType = nameSpace->mChildTypes.back();

		char* typeName = new char[childType.name.size() + 1];
		strcpy(typeName, childType.name.c_str());

		setTokenBehavior(fnv1a32(childType.name.c_str()), new CommonTypeBehavior(childType.typeSize, typeName));
	}*/

	context.scopeManager->popScope();
}
