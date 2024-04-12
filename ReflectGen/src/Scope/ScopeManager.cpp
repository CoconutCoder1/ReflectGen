#include "pch.h"
#include "ScopeManager.h"

Scope Scope_None = Scope("None", ScopeType_None, nullptr);

void ScopeManager::pushScope(const std::string& label, ScopeType type, void* dataPtr) noexcept
{
	enableIgnoreScope();

	mGenericScopes.emplace(label, type, dataPtr);
	mTypedScopes[type].emplace_back(label, type, dataPtr);
}

void ScopeManager::popScope(size_t count) noexcept
{
	for (size_t i = 0; i < count && !mGenericScopes.empty(); ++i)
	{
		ScopeType lastType = mGenericScopes.top().type;

		if (lastType == ScopeType_Multiple)
		{
			size_t numScopes = reinterpret_cast<size_t>(mGenericScopes.top().scopeDataPtr);

			count += std::max(numScopes, 1ULL);
		}

		mTypedScopes[lastType].pop_back();
		mGenericScopes.pop();
	}
}

size_t ScopeManager::getGenericScopeCount() const noexcept
{
	return mGenericScopes.size();
}

const Scope* ScopeManager::getCurrentScope() const noexcept
{
	if (mGenericScopes.empty())
		return &Scope_None;

	return &mGenericScopes.top();
}

const std::vector<Scope>& ScopeManager::getScopeList(ScopeType type) const noexcept
{
	return mTypedScopes[type];
}

bool ScopeManager::hasScopeType(ScopeType type) const noexcept
{
	return !getScopeList(type).empty();
}

bool ScopeManager::shouldIgnoreScope() const noexcept
{
	return mShouldIgnoreScope;
}

void ScopeManager::enableIgnoreScope() noexcept
{
	mShouldIgnoreScope = true;
}

void ScopeManager::disableIgnoreScope() noexcept
{
	mShouldIgnoreScope = false;
}

void ScopeManager::beginArgumentList() noexcept
{
	mArgumentListContexts.push({  });
}

void ScopeManager::endArgumentList() noexcept
{
	if (!mArgumentListContexts.empty())
		mArgumentListContexts.pop();
}

bool ScopeManager::isArgumentList() const noexcept
{
	return !mArgumentListContexts.empty();
}

ArgumentListContext& ScopeManager::getArgumentListContext() noexcept
{
	return mArgumentListContexts.top();
}