# BringAuto Logger Library

The BringAuto Logger Library provides a robust and stable API for logging in internal projects. This library consists of header files (.hpp) and multiple implementations. To use the library, compile it with your desired implementation, then integrate the resulting `.so` file and headers into your application.

## Requirements

Before building the library, ensure the following dependencies are installed on your host system:

- **CMake** [>= 3.20]
- **C++20**
- **[cmlib](https://github.com/cmakelib/cmakelib)**
- **[spdlog](https://github.com/gabime/spdlog)**
- **[Google Test](https://github.com/google/googletest/blob/main/googletest/README.md)** (for running tests)

## Build Instructions

To build the library, follow these steps:

```bash
mkdir -p _build && cd _build
cmake .. -DCMAKE_INSTALL_PREFIX=<path_where_to_install> -DLIB_TYPE=SPDLOG -DCMLIB_DIR=<path_to_cmlib_dir> -DCMAKE_BUILD_TYPE=Release
make -j 8
```

### Environment Variable

If CMLIB is installed and the `CMLIB_DIR` environment variable is set, you can omit the `-DCMLIB_DIR=<path_to_cmlib_dir>` option.

### Implementations

Specify the logging implementation type with the `-DLIB_TYPE=<TYPE>` option. Supported types are:

- **DUMMY**: For testing purposes; outputs log messages to the console.
- **SPDLOG**: Utilizes the [spdlog](https://github.com/gabime/spdlog) logging library.

### Example Application

To build the example application, enable the `BRINGAUTO_SAMPLES` option in CMake:

```bash
cmake .. -DBRINGAUTO_SAMPLES=ON
```

The executable for the example application will be located in the `./_build/example/` directory.

### Running Tests

To build and run tests, use the following options:

- **Build Tests**: Use the flag `-DBRINGAUTO_TESTS=ON`. Test executables will be located in the `./_build/tests/` directory.

To execute the tests, run:

```bash
ctest .
```

from within the `_build` directory after configuring with `-DBRINGAUTO_TESTS=ON`.

## Usage

To integrate the library into your project:

1. **Generate a Package**: Use the `cpack` command to create a package.
2. **Install the Package**: Install the package on your system.
3. **Include in CMake**:

    ```cmake
    FIND_PACKAGE(libbringauto_logger)
    TARGET_LINK_LIBRARIES(<target> bringauto_logger::bringauto_logger)
    ```

4. **Initialize Logger**:

    - Call `Logger::addSink()` with the desired sink and parameters.
    - Call `Logger::init()` to finalize the setup.
    - After initialization, you can use the logging functions. Refer to the example application for detailed usage.

## Installation

To install the library, enable the `BRINGAUTO_INSTALL` option in CMake:

```bash
cmake .. -DBRINGAUTO_INSTALL=ON
```

To create a package, use the `BRINGAUTO_INSTALL` option and rename the package to follow our naming convention: `<packagename>_<version>_<architecture>-<distro>.<extension>`. For example: `libbringauto_logger_1.5.01_amd64-ubuntu2004.zip`.
