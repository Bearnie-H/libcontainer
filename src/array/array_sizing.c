/*
    Copyright (c) 2021 Bearnie-H

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
*/

#include <stdlib.h>
#include <string.h>

#include "../logging/logging.h"
#include "include/array.h"

int Array_Grow(Array_t *Array, size_t AdditionalCapacity) {

    size_t   NewCap = 1, MinCap = 0, ElementSize = sizeof(void *);
    uint8_t *Temp = NULL;

    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "Error, NULL Array_t* provided.");
        return 1;
    }

    if ( NULL == Array->Contents.ContentBytes ) {
        DEBUG_PRINTF("%s", "Error, Array_t* in invalid state with NULL Contents pointer.");
        return 1;
    }

    if ( 0 != Array->ElementSize ) {
        ElementSize = Array->ElementSize;
    }

    /* Compute the desired capacity, and then perform the efficient resize
     * algorithm. */
    MinCap = Array->Length + AdditionalCapacity;
    NewCap = Array->Capacity;

    /* Explicitly assert non-zero capacity. */
    if ( 0 == NewCap ) {
        NewCap = 1;
    }

    if ( Array->Capacity >= MinCap ) {
        DEBUG_PRINTF("%s", "Array_t capacity is already large enough to hold "
                           "the requested elements.");
        return 0;
    }

    while ( NewCap <= MinCap ) {
        if ( NewCap <= ARRAY_DOUBLING_THRESHOLD ) {
            NewCap <<= 1;
        } else {
            NewCap += ARRAY_DOUBLING_THRESHOLD;
        }
    }

    Temp = (uint8_t *)realloc(Array->Contents.ContentBytes, NewCap * ElementSize);
    if ( NULL == Temp ) {
        DEBUG_PRINTF("%s", "Failed to reallocate and grow Array_t Contents");
        return 1;
    }

    Array->Contents.ContentBytes = Temp;
    Array->Capacity              = NewCap;

    DEBUG_PRINTF("Successfully increased Array_t capacity to [ %ld ]",
                 (unsigned long)Array->Capacity);
    return 0;
}

size_t Array_Length(Array_t *Array) {

    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "Error, NULL Array_t* provided.");
        return 0;
    }

    return Array->Length;
}

size_t Array_Capacity(Array_t *Array) {

    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "Error, NULL Array_t* provided.");
        return 0;
    }

    return Array->Capacity;
}
