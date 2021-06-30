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

#ifndef LIBCONTAINER_BINARY_HEAP_H
#define LIBCONTAINER_BINARY_HEAP_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#define LIBCONTAINER_ENABLE_BINARY_HEAP
#define LIBCONTAINER_ENABLE_ARRAY
#include "../../../../include/libcontainer.h"

#include "binary_heap_node.h"
#include "../../../iterator/include/iterator.h"
#include "../../../array/include/array.h"

#define PARENT_INDEX(Index) ((0 == (Index)) ? (0) : ( ((Index)-1)/2 ))
#define LEFT_CHILD_INDEX(Index) (2*(Index) + 1)
#define RIGHT_CHILD_INDEX(Index) (2*(Index) + 2)

struct Binary_Heap_t {

    Array_t* Items;

    CompareFunc_t* KeyCompareFunc;
    ReleaseFunc_t* KeyReleaseFunc;
};

/* ++++++++++ Private Binary_Heap_t Functions ++++++++++ */

/*
    Binary_Heap_siftUp

    This function performs the Sift-Up operation during Heap <>.
    This ensures the Heap always satisfies the Heap property.

    Inputs:
    Heap    -   Pointer to the Binary_Heap_t to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure or error.
*/
int Binary_Heap_siftUp(Binary_Heap_t* Heap);

/*
    Binary_Heap_siftDown

    This function performs the Sift-Down operation during Heap <>.
    This ensures the Heap always satisfies the Heap property.

    Inputs:
    Heap    -   Pointer to the Binary_Heap_t to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure or error.
*/
int Binary_Heap_siftDown(Binary_Heap_t* Heap);

/* ---------- Private Binary_Heap_t Functions ---------- */

#if defined(TESTING) || defined(DEBUGGER)

#include "binary_heap_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
