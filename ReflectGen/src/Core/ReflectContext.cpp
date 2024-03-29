#include "pch.h"
#include "ReflectContext.h"

#include "Descriptor/DescriptorContext.h"

#include "Scope/ScopeManager.h"

NamespaceDesc* ReflectContext::getCurrentNamespace() const noexcept
{
	NamespaceDesc* result = nullptr;

	const auto& nameSpaceStack = scopeManager->getScopeList(ScopeType_Namespace);

	if (!nameSpaceStack.empty())
	{
		NamespaceDesc* parentNameSpace = descriptorContext->getGlobalNamespace();

		for (auto itr = nameSpaceStack.begin(); itr < nameSpaceStack.end(); ++itr)
		{
			parentNameSpace = parentNameSpace->findOrAddChild(itr->name, parentNameSpace);
		}

		result = parentNameSpace;
	}
	else
		result = descriptorContext->getGlobalNamespace();

	return result;
}