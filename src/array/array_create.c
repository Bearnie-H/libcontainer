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

Array_t *Array_Create(size_t StartingCapacity, size_t ElementSize) {

    Array_t *Array    = NULL;
    size_t   Capacity = 1;

    if ( 0 == ElementSize ) {
        DEBUG_PRINTF("Error, invalid ElementSize [ %ld ].", (unsigned long)ElementSize);
        return NULL;
    }

    if ( 0 == StartingCapacity ) {
        StartingCapacity = LIBCONTAINER_ARRAY_DEFAULT_CAPACITY;
    }

    Array = (Array_t *)calloc(1, sizeof(Array_t));
    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "Error, failed to allocate memory for Array_t.");
        return NULL;
    }

    /*
        Ensure the capacity is aligned to a power of 2 boundary.
    */
    while ( Capacity <= StartingCapacity ) { Capacity <<= 1; }

    Array->Contents.ContentBytes = (uint8_t *)calloc(Capacity, ElementSize);
    if ( NULL == Array->Contents.ContentBytes ) {
        DEBUG_PRINTF("%s", "Error, failed to allocate memory for Array_t->Contents.");
        free(Array);
        return NULL;
    }

    Array->Capacity    = Capacity;
    Array->ElementSize = ElementSize;
    Array->Length      = 0;
    Array->ReleaseFunc = NULL;
    Array->Iterator    = NULL;

    DEBUG_PRINTF("%s", "Successfully created Array_t*.");
    return Array;
}

Array_t *Array_RefCreate(size_t StartingCapacity, ReleaseFunc_t *ReleaseFunc) {

    Array_t *Array    = NULL;
    size_t   Capacity = 1;

    if ( NULL == ReleaseFunc ) {
        DEBUG_PRINTF("%s", "NULL ReleaseFunc provided, defaulting to free().");
        ReleaseFunc = free;
    }

    if ( 0 == StartingCapacity ) {
        StartingCapacity = LIBCONTAINER_ARRAY_DEFAULT_CAPACITY;
    }

    Array = (Array_t *)calloc(1, sizeof(Array_t));
    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "Error, failed to allocate memory for Array_t.");
        return NULL;
    }

    /*
        Ensure the capacity is aligned to a power of 2 boundary.
    */
    while ( Capacity <= StartingCapacity ) { Capacity <<= 1; }

    Array->Contents.ContentRefs = (void **)calloc(Capacity, sizeof(void *));
    if ( NULL == Array->Contents.ContentRefs ) {
        DEBUG_PRINTF("%s", "Error, failed to allocate memory for Array_t->Contents.");
        free(Array);
        return NULL;
    }

    Array->Capacity    = Capacity;
    Array->ElementSize = 0;
    Array->Length      = 0;
    Array->ReleaseFunc = ReleaseFunc;
    Array->Iterator    = NULL;

    DEBUG_PRINTF("%s", "Successfully created Array_t*.");
    return Array;
}

int Array_Clear(Array_t *Array) {

    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "NULL Array*, nothing to clear.");
        return 0;
    }

    if ( 0 == Array_Length(Array) ) {
        DEBUG_PRINTF("%s", "Empty Array*, nothing to clear.");
        return 0;
    }

    if ( 0 != Array_RemoveN(Array, 0, Array_Length(Array)) ) {
        DEBUG_PRINTF("%s", "Error: Failed to remove all entries from Array.");
        return 1;
    }

    DEBUG_PRINTF("%s", "Successfully cleared Array_t contents.");
    return 0;
}

void Array_Release(Array_t *Array) {

    size_t Index = 0;

    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "NULL Array_t* provided, nothing to release.");
        return;
    }

    if ( NULL != Array->Contents.ContentBytes ) {

        /*
            If the array holds references which maintain their own memory,
            iterate over the array and release them all with the provided
            ReleaseFunc.
        */
        if ( (0 == Array->ElementSize) && (NULL != Array->ReleaseFunc) ) {
            DEBUG_PRINTF("%s", "Array_t is array of references, releasing each "
                               "element with the provided ReleaseFunc.");
            for ( Index = 0; Index < Array->Length; Index++ ) {
                Array->ReleaseFunc(Array->Contents.ContentRefs[Index]);
            }
        }

        /* Finally, release the memory known to be held by the array. */
        free(Array->Contents.ContentRefs);

    } else {
        DEBUG_PRINTF("%s", "Array_t has NULL contents pointer, no contents to release.");
    }

    Iterator_Invalidate(&(Array->Iterator));

    ZERO_CONTAINER(Array, Array_t);
    free(Array);

    DEBUG_PRINTF("%s", "Successfully released Array_t*.");
    return;
}
