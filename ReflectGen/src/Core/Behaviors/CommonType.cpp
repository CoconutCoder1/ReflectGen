#include "pch.h"
#include "CommonType.h"

#include "Core/TokenContext.h"

std::unordered_map<std::string, TypeAttributes> recognizedAttributes = {
	{ "const", TypeAttribute_Const },
	{ "unsigned", TypeAttribute_Unsigned },
	{ "signed", TypeAttribute_Signed },
	{ "volatile", TypeAttribute_Volatile },
};

CommonTypeBehavior::CommonTypeBehavior(size_t typeSize, const char* typeNamePtr)
	: mTypeSize(typeSize), mTypeNamePtr(typeNamePtr), mAttributes(TypeAttribute_None)
{
}

void CommonTypeBehavior::handle(const ReflectContext& context)
{
	/* Get attributes for the used type */
	mAttributes = context.tokenContext->getTypeAttributes();

	/* Clear list of attributes for the next type */
	context.tokenContext->clearTypeAttributes();

#if 1 /* Debug print */
	printf("Type Detected: [ Name: \"%s\", Size: %x, Attributes: %x ]\n", mTypeNamePtr, mTypeSize, mAttributes);
#endif
}

size_t CommonTypeBehavior::getTypeSize() const
{
	return mTypeSize;
}

void CommonTypeBehavior::setAttributes(uint32_t attributes)
{
	mAttributes = attributes;
}

uint32_t CommonTypeBehavior::getAttributes() const
{
	return mAttributes;
}