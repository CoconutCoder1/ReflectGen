#include "pch.h"
#include "TokenBehavior.h"

#include "FNV1a.h"

#include "Behaviors/Class.h"
#include "Behaviors/Struct.h"
#include "Behaviors/Namespace.h"
#include "Behaviors/Scope.h"
#include "Behaviors/CommonType.h"
#include "Behaviors/TypeAttribute.h"

#include "TypeAttributes.h"

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

template<typename T>
const char* getTypeName()
{
	const char* namePtr = typeid(T).name();

	if (strstr(namePtr, "class ") != nullptr) // class MyClass
		return namePtr + 6;											//       ^ Return a pointer to here

	if (strstr(namePtr, "struct ") != nullptr) // struct MyStruct
		return namePtr + 7;											 //        ^ Return a pointer to here

	return namePtr;
}

template<typename T>
void addCommonTypeBehavior()
{
	const char* typeName = getTypeName<T>();

	setTokenBehavior(fnv1a32(typeName), new CommonTypeBehavior(sizeof(T), typeName));
}

template<typename T>
void addCommonTypeBehavior(const char* typeName)
{
	setTokenBehavior(fnv1a32(typeName), new CommonTypeBehavior(sizeof(T), typeName));
}

// Takes the raw name of specified type
#define ADD_COMMON_TYPE_BEHAVIOR(_type) addCommonTypeBehavior<_type>(#_type)

void setupCommonTypes()
{
	// Built-in keywords
	addCommonTypeBehavior<char>();
	addCommonTypeBehavior<short>();
	addCommonTypeBehavior<int>();
	addCommonTypeBehavior<float>();
	addCommonTypeBehavior<double>();
	addCommonTypeBehavior<long>();

	// Integer types
	ADD_COMMON_TYPE_BEHAVIOR(int8_t);
	ADD_COMMON_TYPE_BEHAVIOR(int16_t);
	ADD_COMMON_TYPE_BEHAVIOR(int32_t);
	ADD_COMMON_TYPE_BEHAVIOR(int64_t);

	// Unsigned int types
	ADD_COMMON_TYPE_BEHAVIOR(uint8_t);
	ADD_COMMON_TYPE_BEHAVIOR(uint16_t);
	ADD_COMMON_TYPE_BEHAVIOR(uint32_t);
	ADD_COMMON_TYPE_BEHAVIOR(uint64_t);
}

void setupTokenBehaviors()
{
	setTokenBehavior(fnv1a32_CT("class"), new ClassBehavior);
	setTokenBehavior(fnv1a32_CT("struct"), new StructBehavior);

	setTokenBehavior(fnv1a32_CT("namespace"), new NamespaceBehavior);

	setTokenBehavior(fnv1a32_CT("{"), new BeginScopeBehavior);
	setTokenBehavior(fnv1a32_CT("}"), new EndScopeBehavior);

	setTokenBehavior(fnv1a32_CT("const"), new TypeAttributeBehavior(TypeAttribute_Const));
	setTokenBehavior(fnv1a32_CT("unsigned"), new TypeAttributeBehavior(TypeAttribute_Unsigned));
	setTokenBehavior(fnv1a32_CT("signed"), new TypeAttributeBehavior(TypeAttribute_Signed));
	setTokenBehavior(fnv1a32_CT("volatile"), new TypeAttributeBehavior(TypeAttribute_Volatile));

	setupCommonTypes();
}