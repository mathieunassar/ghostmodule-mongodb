``` ascii
                                                   ____
_______  _      _  ______  ________ _________      '_' \
|  _____ |______| |      | |_______     |        \_\    \_/    _
|______| |      | |______| _______|     |           \____\____|_|
```

# ghostmodule template project

Lightweight, multiplatform and accessible framework for command line-based programs and C++ microservices.

https://github.com/mathieunassar/ghostmodule

This repository contains a template project that does the following:

- Provides a **default CMake structure** for the project;
- Depends on ghostmodule and **resolves the dependencies** with Conan;
- Provides **sample build scripts** for TravisCI and Appveyor.

|     Build system     |                         Build status                         |
| :------------------: | :----------------------------------------------------------: |
|  Windows (Appveyor)  | [![Build status](https://ci.appveyor.com/api/projects/status/0qewqv8g3b1epwgu/branch/master?svg=true)](https://ci.appveyor.com/project/mathieunassar/ghostmodule-conan/branch/master) |
| Linux & OSX (Travis) | [![Build Status](https://travis-ci.com/mathieunassar/ghostmodule-conan.svg?branch=master)](https://travis-ci.com/mathieunassar/ghostmodule-conan) |

## Issues

If you wish to report an issue or make a request for a package, please do so here:

https://github.com/mathieunassar/ghostmodule/issues

## Usage

#### Prerequisites

CMake and Conan must be installed on your build setup, along with one of the supported compilers.

Once `conan` is installed, the following remote must be added to locate ghostmodule's recipe:

```
$ conan remote add ghostrobotics "https://api.bintray.com/conan/mathieunassar/ghostrobotics"
```

#### Build

To build the template project, execute the following commands from the project's root:

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

#### Testing

If you defined test projects, you can enable testing by executing the following commands:

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON
cmake --build . --config Release
ctest . -C Release
```

