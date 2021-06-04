/*
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

#ifndef LIBCONTAINER_ARRAY_H
#define LIBCONTAINER_ARRAY_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <sys/types.h>
#include <stdint.h>

#define LIBCONTAINER_ENABLE_ARRAY

#include "../../../include/libcontainer.h"
#include "array_callbacks.h"

#ifndef LIBCONTAINER_ARRAY_DEFAULT_CAPACITY
#define LIBCONTAINER_ARRAY_DEFAULT_CAPACITY 8 /* The default capacity of arrays unless otherwise specified. */
#endif

#define ARRAY_DOUBLING_THRESHOLD 4096

struct Array_t {

    /*
        ReleaseFunc is used if the elements are pointer-types which hold their
        own memory. This allows providing an outside hook for releasing memory
        held by the objects within an array when the array itself is released.

        This only has meaning if the IsReference value for an array is true.
    */
    ReleaseFunc_t* ReleaseFunc;

    /*
        Contents is the start of the memory address holding the
        actual data of the array. This is represented as a sequence of
        raw bytes, with segmenting and indexing performed explicitly
        by this library.
    */
    uint8_t* Contents;

    /*
        The number of "live" elements within the array. This is also used
        to find the end of the memory segment held by this array, for inserting
        or appending new elements.
    */
    size_t Length;

    /*
        The number of elements which this array owns memory to hold.
        For efficiency, the array typically holds more memory than is
        actively used, so as to reduce the number of allocations or reallocations
        as elements are added or removed.
    */
    size_t Capacity;

    /*
        The size (in bytes) of a single element held by the array.
        Since this array must be able to hold arbitrary types, we need to know
        how many bytes each element takes up in order to correctly index into
        the Contents pointer.
    */
    size_t ElementSize;

    /*
        IsReference allows disambiguating whether the elements held by this
        array are held WITHIN the array, or if the array simply holds pointers
        to memory held by something else.
    */
    bool IsReference;

    /*
        IsShadow allow disambiguating whether or not this array is a shadow copy
        of another (as created by Array_Duplicate), and therefore should not
        attempt to release the memory of it's contents on release if it's a reference-type.
    */
    bool IsShadow;

};

/*
    Array_Grow

    This function will grow the array, ensuring it has enough capacity to hold at least
    AdditionalCapacity worth of elements.

    Inputs:
    Array               -   Pointer to the array to grow.
    AdditionalCapacity  -   The number of additional elements requested to hold.

    Outputs:
    int -   Returns 0 on success, non-zero on failure.
*/
int Array_Grow(Array_t* Array, size_t AdditionalCapacity);

/*
    Array_Capacity

    This function returns the number of items a given Array_t can hold before allocating more memory.

    Inputs:
    Array   -   The array to determine the capacity of.

    Outputs:
    size_t -   The capacity of the array (0 inclusive) on success, 0 on failure.

    Note:
    This operation is O(1) in the capacity of the array.
*/
size_t Array_Capacity(Array_t* Array);

#if defined(TESTING) || defined(DEBUGGER)

#include "array_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif

