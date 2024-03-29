#pragma once

#include "../TokenBehavior.h"

class NamespaceBehavior : public ITokenBehavior
{
public:
	virtual ~NamespaceBehavior() = default;
	virtual void handle(const ReflectContext& context) override;
};