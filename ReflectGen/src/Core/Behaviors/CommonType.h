#pragma once

#include "../TokenBehavior.h"
#include "../TypeAttributes.h"

class CommonTypeBehavior : public ITokenBehavior
{
public:
	CommonTypeBehavior(size_t typeSize, const char* typeNamePtr);
	virtual ~CommonTypeBehavior() = default;
	virtual void handle(const ReflectContext& context) override;

	size_t getTypeSize() const;

	void setAttributes(uint32_t attributes);
	uint32_t getAttributes() const;

private:
	const char* mTypeNamePtr;
	size_t mTypeSize;
	uint32_t mAttributes;
};