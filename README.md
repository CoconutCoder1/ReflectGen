# ReflectGen
Attempt at somewhat bringing some sort of reflection to C++.

It works by parsing the project files, source and header files, to generate type info files.

Ideally this project will provide more than just class and struct name in the future.

# Usage
In the project provided, ReflectGen is set as a Pre-Build Event command.

This is so that it automatically generates type info files when building the project.

```
ReflectGen.exe <project/root/source/directory>
```

Then getting information about a user-defined type:

```cpp
#include "gen/ReflectTypeInfo.h"

class MyObject {};

const reflectgen::TypeInfo* myObjectInfoPtr = reflectgen::getTypeInfo<MyObject>();
```
