#include "pch.h"
#include "ArgumentList.h"

#include "Scope/ScopeManager.h"
#include "Descriptor/DescriptorContext.h"

void BeginArgumentListBehavior::handle(const ReflectContext& context)
{
	context.scopeManager->beginArgumentList();
}

void EndArgumentListBehavior::handle(const ReflectContext& context)
{
	context.scopeManager->endArgumentList();

	if (context.descriptorContext->isPendingFunction())
	{
		context.descriptorContext->endPendingFunction();
		auto& funcDesc = context.descriptorContext->getPendingFunctionDesc();

		auto scopeType = context.scopeManager->getCurrentScope()->type;
		
		if (scopeType == ScopeType_Class || scopeType == ScopeType_Struct)
		{
			auto& typeDesc = context.getCurrentNamespace()->mChildTypes.back();

			typeDesc.childFunctions.push_back(funcDesc);
		}
		else
		{
			auto* nameSpaceDescPtr = context.getCurrentNamespace();

			nameSpaceDescPtr->mChildFunctions.push_back(funcDesc);
		}
	}
}