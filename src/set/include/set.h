/*
    MIT License

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
*/

#ifndef LIBCONTAINER_SET_H
#define LIBCONTAINER_SET_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#define LIBCONTAINER_ENABLE_SET
#define LIBCONTAINER_ENABLE_BINARY_TREE
#include "../../../include/libcontainer.h"

struct Set_t {

    /*
        A set is simply an interface over a Binary_Tree_t.

        The Binary_Tree_t already provides a container with
        guaranteed unique keys, as well as a sorted iterator
        interface. Furthermore, the comparable C++ container
        (std::ordered_set) is based on a self-balancing binary
        tree to provide the time complexity guarantees
        required.
    */
    Binary_Tree_t* Contents;
};

#if defined(TESTING) || defined(DEBUGGER)

#include "set_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
