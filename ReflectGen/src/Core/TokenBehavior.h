#pragma once

#include "ReflectContext.h"

class TokenContext;
class DescriptorContext;

class ITokenBehavior
{
public:
	virtual ~ITokenBehavior() = default;
	virtual void handle(const ReflectContext& context) = 0;
};

using TokenBehaviorMap = std::unordered_map<uint32_t, ITokenBehavior*>;

ITokenBehavior* getTokenBehavior(uint32_t hash) noexcept;
void setTokenBehavior(uint32_t hash, ITokenBehavior* behaviorPtr) noexcept;

void setupTokenBehaviors();