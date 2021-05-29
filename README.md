# libcontainer

This library provides a set of generic containers in a pure C (C99) implementation.

## Purpose

...

## Building the Library

To build this library from source, all that is required is the standard `make` program.
Download this repository, navigate to the project root and simply `make` to compile
and run the testing suite, as well as build the debug and release libraries.

For more help with the available `make` targets, see the menu provided by `make help`.

If building from source, there are a handful of additional macros which can be tuned to
your specific application:

| Macro Name | Description |
| :----------: | :---------- |
| `LIBCONTAINER_ARRAY_DEFAULT_CAPACITY` | Minimum Capacity of Array_t objects unless specified |

To specify non-default values for any of these tunable parameters, simply set the variable as desired
when calling the `make` program, e.g. `make LIBCONTAINER_ARRAY_DEFAULT_CAPACITY=16 release`.
Note that all of these are initialized to sensible defaults, and making these kinds of
changes are only necessary if you wish to tune the performance for your use-case.

## Installing the Library

The `Makefile` included in this project defines placeholder `install` and `uninstall` targets to perform
this installation. Currently these are no-ops and simply error out, but at some point these will
fully install the compiled library and header file to the necessary paths.

Until the `make install` target is defined, the library can be installed by placing it where your
linker can see (or by including it into the other project requiring it), and the main library header
`libcontainer.h` into the compiler include-path.

...

## Using the Library

Once the library is installed, either to your system path or to a project using the library,
there are a handful of additional useful macros to help maintain a cleaner namespace.
Each of the containers can be individually enabled for each compilation unit by defining
the `LIBCONTAINER_ENABLE_<Container>` macros as desired. For example, if only the Array_t
container is desired in a specific file, simply add `#define LIBCONTAINER_ENABLE_ARRAY`
before `#include`-ing the library header to only bring the Array_t type and functions in.

The library binary itself contains all of the implemented containers, and this mechanism provides
a way to ensure only the containers you want end up compiled into your project.

...

### Examples

As this library provides a significant number of functionality, only a small set of example
operations are provided here.

...

## License

Copyright (c) 2021 Bearnie-H

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
