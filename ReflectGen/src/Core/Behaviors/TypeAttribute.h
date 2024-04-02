#pragma once

#include "../TokenBehavior.h"

class TypeAttributeBehavior : public ITokenBehavior
{
public:
	TypeAttributeBehavior(uint32_t typeAttributeFlag);
	virtual ~TypeAttributeBehavior() = default;
	virtual void handle(const ReflectContext& context) override;

private:
	uint32_t mTypeAttributeFlag;
};