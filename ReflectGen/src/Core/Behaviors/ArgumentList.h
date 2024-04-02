#pragma once

#include "../TokenBehavior.h"

class BeginArgumentListBehavior : public ITokenBehavior
{
	virtual ~BeginArgumentListBehavior() = default;
	virtual void handle(const ReflectContext& context) override;
};

class EndArgumentListBehavior : public ITokenBehavior
{
	virtual ~EndArgumentListBehavior() = default;
	virtual void handle(const ReflectContext& context) override;
};