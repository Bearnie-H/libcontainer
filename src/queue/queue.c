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
#include "include/queue.h"

Queue_t *Queue_Create(size_t ValueSize, ReleaseFunc_t *ReleaseFunc) {

    Queue_t *Queue = NULL;

    if ( 0 == ValueSize ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: ValueSize of 0 indicated reference-type items.");
#endif
    }

    if ( NULL == ReleaseFunc ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: NULL ReleaseFunc* provided, defaulting to free().");
#endif
        ReleaseFunc = free;
    }

    Queue = (Queue_t *)calloc(1, sizeof(Queue_t));
    if ( NULL == Queue ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Queue_t.");
#endif
        return NULL;
    }

    Queue->Items = List_Create();
    if ( NULL == Queue->Items ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to allocate internal memory to hold items in Queue_t.");
#endif
        free(Queue);
        return NULL;
    }

    Queue->ItemSize    = ValueSize;
    Queue->ReleaseFunc = ReleaseFunc;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully created and initialized Queue_t.");
#endif
    return Queue;
}

size_t Queue_Length(Queue_t *Queue) {

    if ( NULL == Queue ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
#endif
        return 0;
    }

    return List_Length(Queue->Items);
}

bool Queue_IsEmpty(Queue_t *Queue) {

    if ( NULL == Queue ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Warning: NULL Queue* provided.");
#endif
        return true;
    }

    return (0 == List_Length(Queue->Items));
}

int Queue_Push(Queue_t *Queue, void *Value, size_t ValueSize) {

    if ( NULL == Queue ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
#endif
        return 1;
    }

    ValueSize = ((ValueSize > Queue->ItemSize) ? (ValueSize) : (Queue->ItemSize));

    if ( 0 == ValueSize ) {
        return List_RefAppend(Queue->Items, Value, Queue->ReleaseFunc);
    } else {
        return List_Append(Queue->Items, Value, ValueSize);
    }
}

void *Queue_Peek(Queue_t *Queue) {

    if ( NULL == Queue ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
#endif
        return NULL;
    }

    return List_GetElement(Queue->Items, 0);
}

void *Queue_Pop(Queue_t *Queue) {

    if ( NULL == Queue ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
#endif
        return NULL;
    }

    return List_PopFront(Queue->Items);
}

int Queue_DoCallback(Queue_t *Queue, CallbackFunc_t *Callback) {

    if ( NULL == Queue ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
#endif
        return -1;
    }

    return List_DoCallback(Queue->Items, Callback);
}

int Queue_DoCallbackArg(Queue_t *Queue, CallbackArgFunc_t *Callback, void *Args) {

    if ( NULL == Queue ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
#endif
        return -1;
    }

    return List_DoCallbackArg(Queue->Items, Callback, Args);
}

int Queue_Clear(Queue_t *Queue) {

    if ( NULL == Queue ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
#endif
        return 1;
    }

    return List_Clear(Queue->Items);
}

void Queue_Release(Queue_t *Queue) {

    if ( NULL == Queue ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: NULL Queue* provided, nothing to release.");
#endif
        return;
    }

    List_Release(Queue->Items);

    ZERO_CONTAINER(Queue, Queue_t);
    free(Queue);

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully released Queue_t.");
#endif
    return;
}
