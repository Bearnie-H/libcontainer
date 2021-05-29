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

    Array_t *Array = NULL;

    if (0 == ElementSize) {
        DEBUG_PRINTF("Error, invalid ElementSize [ %ld ].", (unsigned long)ElementSize);
        return NULL;
    }

    if (0 == StartingCapacity) {
        StartingCapacity = LIBCONTAINER_ARRAY_DEFAULT_CAPACITY;
    }

    Array = (Array_t *)malloc(sizeof(Array_t));
    if (NULL == Array) {
        DEBUG_PRINTF("%s", "Error, failed to allocate memory for Array_t.");
        return NULL;
    }

    Array->Contents = (uint8_t *)calloc(StartingCapacity, ElementSize);
    if (NULL == Array->Contents) {
        DEBUG_PRINTF("%s", "Error, failed to allocate memory for Array_t->Contents.");
        free(Array);
        return NULL;
    }

    Array->Capacity = StartingCapacity;
    Array->ElementSize = ElementSize;
    Array->Length = 0;
    Array->IsReference = false;
    Array->ReleaseFunc = NULL;
    Array->IsShadow = false;

    DEBUG_PRINTF("%s", "Successfully created Array_t*.");
    return Array;
}

Array_t *Array_RefCreate(size_t StartingCapacity, size_t ElementSize, ReleaseFunc_t ReleaseFunc) {

    Array_t *Array = NULL;

    if (NULL == ReleaseFunc) {
        DEBUG_PRINTF("%s", "NULL ReleaseFunc provided, defaulting to free().");
        ReleaseFunc = free;
    }

    if (0 == ElementSize) {
        DEBUG_PRINTF("Error, invalid ElementSize [ %ld ].", (unsigned long)ElementSize);
        return NULL;
    }

    if (0 == StartingCapacity) {
        StartingCapacity = LIBCONTAINER_ARRAY_DEFAULT_CAPACITY;
    }

    Array = (Array_t *)malloc(sizeof(Array_t));
    if (NULL == Array) {
        DEBUG_PRINTF("%s", "Error, failed to allocate memory for Array_t.");
        return NULL;
    }

    Array->Contents = (uint8_t *)calloc(StartingCapacity, ElementSize);
    if (NULL == Array->Contents) {
        DEBUG_PRINTF("%s", "Error, failed to allocate memory for Array_t->Contents.");
        free(Array);
        return NULL;
    }

    Array->Capacity = StartingCapacity;
    Array->ElementSize = ElementSize;
    Array->Length = 0;
    Array->IsReference = true;
    Array->ReleaseFunc = ReleaseFunc;
    Array->IsShadow = false;

    DEBUG_PRINTF("%s", "Successfully created Array_t*.");
    return Array;
}

Array_t *Array_Duplicate(Array_t *Source) {

    Array_t *Destination = NULL;

    if (NULL == Source) {
        DEBUG_PRINTF("%s", "Error, NULL Source Array_t* provided.");
        return NULL;
    }

    if (Source->IsReference) {
        Destination = Array_RefCreate(Source->Capacity, Source->ElementSize, Source->ReleaseFunc);
    } else {
        Destination = Array_Create(Source->Capacity, Source->ElementSize);
    }

    if (NULL == Destination) {
        DEBUG_PRINTF("%s", "Error, Failed to create new empty Array_t.");
        return NULL;
    }

    memcpy(Destination->Contents, Source->Contents, Source->Length);

    /*
        The duplicated array is only a shadow copy if the source array holds
        references. If it holds values, these are duplicated to new values in
       the new array.
    */
    Destination->IsShadow = Source->IsReference;
    Destination->Length = Source->Length;

    return Destination;
}

void Array_Release(Array_t *Array) {

    size_t Index = 0;

    if (NULL == Array) {
        DEBUG_PRINTF("%s", "NULL Array_t* provided, nothing to release.");
        return;
    }

    if (NULL != Array->Contents) {

        /*
            If the array holds references which maintain their own memory,
            iterate over the array and release them all with the provided
            ReleaseFunc.
        */
        if (Array->IsReference && !Array->IsShadow) {
            DEBUG_PRINTF("%s", "Array_t is array of references, releasing each "
                               "element with the provided ReleaseFunc.");
            for (Index = 0; Index < Array->Length; Index++) {
                Array->ReleaseFunc(*(uint8_t **)&(Array->Contents[Index * Array->ElementSize]));
            }
        }

        /* Finally, release the memory known to be held by the array. */
        free(Array->Contents);
        Array->Contents = NULL;
        Array->ReleaseFunc = NULL;

    } else {
        DEBUG_PRINTF("%s", "Array_t has NULL contents pointer, no contents to release.");
    }

    Array->Capacity = 0;
    Array->Length = 0;
    Array->IsReference = false;
    Array->IsShadow = false;

    free(Array);
    DEBUG_PRINTF("%s", "Successfully released Array_t*.");
    return;
}
