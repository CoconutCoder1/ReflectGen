#pragma once

struct FieldDesc
{
	std::string fieldName;
	uint32_t size;
	uint32_t offset;
};

struct TypeDesc
{
	std::string defType; /* struct, class */
	std::string name; /* typename */
	struct NamespaceDesc* parentNameSpace;
	std::vector<FunctionDesc> childFunctions;
	std::vector<FieldDesc> fields;
	size_t typeSize = 0;
};