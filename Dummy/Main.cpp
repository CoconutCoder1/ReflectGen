#include <iostream>

#include "gen/ReflectTypeInfo.h"

class TestClass
{
};

class NewClass : public TestClass
{
};

namespace mynamespace {
namespace mynamespace2 {

class MyObject
{
};

} // namespace mynamespace2

class MyObject
{
};

} // namespace mynamespace

namespace mynamespace::mynamespace3 {

class MyObject
{
};

} // namespace mynamespace::mynamespace3

int main()
{
	/* Classes in the global namespace */
	printf("%s\n", reflectgen::getTypeInfo<NewClass>()->mClassNameStr);
	printf("%s\n", reflectgen::getTypeInfo<TestClass>()->mClassNameStr);

	/* Classes within namespaces */
	printf("%s\n", reflectgen::getTypeInfo<mynamespace::MyObject>()->mClassNameStr);
	printf("%s\n", reflectgen::getTypeInfo<mynamespace::mynamespace2::MyObject>()->mClassNameStr);
	printf("%s\n", reflectgen::getTypeInfo<mynamespace::mynamespace3::MyObject>()->mClassNameStr);

	float f = 10.f;
	const float cf = 50.f;
	double d = 20.0;
	uint64_t u64 = UINT64_MAX;

	printf("Float value: %f\n", f);
	printf("Double value: %f\n", d);
	printf("Uint64 value: %llx\n", u64);

	return 0;
}