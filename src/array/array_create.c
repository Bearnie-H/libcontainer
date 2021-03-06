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
#ifdef DEBUG
        DEBUG_PRINTF("Error, invalid ElementSize [ %lu ].", (unsigned long)ElementSize);
#endif
        return NULL;
    }

    if ( 0 == StartingCapacity ) {
        StartingCapacity = LIBCONTAINER_ARRAY_DEFAULT_CAPACITY;
    }

    Array = (Array_t *)calloc(1, sizeof(Array_t));
    if ( NULL == Array ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error, failed to allocate memory for Array_t.");
#endif
        return NULL;
    }

    /*
        Ensure the capacity is aligned to a power of 2 boundary.
    */
    while ( Capacity <= StartingCapacity ) { Capacity <<= 1; }

    Array->Contents.ContentBytes = (uint8_t *)calloc(Capacity, ElementSize);
    if ( NULL == Array->Contents.ContentBytes ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error, failed to allocate memory for Array_t->Contents.");
#endif
        free(Array);
        return NULL;
    }

    Array->Capacity    = Capacity;
    Array->ElementSize = ElementSize;
    Array->Length      = 0;
    Array->ReleaseFunc = NULL;
    Array->Iterator    = NULL;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully created Array_t*.");
#endif
    return Array;
}

Array_t *Array_RefCreate(size_t StartingCapacity, ReleaseFunc_t *ReleaseFunc) {

    Array_t *Array    = NULL;
    size_t   Capacity = 1;

    if ( NULL == ReleaseFunc ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "NULL ReleaseFunc provided, defaulting to free().");
#endif
        ReleaseFunc = free;
    }

    if ( 0 == StartingCapacity ) {
        StartingCapacity = LIBCONTAINER_ARRAY_DEFAULT_CAPACITY;
    }

    Array = (Array_t *)calloc(1, sizeof(Array_t));
    if ( NULL == Array ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error, failed to allocate memory for Array_t.");
#endif
        return NULL;
    }

    /*
        Ensure the capacity is aligned to a power of 2 boundary.
    */
    while ( Capacity <= StartingCapacity ) { Capacity <<= 1; }

    Array->Contents.ContentRefs = (void **)calloc(Capacity, sizeof(void *));
    if ( NULL == Array->Contents.ContentRefs ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error, failed to allocate memory for Array_t->Contents.");
#endif
        free(Array);
        return NULL;
    }

    Array->Capacity    = Capacity;
    Array->ElementSize = 0;
    Array->Length      = 0;
    Array->ReleaseFunc = ReleaseFunc;
    Array->Iterator    = NULL;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully created Array_t*.");
#endif
    return Array;
}

int Array_Clear(Array_t *Array) {

    void *ArrayValue = NULL;

    if ( NULL == Array ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "NULL Array*, nothing to clear.");
#endif
        return 0;
    }

    if ( 0 == Array->Length ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: Array_t already empty, nothing to clear.");
#endif
        return 0;
    }

    Iterator_Invalidate(&(Array->Iterator));

    if ( NULL != Array->Contents.ContentBytes ) {

        /*
            If the array holds references which maintain their own memory,
            iterate over the array and release them all with the provided
            ReleaseFunc.
        */
        if ( (0 == Array->ElementSize) && (NULL != Array->ReleaseFunc) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Array_t is array of references, releasing each "
                               "element with the provided ReleaseFunc.");
#endif
            ARRAY_FOREACH(Array, ArrayValue) {
                Array->ReleaseFunc(ArrayValue);
            }
        }

    } else {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Array_t has NULL contents pointer, no contents to release.");
#endif
    }

    Iterator_Invalidate(&(Array->Iterator));

    Array->Length = 0;
#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully cleared Array_t contents.");
#endif
    return 0;
}

void Array_Release(Array_t *Array) {

    if ( NULL == Array ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "NULL Array_t* provided, nothing to release.");
#endif
        return;
    }

    Array_Clear(Array);
    free(Array->Contents.ContentRefs);

    ZERO_CONTAINER(Array, Array_t);
    free(Array);

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully released Array_t*.");
#endif
    return;
}
