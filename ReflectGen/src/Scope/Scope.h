#pragma once

enum ScopeType : int
{
	ScopeType_Unknown,
	ScopeType_Namespace,
	ScopeType_Multiple,
	ScopeType_Class,
	ScopeType_Struct,
	ScopeType_Count,
};

struct Scope
{
	Scope(const std::string& name, ScopeType type, void* scopeDataPtr)
		: name(name), type(type), scopeDataPtr(scopeDataPtr)
	{
	}

	std::string name;
	ScopeType type;
	void* scopeDataPtr;
};