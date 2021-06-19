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

#ifndef LIBCONTAINER_HASHMAP_ITERATOR_H
#define LIBCONTAINER_HASHMAP_ITERATOR_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#define LIBCONTAINER_ENABLE_ARRAY
#define LIBCONTAINER_ENABLE_HASHMAP
#include "../../../include/libcontainer.h"

/*
    Iterator_Initialize_Hashmap

    This function initializes the Iterator for a Hashmap, preparing for forward or reverse
    iteration over the Map.

    Inputs:
    Map             -   Pointer to the Hashmap to operate on.
    CompareFunc     -   (Optional) function to use if the iteration order
                            should be the Sorted Keys. Pass NULL to
                            iterate in the arbitrary order returned by *_Keys().

    Outputs:
    void*   -   Pointer to the corresponding item from the Hashmap.
*/
int Iterator_Initialize_Hashmap(Hashmap_t *Map, CompareFunc_t* CompareFunc);

#if defined(TESTING) || defined(DEBUGGER)

#include "hashmap_iterator_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
