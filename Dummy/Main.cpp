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
class MyObject;
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
	return 0;
}