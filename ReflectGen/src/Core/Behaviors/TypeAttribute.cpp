#include "pch.h"
#include "TypeAttribute.h"

#include "Core/TokenContext.h"
#include "Core/TypeAttributes.h"

const char* typeAttribute_toString(uint32_t attributes)
{
	if (attributes == TypeAttribute_None)
		return "none";

	if (attributes == TypeAttribute_Const)
		return "const";

	if (attributes == TypeAttribute_Unsigned)
		return "unsigned";

	if (attributes == TypeAttribute_Signed)
		return "signed";

	if (attributes == TypeAttribute_Volatile)
		return "volatile";

	return "unknown";
}

TypeAttributeBehavior::TypeAttributeBehavior(uint32_t typeAttributeFlag)
	: mTypeAttributeFlag(typeAttributeFlag)
{
}

void TypeAttributeBehavior::handle(const ReflectContext& context)
{
#if 0 // Debug print
	printf("Type Attribute Added: %s\n", typeAttribute_toString(mTypeAttributeFlag));
#endif

	context.tokenContext->addTypeAttribute(mTypeAttributeFlag);
}