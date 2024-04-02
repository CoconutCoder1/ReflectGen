#pragma once

enum TypeAttributes : uint32_t
{
	TypeAttribute_None = 0,
	TypeAttribute_Const = (1 << 0),
	TypeAttribute_Unsigned = (1 << 1),
	TypeAttribute_Signed = (1 << 2),
	TypeAttribute_Volatile = (1 << 3),
};