/* ReflectGen_DontParse */

#include "ReflectTypeInfo.h"

/* Forward declared types */
// Function: main | NumArguments: 2
class TestClass;
class NewClass;
namespace mynamespace{
class MyObject;
namespace mynamespace2{
class MyObject;
} // mynamespace2
namespace mynamespace3{
// Function: PrintString | NumArguments: 1
class MyObject;
// Member Function: MyObject::TestMemberFunction1 | NumArguments: 0
// Member Function: MyObject::TestMemberFunction2 | NumArguments: 1
enum MyEnum;
} // mynamespace3
} // mynamespace

namespace reflectgen {

/* Declare type info */
const TypeInfo* detail::DTI<TestClass>::mTypeInfoPtr = new detail::TypeInfoImpl<TestClass>("TestClass");
const TypeInfo* detail::DTI<NewClass>::mTypeInfoPtr = new detail::TypeInfoImpl<NewClass>("NewClass");
const TypeInfo* detail::DTI<mynamespace::MyObject>::mTypeInfoPtr = new detail::TypeInfoImpl<mynamespace::MyObject>("mynamespace.MyObject");
const TypeInfo* detail::DTI<mynamespace::mynamespace2::MyObject>::mTypeInfoPtr = new detail::TypeInfoImpl<mynamespace::mynamespace2::MyObject>("mynamespace.mynamespace2.MyObject");
const TypeInfo* detail::DTI<mynamespace::mynamespace3::MyObject>::mTypeInfoPtr = new detail::TypeInfoImpl<mynamespace::mynamespace3::MyObject>("mynamespace.mynamespace3.MyObject");
const TypeInfo* detail::DTI<mynamespace::mynamespace3::MyEnum>::mTypeInfoPtr = new detail::TypeInfoImpl<mynamespace::mynamespace3::MyEnum>("mynamespace.mynamespace3.MyEnum");

} // reflectgen
