#include <iostream>

#include "gen/ReflectTypeInfo.h"

class TestClass
{
	uint64_t member0;
};

class NewClass : public TestClass
{
};

namespace mynamespace {
namespace mynamespace2 {

class MyObject
{
	uint64_t member0;
	uint64_t member1;
	uint64_t member2;
};

} // namespace mynamespace2

class MyObject
{
};

} // namespace mynamespace

namespace mynamespace::mynamespace3 {

class MyObject
{
	void TestMemberFunction1()
	{

	}

	void TestMemberFunction2(int)
	{

	}
};

enum MyEnum
{
};

void PrintString(const char* thing)
{
	printf(thing);
}

} // namespace mynamespace::mynamespace3

void printTypeInfo(const reflectgen::TypeInfo* typeInfoPtr) noexcept
{
	printf("%s : 0x%X\n", typeInfoPtr->mClassNameStr, typeInfoPtr->mTypeSize);
}

int main(int, char**)
{
	/* Classes in the global namespace */
	printTypeInfo(reflectgen::getTypeInfo<NewClass>());
	printTypeInfo(reflectgen::getTypeInfo<TestClass>());

	/* Classes within namespaces */
	printTypeInfo(reflectgen::getTypeInfo<mynamespace::MyObject>());
	printTypeInfo(reflectgen::getTypeInfo<mynamespace::mynamespace2::MyObject>());
	printTypeInfo(reflectgen::getTypeInfo<mynamespace::mynamespace3::MyObject>());

	/* Enum */
	printTypeInfo(reflectgen::getTypeInfo<mynamespace::mynamespace3::MyEnum>());
	
	int test = 10;

	return 0;
}