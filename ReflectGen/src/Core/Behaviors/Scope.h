#pragma once

#include "../TokenBehavior.h"

class BeginScopeBehavior : public ITokenBehavior
{
public:
	virtual ~BeginScopeBehavior() = default;
	virtual void handle(const ReflectContext& context) override;
};

class EndScopeBehavior : public ITokenBehavior
{
public:
	virtual ~EndScopeBehavior() = default;
	virtual void handle(const ReflectContext& context) override;
};