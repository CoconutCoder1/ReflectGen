#include <iostream>

#include "gen/ReflectTypeInfo.h"

class TestClass
{
};

class NewClass
{
};

int main()
{
	printf("%s\n", reflectgen::getTypeInfo<NewClass>()->mClassNameStr);
	printf("%s\n", reflectgen::getTypeInfo<TestClass>()->mClassNameStr);
	return 0;
}