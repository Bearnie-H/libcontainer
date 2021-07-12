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
#include <sys/types.h>

#include "../logging/logging.h"
#include "include/set.h"

Set_t *Set_Create(CompareFunc_t *CompareFunc, size_t ValueSize, ReleaseFunc_t *ReleaseFunc) {

    Set_t *Set = NULL;

    Set = (Set_t *)calloc(1, sizeof(Set_t));
    if ( NULL == Set ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Set_t.");
#endif
        return NULL;
    }

    Set->Contents = Binary_Tree_Create(CompareFunc, ValueSize, ReleaseFunc, Policy_Error);
    if ( NULL == Set->Contents ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to prepare internal data structure to implement Set_t.");
#endif
        free(Set);
        return NULL;
    }

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully created and initialized Set_t.");
#endif
    return Set;
}

size_t Set_Length(Set_t *Set) {

    if ( NULL == Set ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Warning: NULL Set* provided.");
#endif
        return 0;
    }

    return Binary_Tree_Length(Set->Contents);
}

int Set_Insert(Set_t *Set, void *Value, size_t ValueSize) {

    if ( NULL == Set ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Set* provided.");
#endif
        return 1;
    }

    return Binary_Tree_Insert(Set->Contents, Value, ValueSize, NULL, 0, NULL);
}

bool Set_ValueExists(Set_t *Set, void *Value, size_t ValueSize) {

    if ( NULL == Set ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Set* provided.");
#endif
        return false;
    }

    return Binary_Tree_KeyExists(Set->Contents, Value, ValueSize);
}

void *Set_Next(Set_t *Set) {

    if ( NULL == Set ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Set* provided.");
#endif
        return NULL;
    }

    return Binary_Tree_Next(Set->Contents, Direction_InOrder).Key;
}

int Set_DoCallback(Set_t *Set, CallbackFunc_t *Callback) {

    void *Value  = NULL;
    int   RetVal = 0;

    if ( NULL == Set ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Set* provided.");
#endif
        return -1;
    }

    Iterator_Invalidate(&(Set->Contents->Iterator));

    SET_FOREACH(Set, Value) {
        if ( 0 != Callback(Value) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Warning: Callback function returned non-zero");
#endif
            RetVal += 1;
        }
    }

    return RetVal;
}

int Set_DoCallbackArgs(Set_t *Set, CallbackArgFunc_t *Callback, void *Args) {

    void *Value  = NULL;
    int   RetVal = 0;

    if ( NULL == Set ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Set* provided.");
#endif
        return -1;
    }

    Iterator_Invalidate(&(Set->Contents->Iterator));

    SET_FOREACH(Set, Value) {
        if ( 0 != Callback(Value, Args) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Warning: Callback function returned non-zero");
#endif
            RetVal += 1;
        }
    }

    return RetVal;
}

int Set_Remove(Set_t *Set, void *Value, size_t ValueSize) {

    if ( NULL == Set ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Set* provided.");
#endif
        return 1;
    }

    return Binary_Tree_Remove(Set->Contents, Value, ValueSize);
}

int Set_Clear(Set_t *Set) {

    if ( NULL == Set ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Set* provided.");
#endif
        return 1;
    }

    return Binary_Tree_Clear(Set->Contents);
}

void Set_Release(Set_t *Set) {

    if ( NULL == Set ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: NULL Set* provided.");
#endif
        return;
    }

    Binary_Tree_Release(Set->Contents);

    ZERO_CONTAINER(Set, Set_t);
    free(Set);

    return;
}
