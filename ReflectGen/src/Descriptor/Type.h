#pragma once

struct TypeDesc
{
	std::string defType; /* struct, class */
	std::string name; /* typename */
	struct NamespaceDesc* parentNameSpace;
};