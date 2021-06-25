# libcontainer

This library provides a set of generic containers and container adaptors in a pure
C (C99) implementation.
This library is intended to be portable, and suitable for building larger tooling
or applications on top of.

## Purpose

The purpose of this library is to provide the same kind of generic, useful containers
similar to those found in the C++ STL or other modern languages. This started as a selfish
desire to not re-implement type-specific containers every time I needed to work with
a different underlying type. Working with generic containers does come with somewhat
of a performance cost, as the internal algorithms can't assume anything about the
shape or size of the items they operate on.

Ideally, this library provides a helpful base for building larger and more interesting
tools. By providing generic containers, more time can be spent on the interesting
problems to be solved, and less time spent wrangling data.

## Building the Library

To build this library from source, all that is required is the standard `make` program.
Download this repository, navigate to the project root and simply `make` to compile
and run the testing suite, as well as build the debug and release libraries.

For more help with the available `make` targets, see the menu provided by `make help`.

If building from source, there are a handful of additional macros which can be tuned to
your specific application:

| Macro Name | Description |
| :--- | :--- |
| `VERBOSE_TEST_SUCCESS`                  | List out a "Test Successful" message for each passing test |
| `LIBCONTAINER_ZERO_ON_RELEASE`          | `memset()` all containers to 0's after releasing           |
| `LIBCONTAINER_ARRAY_DEFAULT_CAPACITY`   | Minimum Capacity of Array_t objects unless specified       |
| `LIBCONTAINER_HASHMAP_LOAD_FACTOR`      | Hashmap Load Factor threshold before a table rehash        |
| `LIBCONTAINER_HASHMAP_DEFAULT_CAPACITY` | Hashmap default starting size                              |

To specify non-default values for any of these tunable parameters, simply set the variable as desired
when calling the `make` program, e.g. `make LIBCONTAINER_ARRAY_DEFAULT_CAPACITY=16 release`.
Note that all of these are initialized to sensible defaults, and making these kinds of
changes are only necessary if you wish to tune the performance for your use-case.

## Testing the Library.

This library comes with a self-test suite to validate the behaviour
of the public API exposed by the main library header `libcontainer.h`.
This testing suite can be easily accessed with the `make test` target.
This will build the library, along with the testing functions and an
entry-point stub, and run this executable. Any failing tests will be
printed out, as well as a message indicating the number of failed tests.

This library has been tested on x86_64 and arm64, as well as OS X, Ubuntu,
and Debian. Furthermore, the included `Valgrind-Test.sh` script can be
used to instrument the test executable with the Valgrind `memcheck` tool.

## Installing the Library

The `Makefile` included in this project defines `install` and `uninstall` targets to
perform a proper installation of the library, as well as uninstalling it.

To properly install the library, the process is as simple as:
```bash
#   Build and run the test suite, and build the debug and release libraries.
make

#   Install the release library, header, and man page to your system.
sudo make install
```
from the command line.

By default, this will place the library binaries (release and debug) into `/usr/local/lib`
and header file into `/usr/local/include`, and the man page in `/usr/local/share/man/man1/`.
Setting the environment variable `INSTALLDIR`
will allow updating the prefix used for the binary and header file, and updating the
`INSTALLMANDIR` environment variable will update the prefix for the man page.
Once installed, the soft-links `libcontainer.a`, `libcontainer-debug.a` and header `libcontainer.h`
will be accessible and stable across library versions.

An additional, optional, step you may wish to perform is to add the library and header folders
to the `INCLUDE_PATH` and `LIBRARY_PATH` so these are visible without any compiler flags.
More commonly, either these files will be linked into further projects, or these additional
include and library paths can be passed to the compiler in the other project.

## Using the Library

Once the library is installed, either to your system path or to a project using the library,
there are a handful of additional useful macros to help maintain a cleaner namespace.
Each of the containers can be individually enabled for each compilation unit by defining
the `LIBCONTAINER_ENABLE_<Container>` macros as desired. For example, if only the `Array_t`
container is desired in a specific file, simply add `#define LIBCONTAINER_ENABLE_ARRAY`
before `#include`-ing the library header to only bring the `Array_t` type and functions in.

The library binary itself contains all of the implemented containers, and this mechanism provides
a way to ensure only the containers you want end up compiled into your project.

## Available Containers

Currently, this library provides the following containers for use:

| Container | Enable Macro | Description |
| :--- | :--- | :--- |
| `Array_t`       | `LIBCONTAINER_ENABLE_ARRAY`       | Auto-resizing linear contiguous storage              |
| `List_t`        | `LIBCONTAINER_ENABLE_LIST`        | Doubly-Linked dynamic list                           |
| `Hashmap_t`     | `LIBCONTAINER_ENABLE_HASHMAP`     | Auto-balancing key-value associative array           |
| `Binary_Tree_t` | `LIBCONTAINER_ENABLE_BINARY_TREE` | Self-balancing binary search tree of key-value pairs |
| `Stack_t`       | `LIBCONTAINER_ENABLE_STACK`       | LIFO (Last-In-First-Out), single-item access         |
| `String_t`      | `LIBCONTAINER_ENABLE_STRING`      | Richer, safe, dynamically growable String type       |

The internal implementations of these containers should not be a concern for their use. For example,
currently the Binary_Tree_t implementation is based on an [AVL Tree](https://en.wikipedia.org/wiki/AVL_tree),
but no part of this implementation is externally visible. This library will always ensure
the public interface does not rely on the specific implementation of the underlying container.

If you wish to enable all containers, the macro `LIBCONTAINER_ENABLE_ALL` is a shorthand to
enable all containers available in the library.

### Future Containers and Updates

In addition to the set of containers listed in the section above, there are plans to
implement the following additional containers:
| Container | Enable Macro | Description |
| :--- | :--- | :--- |
| `Set_t`            | `LIBCONTAINER_ENABLE_SET`            | Container of explicitly unique keys                    |
| `Queue_t`          | `LIBCONTAINER_ENABLE_QUEUE`          | FIFO (First-In First-Out) item access                  |
| `Priority_Queue_t` | `LIBCONTAINER_ENABLE_PRIORITY_QUEUE` | Priority-ordered FIFO (First-In First-Out) item access |

Note that the containers listed in this section are still tentative. Only those provided in the section
above are implemented as of now.

In addition to these new containers, there are a handful of outstanding extensions to the
existing containers that will be implemented at some point, likely before the v.1.0.0 release.
These include:

* Extending `Binary_Tree_t` to accept a generic Key, not limited to `int`.
* Extending `Binary_Tree_t` to specify duplicate-item handling, i.e. ignore-new, overwrite, error, etc.

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
