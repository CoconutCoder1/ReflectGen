#pragma once

#include "../TokenBehavior.h"

class StructBehavior : public ITokenBehavior
{
public:
	virtual ~StructBehavior() = default;
	virtual void handle(const ReflectContext& context) override;
};