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
#include "include/stack.h"

Stack_t *Stack_Create(size_t ValueSize, ReleaseFunc_t *ReleaseFunc) {

    Stack_t *Stack = NULL;

    if ( 0 == ValueSize ) {
        DEBUG_PRINTF("%s", "Note: ValueSize of 0 indicates Reference-Type items.");
    }

    if ( NULL == ReleaseFunc ) {
        DEBUG_PRINTF("%s", "Note: NULL ReleaseFunc* provided, defaulting to free().");
        ReleaseFunc = free;
    }

    Stack = (Stack_t *)calloc(1, sizeof(Stack_t));
    if ( NULL == Stack ) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate new Stack_t.");
        return NULL;
    }

    Stack->Items = List_Create();
    if ( NULL == Stack->Items ) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate internal memory to hold items in Stack_t.");
        free(Stack);
        return NULL;
    }

    Stack->ItemSize    = ValueSize;
    Stack->ReleaseFunc = ReleaseFunc;

    return Stack;
}

size_t Stack_Length(Stack_t *Stack) {

    if ( NULL == Stack ) {
        DEBUG_PRINTF("%s", "Warning: NULL Stack* provided, no length to report.");
        return 0;
    }

    return List_Length(Stack->Items);
}

bool Stack_IsEmpty(Stack_t *Stack) {

    if ( NULL == Stack ) {
        DEBUG_PRINTF("%s", "Note: NULL Stack* provided, treating as empty.");
        return true;
    }

    return (0 == List_Length(Stack->Items));
}

int Stack_Push(Stack_t *Stack, void *Value, size_t ValueSize) {

    if ( NULL == Stack ) {
        DEBUG_PRINTF("%s", "NULL Stack* provided.");
        return 1;
    }

    ValueSize = ((ValueSize > Stack->ItemSize) ? (ValueSize) : (Stack->ItemSize));

    if ( 0 == ValueSize ) {
        return List_RefPrepend(Stack->Items, Value, Stack->ReleaseFunc);
    } else {
        return List_Prepend(Stack->Items, Value, ValueSize);
    }
}

void *Stack_Peek(Stack_t *Stack) {

    if ( NULL == Stack ) {
        DEBUG_PRINTF("%s", "NULL Stack* provided.");
        return NULL;
    }

    return List_GetElement(Stack->Items, 0);
}

void *Stack_Pop(Stack_t *Stack) {

    if ( NULL == Stack ) {
        DEBUG_PRINTF("%s", "NULL Stack* provided.");
        return NULL;
    }

    return List_PopFront(Stack->Items);
}

int Stack_DoCallback(Stack_t *Stack, CallbackFunc_t *Callback) {

    if ( NULL == Stack ) {
        DEBUG_PRINTF("%s", "Error: NULL Stack* provided.");
        return 1;
    }

    return List_DoCallback(Stack->Items, Callback);
}

int Stack_DoCallbackArg(Stack_t *Stack, CallbackArgFunc_t *Callback, void *Args) {

    if ( NULL == Stack ) {
        DEBUG_PRINTF("%s", "Error: NULL Stack* provided.");
        return 1;
    }

    return List_DoCallbackArg(Stack->Items, Callback, Args);
}

int Stack_Clear(Stack_t *Stack) {

    if ( NULL == Stack ) {
        DEBUG_PRINTF("%s", "NULL Stack* provided.");
        return 1;
    }

    return List_Clear(Stack->Items);
}

void Stack_Release(Stack_t *Stack) {

    if ( NULL == Stack ) {
        DEBUG_PRINTF("%s", "NULL Stack* provided.");
        return;
    }

    List_Release(Stack->Items);
    ZERO_CONTAINER(Stack, Stack_t);
    free(Stack);

    return;
}
