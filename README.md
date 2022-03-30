# BringAuto logger library #

This library provides stable API for logging in our internal project. Project consists of .hpp header files and multiple
implementations. To use this project compile it with wanted implementation and provide .so and headers to your app.

## Requirements

Following dependencies must be installed on the host system

- cmake [>= 3.20]
- C++17
- [cmlib](https://github.com/cmakelib/cmakelib)

## Build

```
mkdir -p _build && cd _build
cmake .. -DCMAKE_INSTALL_PREFIX=/home/nothrax/Documents/test_install -DLIB_TYPE=SPDLOG -DCMLIB_DIR=/home/nothrax/Documents/cmakelib -DCMAKE_BUILD_TYPE=Release
make -j 8
```

### Implementations
Choose with `-DLIB_TYPE=<TYPE>`, supported types:
- DUMMY - testing purposes, outputs test messages into console
- SPDLOG - uses [spdlog](https://github.com/gabime/spdlog) logging library

### Example

For building example app use -DBRINGAUTO_SAMPLES=ON with cmake

### Tests
- For building tests use -DBRINGAUTO_TESTS=ON 
- For api and chosen implementation tests or -DBRINGAUTO_ALL_TESTS=ON 
- For api and all implementation tests, run test with command ctest

## Usage

Generate package with cpack command, install the package and include:

```
FIND_PACKAGE(libbringauto_logger)
TARGET_LINK_LIBRARIES(<target> bringauto_logger::bringauto_logger)
```

To create logger first call Logger::addSink() function with specific sink and parameters. After adding all desired sinks
call Logger::init() with settings. After initialization you are able to call all log functions. See example app for more
details.

# Installation

- For installation provide -DBRINGAUTO_INSTALL switch to cmake
- For package creation provide -DBRINGAUTO_INSTALL switch to cmake, created package rename to match our package naming
  convention: `<packagename>_<version>_<architecture>-<distro>.<extension>`, for example `libbringauto_logger_1.5.01_amd64-ubuntu2004.zip`
