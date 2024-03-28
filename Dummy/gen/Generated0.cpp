/* ReflectGen_DontParse */

#include "ReflectTypeInfo.h"

/* Forward declared types */
class TestClass;
class NewClass;

namespace reflectgen {

/* Declare type info */
const TypeInfo* detail::DTI<TestClass>::mTypeInfoPtr = new detail::TypeInfoImpl<TestClass>("TestClass");
const TypeInfo* detail::DTI<NewClass>::mTypeInfoPtr = new detail::TypeInfoImpl<NewClass>("NewClass");

} // reflectgen
