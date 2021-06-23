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

#include <stdlib.h>

#include "../logging/logging.h"
#include "include/array.h"

int Iterator_Initialize_Array(Array_t *Array) {

    Iterator_t *Iterator = NULL;

    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "Error: NULL Array* provided.");
        return 1;
    }

    Iterator_Invalidate(&(Array->Iterator));

    Iterator = (Iterator_t *)calloc(1, sizeof(Iterator_t));
    if ( NULL == Iterator ) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory to initialize Iterator.");
        return 1;
    }

    Iterator->Context = (ssize_t *)calloc(1, sizeof(ssize_t));
    if ( NULL == Iterator->Context ) {
        DEBUG_PRINTF("%s", "Error: Failed to prepare Iterator context.");
        free(Iterator);
        return 1;
    }

    Iterator->ReleaseContext = (ReleaseFunc_t *)free;
    Iterator->Type           = Iterator_Array;

    DEBUG_PRINTF("%s", "Successfully prepared Array Iterator.");
    Array->Iterator = Iterator;
    return 0;
}

void *Array_Next(Array_t *Array) {

    ssize_t *Index = 0;

    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "Error: NULL Array* provided.");
        return NULL;
    }

    if ( (NULL == Array->Iterator) || (Array->Iterator->Type == Iterator_UNKNOWN) ) {
        if ( 0 != Iterator_Initialize_Array(Array) ) {
            DEBUG_PRINTF("%s",
                         "Error: Failed to initialize Iterator to begin Iteration over Array.");
            return NULL;
        }
        *((ssize_t *)(Array->Iterator->Context)) = 0;
    }

    Index = (ssize_t *)(Array->Iterator->Context);

    if ( (size_t)(*Index) >= Array_Length(Array) ) {
        DEBUG_PRINTF("%s", "Note: Iteration has reached the end of the Array_t.");
        Iterator_Invalidate(&(Array->Iterator));
        return NULL;
    }

    return Array_GetElement(Array, (size_t)(*Index)++);
}

void *Array_Previous(Array_t *Array) {

    ssize_t *Index = 0;

    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "Error: NULL Array* provided.");
        return NULL;
    }

    if ( (NULL == Array->Iterator) || (Array->Iterator->Type == Iterator_UNKNOWN) ) {
        if ( 0 != Iterator_Initialize_Array(Array) ) {
            DEBUG_PRINTF("%s",
                         "Error: Failed to initialize Iterator to begin Iteration over Array.");
            return NULL;
        }
        *((ssize_t *)(Array->Iterator->Context)) = (ssize_t)Array_Length(Array) - 1;
    }

    Index = (ssize_t *)(Array->Iterator->Context);

    if ( *Index < 0 ) {
        DEBUG_PRINTF("%s", "Note: Iteration has reached the end of the Array_t.");
        Iterator_Invalidate(&(Array->Iterator));
        return NULL;
    }

    return Array_GetElement(Array, (size_t)(*Index)--);
}
