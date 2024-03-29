#include "pch.h"
#include "Scope.h"

#include "Core/TokenContext.h"

#include "Scope/ScopeManager.h"
#include "Descriptor/DescriptorContext.h"

void BeginScopeBehavior::handle(const ReflectContext& context)
{
	ScopeManager* scopeManagerPtr = context.scopeManager;

	if (!scopeManagerPtr->shouldIgnoreScope())
	{
		scopeManagerPtr->pushScope("UnknownScope", ScopeType_Unknown);
		scopeManagerPtr->disableIgnoreScope();
	}
}

void EndScopeBehavior::handle(const ReflectContext& context)
{
	context.scopeManager->popScope();
}
