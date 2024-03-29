#include "pch.h"

#include "Core/ReflectGen.h"
#include "Core/TokenBehavior.h"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Project root directory must be provided in the command line arguments\n");
		return 1;
	}

	setupTokenBehaviors();

	ReflectGen gen;
	gen.generate(argv[1]);

	return 0;
}