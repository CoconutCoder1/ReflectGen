#pragma once

#include "../TokenBehavior.h"

class EnumBehavior : public ITokenBehavior
{
public:
	virtual ~EnumBehavior() = default;
	virtual void handle(const ReflectContext& context) override;
};