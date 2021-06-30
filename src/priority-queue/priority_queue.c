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
#include "include/priority_queue.h"

Priority_Queue_t *Priority_Queue_Create(bool Ascending) {

    Priority_Queue_t *Queue = NULL;

    Queue = (Priority_Queue_t *)calloc(1, sizeof(Priority_Queue_t));
    if ( NULL == Queue ) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Priority_Queue_t.");
        return NULL;
    }

    if ( Ascending ) {
        Queue->Items = Binary_Heap_Create(CompareFunc_Int_Ascending, NULL);
    } else {
        Queue->Items = Binary_Heap_Create(CompareFunc_Int_Descending, NULL);
    }

    if ( NULL == Queue->Items ) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate internal memory to hold Queued items.");
        free(Queue);
        return NULL;
    }

    DEBUG_PRINTF("%s", "Successfully created Priority_Queue_t.");
    return Queue;
}

size_t Priority_Queue_Length(Priority_Queue_t *Queue) {

    if ( NULL == Queue ) {
        DEBUG_PRINTF("%s", "Warning: NULL Queue* provided.");
        return 0;
    }

    return Binary_Heap_Length(Queue->Items);
}

bool Priority_Queue_IsEmpty(Priority_Queue_t *Queue) {

    if ( NULL == Queue ) {
        DEBUG_PRINTF("%s", "Warning: NULL Queue* provided.");
        return true;
    }

    return Binary_Heap_IsEmpty(Queue->Items);
}

Priority_Queue_Item_t Priority_Queue_Peek(Priority_Queue_t *Queue) {

    Priority_Queue_Item_t      Item         = {0, NULL};
    Binary_Heap_KeyValuePair_t KeyValuePair = {NULL, NULL};

    if ( NULL == Queue ) {
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
        return Item;
    }

    KeyValuePair = Binary_Heap_Peek(Queue->Items);
    if ( NULL != KeyValuePair.Key ) {
        Item.Priority = *(int *)KeyValuePair.Key;
    }

    Item.Value = KeyValuePair.Value;

    DEBUG_PRINTF("%s", "Successfully Peek()'d top value from Priority_Queue_t.");
    return Item;
}

Priority_Queue_Item_t Priority_Queue_Pop(Priority_Queue_t *Queue) {

    Priority_Queue_Item_t      Item         = {0, NULL};
    Binary_Heap_KeyValuePair_t KeyValuePair = {NULL, NULL};

    if ( NULL == Queue ) {
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
        return Item;
    }

    KeyValuePair = Binary_Heap_Pop(Queue->Items);
    if ( NULL != KeyValuePair.Key ) {
        Item.Priority = *(int *)KeyValuePair.Key;
        free(KeyValuePair.Key);
    }

    Item.Value = KeyValuePair.Value;

    DEBUG_PRINTF("%s", "Successfully Pop()'d top item from Priority_Queue_t.");
    return Item;
}

int Priority_Queue_Remove(Priority_Queue_t *Queue) {

    if ( NULL == Queue ) {
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
        return 1;
    }

    return Binary_Heap_Remove(Queue->Items);
}

int Priority_Queue_Push(Priority_Queue_t *Queue, int Priority, void *Value, size_t ValueSize,
                        ReleaseFunc_t *ReleaseFunc) {

    if ( NULL == Queue ) {
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
        return 1;
    }

    return Binary_Heap_Push(Queue->Items, &(Priority), sizeof(Priority), Value, ValueSize,
                            ReleaseFunc);
}

Priority_Queue_Item_t Priority_Queue_Next(Priority_Queue_t *Queue) {

    Priority_Queue_Item_t      Item         = {0, NULL};
    Binary_Heap_KeyValuePair_t KeyValuePair = {NULL, NULL};

    if ( NULL == Queue ) {
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
        return Item;
    }

    KeyValuePair = Binary_Heap_Next(Queue->Items);
    if ( NULL != KeyValuePair.Key ) {
        Item.Priority = *(int *)KeyValuePair.Key;
    }
    Item.Value = KeyValuePair.Value;

    DEBUG_PRINTF("%s", "Successfully retrieved Next value from Priority_Queue_t.");
    return Item;
}

int Priority_Queue_DoCallback(Priority_Queue_t *Queue, CallbackFunc_t *Callback) {

    Priority_Queue_Item_t Item   = {0, NULL};
    int                   RetVal = 0;

    if ( NULL == Queue ) {
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
        return -1;
    }

    PRIORITY_QUEUE_FOREACH(Queue, Item) {
        if ( 0 != Callback(&Item) ) {
            DEBUG_PRINTF("%s", "Warning: Callback() function returned non-zero.");
            RetVal += 1;
        }
    }

    return RetVal;
}

int Priority_Queue_DoCallbackArg(Priority_Queue_t *Queue, CallbackArgFunc_t *Callback, void *Args) {

    Priority_Queue_Item_t Item   = {0, NULL};
    int                   RetVal = 0;

    if ( NULL == Queue ) {
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
        return -1;
    }

    PRIORITY_QUEUE_FOREACH(Queue, Item) {
        if ( 0 != Callback(&Item, Args) ) {
            DEBUG_PRINTF("%s", "Warning: Callback() function returned non-zero.");
            RetVal += 1;
        }
    }

    return RetVal;
}

int Priority_Queue_Clear(Priority_Queue_t *Queue) {

    if ( NULL == Queue ) {
        DEBUG_PRINTF("%s", "Error: NULL Queue* provided.");
        return 1;
    }

    return Binary_Heap_Clear(Queue->Items);
}

void Priority_Queue_Release(Priority_Queue_t *Queue) {

    if ( NULL == Queue ) {
        DEBUG_PRINTF("%s", "Note: NULL Queue* provided, nothing to release.");
        return;
    }

    Binary_Heap_Release(Queue->Items);

    ZERO_CONTAINER(Queue, Priority_Queue_t);
    free(Queue);

    DEBUG_PRINTF("%s", "Successfully released Priority_Queue_t and all held contents.");
    return;
}
