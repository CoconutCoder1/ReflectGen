/* ReflectGen_DontParse */

#include "ReflectTypeInfo.h"

/* Forward declared types */
// Function: printTypeInfo | NumArguments: 0
// Function: main | NumArguments: 2
class TestClass; // Size: 0x8 | Num Fields: 1
class NewClass; // Size: 0x0 | Num Fields: 0
namespace mynamespace{
class MyObject; // Size: 0x0 | Num Fields: 0
namespace mynamespace2{
class MyObject; // Size: 0x18 | Num Fields: 3
} // mynamespace2
namespace mynamespace3{
// Function: PrintString | NumArguments: 1
class MyObject; // Size: 0x0 | Num Fields: 0
// Member Function: MyObject::TestMemberFunction1 | NumArguments: 0
// Member Function: MyObject::TestMemberFunction2 | NumArguments: 1
enum MyEnum; // Size: 0x0 | Num Fields: 0
} // mynamespace3
} // mynamespace

namespace reflectgen {

/* Declare type info */
const TypeInfo* detail::DTI<TestClass>::mTypeInfoPtr = new detail::TypeInfoImpl<TestClass>("TestClass", 0x8);
const TypeInfo* detail::DTI<NewClass>::mTypeInfoPtr = new detail::TypeInfoImpl<NewClass>("NewClass", 0x0);
const TypeInfo* detail::DTI<mynamespace::MyObject>::mTypeInfoPtr = new detail::TypeInfoImpl<mynamespace::MyObject>("mynamespace.MyObject", 0x0);
const TypeInfo* detail::DTI<mynamespace::mynamespace2::MyObject>::mTypeInfoPtr = new detail::TypeInfoImpl<mynamespace::mynamespace2::MyObject>("mynamespace.mynamespace2.MyObject", 0x18);
const TypeInfo* detail::DTI<mynamespace::mynamespace3::MyObject>::mTypeInfoPtr = new detail::TypeInfoImpl<mynamespace::mynamespace3::MyObject>("mynamespace.mynamespace3.MyObject", 0x0);
const TypeInfo* detail::DTI<mynamespace::mynamespace3::MyEnum>::mTypeInfoPtr = new detail::TypeInfoImpl<mynamespace::mynamespace3::MyEnum>("mynamespace.mynamespace3.MyEnum", 0x0);

} // reflectgen
