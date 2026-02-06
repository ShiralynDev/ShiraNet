# Code guidelines
This file contains all the guidelines we expect you to follow when contributing to the project. Please do note that these are still only guidelines and are not required to be strictly followed. Creating a pull request that goes against these guidelines will lead to a discussion as to why the code can't or shall not follow the guidelines which in turn can lead to changes in the guidelines or exceptions.

## Version
This library is created to be compatible with C++20 and shall therefore not use any features from versions after C++20.

## Header files
Every .cpp file should have its own .hpp file, preferably in the same separate directory.

### Guards
One common way to protect headers from being included multiple times is by using the so-called "ifdef guard" but this project prefers `#pragma once` at the top of the header file.

### Include what you use
All files included should be used, no unused includes! If a .cpp file like foo.cpp uses a function from bar.hpp it should include it even if foo.hpp includes it, this helps with clarity of where functions come from.

### Include order
In the .cpp files always keep its header include on top followed by a blank row, local project headers, blank row, C library headers, blank row, C++ library headers

foo.cpp:
```cpp
// this file's header
#include "foo.hpp"

// local project headers
#include "system/sockets.hpp"

// C library headers
#include <raylib.h>
#include <raymath.h>
#include <steam_api.h>

// cpp library headers
#include <RaylibAdditions.hpp>
#include <string>
```
System specific libraries that need ifdefs can be put after all of the other includes.

### System specific code
Try to keep system specific mixed code to a minimum, meaning having one piece of code with a bunch of ifdefs for different OS. Prefer having duplicate code for each different system. For example, if socket implementation is the same in the header but needs different code in the .cpp file, use a socketWindows.cpp and socketUnix.cpp instead of having multiple ifdefs in one shared file.

## Scopes
Scopes are used to keep the code clean and readable.

### Namespaces
Namespaces should be used to provide clarity and ease of use. The main namespace for the entire library is `ShiraNet`. If you are creating sockets implementation you could use namespaces such as `ShiraNet::Sockets` and in there define classes for different types of sockets or namespaces if needed. You should never use the keyword `using` as this makes it much harder to follow where functions stem from. In .hpp files namespaces should be defined with `namespace Foo::Bar` whilst in .cpp files they should be directly used without a big namespace statement `Foo::Bar::TcpSocket::Create()`

## Variable definition
### Local variables
All variables should be initialized on creation
Initalizing with functions is allowed but not prefered.
```cpp
int i;       // bad
int i = 0;   // good
int i = f(); // meh
```

## Naming
Folders, functions and varibles should be named using camelCase <br>
Files, Namespaces and classes should be named using PascalCase <br>
function args should be named using PascalCase (allows same name for class function args as class memebers) <br>
Enums and simmilare should be named using MACRO_CASE <br>

Prefer using full names for varibles, abbreviations like addr might be faster to write but address makes it much clearer to the reader without having to guess what it is and monitors today are wide enough that you can sacrifice that space.
```cpp
namespace ShiraNet::Sockets {
    class TcpSocket {
        private:
            int count = 0;
            std::string ipAddress = "";
        public:
            create(std::string IpAdress) { ipAddress = IpAddress; }; // prefer not using inline defenitions.
    };
}
```