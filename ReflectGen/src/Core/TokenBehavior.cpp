#include "pch.h"
#include "TokenBehavior.h"

#include "FNV1a.h"

#include "Behaviors/Class.h"
#include "Behaviors/Struct.h"
#include "Behaviors/Namespace.h"
#include "Behaviors/Scope.h"

TokenBehaviorMap tokenBehaviorMap;

ITokenBehavior* getTokenBehavior(uint32_t hash) noexcept
{
	if (tokenBehaviorMap.contains(hash))
		return tokenBehaviorMap[hash];

	return nullptr;
}

void setTokenBehavior(uint32_t hash, ITokenBehavior* behaviorPtr) noexcept
{
	tokenBehaviorMap[hash] = behaviorPtr;
}

void setupTokenBehaviors()
{
	setTokenBehavior(fnv1a32_CT("class"), new ClassBehavior);
	setTokenBehavior(fnv1a32_CT("struct"), new StructBehavior);
	setTokenBehavior(fnv1a32_CT("namespace"), new NamespaceBehavior);
	setTokenBehavior(fnv1a32_CT("{"), new BeginScopeBehavior);
	setTokenBehavior(fnv1a32_CT("}"), new EndScopeBehavior);
}