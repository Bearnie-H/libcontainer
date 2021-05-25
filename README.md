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

To change any of these macros, simply add the necessary flags to the `CFLAGS` variable
of the project `Makefile`, e.g. (`-DLIBCONTAINER_ARRAY_DEFAULT_CAPACITY=16`). Note
that all of these are initialized to sensible defaults, and making these kinds of
changes are only necessary if you wish to tune the performance for your use-case.

...

## Installing the Library

...

## Examples

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
