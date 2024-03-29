#pragma once

#include "Scope.h"

class ScopeManager
{
public:
	void pushScope(const std::string& label, ScopeType type, void* dataPtr = nullptr) noexcept;
	void popScope(size_t count = 1) noexcept;

	size_t getGenericScopeCount() const noexcept;

	const Scope* getCurrentScope() const noexcept;
	const std::vector<Scope>& getScopeList(ScopeType type) const noexcept;

	bool shouldIgnoreScope() const noexcept;
	void enableIgnoreScope() noexcept;
	void disableIgnoreScope() noexcept;

private:
	std::stack<Scope> mGenericScopes;
	std::vector<Scope> mTypedScopes[ScopeType_Count];
	bool mShouldIgnoreScope;
};