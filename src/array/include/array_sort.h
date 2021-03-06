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

#ifndef LIBCONTAINER_ARRAY_SORT_H
#define LIBCONTAINER_ARRAY_SORT_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

/* ++++++++++ Private Functions ++++++++++ */

void Array_swapItems(Array_t* Array, size_t IndexA, size_t IndexB);
ssize_t Array_doPartition(Array_t* Array, CompareFunc_t* CompareFunc, ssize_t Lower, ssize_t Upper);
int Array_doQuickSort(Array_t* Array, CompareFunc_t* CompareFunc, ssize_t Lower, ssize_t Upper);

/* ---------- Private Functions ---------- */

#if defined(TESTING) || defined(DEBUGGER)

#include "array_sort_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
