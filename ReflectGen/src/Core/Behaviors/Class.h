#pragma once

#include "../TokenBehavior.h"

class ClassBehavior : public ITokenBehavior
{
public:
	virtual ~ClassBehavior() = default;
	virtual void handle(const ReflectContext& context) override;
};