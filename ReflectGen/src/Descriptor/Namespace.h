#pragma once

struct NamespaceDesc
{
	NamespaceDesc(const std::string& alias, NamespaceDesc* parentNamespacePtr)
		: alias(alias), mParentNamespace(parentNamespacePtr)
	{
		// mChildNamespaces.reserve(10);
	}

	~NamespaceDesc()
	{
		for (auto& childNamespacePtr : mChildNamespaces)
		{
			delete childNamespacePtr;
			childNamespacePtr = nullptr;
		}
		mChildNamespaces.clear();
	}

	std::string alias;
	std::vector<NamespaceDesc*> mChildNamespaces;
	std::vector<TypeDesc> mChildTypes;
	NamespaceDesc* mParentNamespace;

	bool operator==(const std::string& rhs) noexcept
	{
		return alias == rhs;
	}

	NamespaceDesc* findChild(const std::string& alias) noexcept
	{
		auto itr = std::find_if(mChildNamespaces.begin(), mChildNamespaces.end(), [alias](const NamespaceDesc* descPtr) {
			return alias == descPtr->alias;
		});

		return itr != mChildNamespaces.end() ? *itr : nullptr;
	}

	NamespaceDesc* findOrAddChild(const std::string& alias, NamespaceDesc* parent) noexcept
	{
		NamespaceDesc* result = findChild(alias);

		if (!result)
		{
			result = new NamespaceDesc(alias, parent);
			mChildNamespaces.push_back(result);
		}

		return result;
	}
};