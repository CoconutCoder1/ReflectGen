#include "pch.h"
#include "Namespace.h"

#include "Core/TokenContext.h"

#include "Scope/ScopeManager.h"
#include "Descriptor/DescriptorContext.h"

void NamespaceBehavior::handle(const ReflectContext& context)
{
	ScopeManager* scopeManager = context.scopeManager;
	TokenContext* tokenContext = context.tokenContext;

	const auto& nextToken = tokenContext->getToken(1);
	const auto& prevToken = tokenContext->getToken(-1);

	/* using namespace ??; */
	if (prevToken == "using")
		return;

	/* namespace ?? = ??; */
	if (nextToken == "=")
		return;

	/* Anonymous namespace */
	if (nextToken == "{")
		return;

	size_t numNamespaces = 1;
	size_t baseIndex = 1;

	scopeManager->pushScope(nextToken, ScopeType_Namespace);

	/* namespace my::name::space */
	while (tokenContext->getToken(baseIndex + 1) == ":" && tokenContext->getToken(baseIndex + 2) == ":")
	{
		baseIndex += 3;
		scopeManager->pushScope(tokenContext->getToken(baseIndex), ScopeType_Namespace);
		numNamespaces++;
	}

	if (numNamespaces > 1)
		scopeManager->pushScope("MultipleNamespace", ScopeType_Multiple, reinterpret_cast<void*>(numNamespaces));
}