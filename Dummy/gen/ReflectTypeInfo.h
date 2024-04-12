

#ifndef REFLECTGEN_TYPES_H
#define REFLECTGEN_TYPES_H

// ReflectGen_DontParse

namespace reflectgen {

/* ReflectGen_Field */
#define RG_FIELD

class TypeInfo
{
public:
	const char* mClassNameStr;
	unsigned int mTypeSize;
	TypeInfo* mParentPtr;
};

namespace detail {

template<typename T>
class TypeInfoImpl : public TypeInfo
{
public:
	TypeInfoImpl(const char* classNameStr, unsigned int typeSize) noexcept
	{
		mClassNameStr = classNameStr;
		mTypeSize = typeSize;
	}
};

/* Declared Type Info */
template<typename T>
class DTI
{
public:
	static const TypeInfo* mTypeInfoPtr;
};

} // namespace detail

template<typename T>
const TypeInfo* getTypeInfo() noexcept
{
	return detail::DTI<T>::mTypeInfoPtr;
}

void initialize();

} // namespace reflectgen

#endif

