#pragma once

class ScopeManager;
class TokenContext;
class DescriptorContext;

struct NamespaceDesc;

struct ReflectContext
{
	ScopeManager* scopeManager;
	TokenContext* tokenContext;
	DescriptorContext* descriptorContext;

	NamespaceDesc* getCurrentNamespace() const noexcept;
};