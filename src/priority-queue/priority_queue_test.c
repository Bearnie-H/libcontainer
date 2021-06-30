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

#include <stdio.h>
#include <stdlib.h>

#include "../logging/logging.h"
#include "include/priority_queue.h"

static CallbackFunc_t    Test_CallbackFunc_Print;
static CallbackArgFunc_t Test_CallbackArgFunc_Print;

static int Test_CallbackFunc_Print(void *Value) {

    Priority_Queue_Item_t *Item = NULL;

    if ( NULL == Value ) {
        DEBUG_PRINTF("%s", "Error: NULL Value* provided.");
        return 1;
    }

    Item = (Priority_Queue_Item_t *)Value;

    DEBUG_PRINTF("Priority Queue item: [ Priority = %d, Value = \"%s\" ].", Item->Priority,
                 (char *)(Item->Value));

    return 0;
}

static int Test_CallbackArgFunc_Print(void *Value, void *Args) {

    Priority_Queue_Item_t *Item = NULL;

    if ( NULL == Value ) {
        DEBUG_PRINTF("%s", "Error: NULL Value* provided.");
        return 1;
    }

    Item = (Priority_Queue_Item_t *)Value;

    if ( NULL == Args ) {
        DEBUG_PRINTF("Priority Queue item: [ Priority = %d, Value = \"%s\", Args = NULL ].",
                     Item->Priority, (char *)(Item->Value));
    } else {
        DEBUG_PRINTF("Priority Queue item: [ Priority = %d, Value = \"%s\", Args = Non-NULL ].",
                     Item->Priority, (char *)(Item->Value));
    }

    return 0;
}

int Test_priority_queue(void) {

    int FailedTests = 0;

    FailedTests += Test_Priority_Queue_Push();
    FailedTests += Test_Priority_Queue_Peek();
    FailedTests += Test_Priority_Queue_Pop();
    FailedTests += Test_Priority_Queue_Remove();
    FailedTests += Test_Priority_Queue_Next();
    FailedTests += Test_Priority_Queue_DoCallback();
    FailedTests += Test_Priority_Queue_DoCallbackArg();
    FailedTests += Test_Priority_Queue_Clear();

    return FailedTests;
}

Priority_Queue_t *Testing_initialize_queue(void) {

    Priority_Queue_t *Queue = NULL;
    int               Count = 32, i = 0;
    char              Value[32] = {0x00};

    Queue = Priority_Queue_Create(true);
    if ( NULL == Queue ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Priority_Queue_t for testing.");
        return NULL;
    }

    if ( !Priority_Queue_IsEmpty(Queue) ) {
        TEST_PRINTF("%s", "Test Failure - Newly created Priority_Queue_t reporting non-empty.");
        Priority_Queue_Release(Queue);
        return NULL;
    }

    for ( i = 0; i < Count; i++ ) {

        snprintf(Value, sizeof(Value), "Test Queue Value: %d", i);

        if ( Priority_Queue_Length(Queue) != (size_t)(i) ) {
            TEST_PRINTF("Test Failure - Queue Length (%lu) not equal to expectation (%d).",
                        (unsigned long)Priority_Queue_Length(Queue), i);
            Priority_Queue_Release(Queue);
            return NULL;
        }

        if ( 0 != Priority_Queue_Push(Queue, (i % 4), Value, strlen(Value), NULL) ) {
            TEST_PRINTF("Test Failure - Failed to push [ %d (\"%s\") ] to Priority_Queue_t.", i,
                        Value);
            Priority_Queue_Release(Queue);
            return NULL;
        }

        if ( Priority_Queue_IsEmpty(Queue) ) {
            TEST_PRINTF("%s",
                        "Test Failure - Priority_Queue_t reporting empty after successful Push().");
            Priority_Queue_Release(Queue);
            return NULL;
        }
    }

    return Queue;
}

int Test_Priority_Queue_Push(void) {

    Priority_Queue_t *Queue = NULL;

    Queue = Testing_initialize_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    Priority_Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Priority_Queue_Peek(void) {

    Priority_Queue_t *    Queue      = NULL;
    Priority_Queue_Item_t PeekedItem = {0, NULL};

    Queue = Testing_initialize_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    PeekedItem = Priority_Queue_Peek(Queue);
    if ( NULL == PeekedItem.Value ) {
        TEST_PRINTF("%s", "Test Failure - Failed to Peek() item at front of Priority_Queue_t.");
        Priority_Queue_Release(Queue);
        TEST_FAILURE;
    }
    if ( PeekedItem.Priority != 3 ) {
        TEST_PRINTF(
            "Test Failure - Peeked item does not have highest expected priority. (%d) vs. (%d).",
            PeekedItem.Priority, 3);
        Priority_Queue_Release(Queue);
        TEST_FAILURE;
    }
    Priority_Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Priority_Queue_Pop(void) {

    Priority_Queue_t *    Queue        = NULL;
    Priority_Queue_Item_t PoppedItem   = {0, NULL};
    int                   LastPriority = -1;

    Queue = Testing_initialize_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    while ( 0 != Priority_Queue_Length(Queue) ) {

        PoppedItem = Priority_Queue_Pop(Queue);
        if ( NULL == PoppedItem.Value ) {
            TEST_PRINTF("%s", "Test Failure - Failed to Pop() item from Priority_Queue_t.");
            Priority_Queue_Release(Queue);
            TEST_FAILURE;
        }

        if ( -1 == LastPriority ) {
            LastPriority = PoppedItem.Priority;
        } else if ( LastPriority < PoppedItem.Priority ) {
            TEST_PRINTF("Test Failure - Most recently popped item has higher priority (%d) than "
                        "previous item (%d).",
                        LastPriority, PoppedItem.Priority);
            Priority_Queue_Release(Queue);
            free(PoppedItem.Value);
            TEST_FAILURE;
        }
        free(PoppedItem.Value);
        LastPriority = PoppedItem.Priority;
    }

    Priority_Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Priority_Queue_Remove(void) {

    Priority_Queue_t *Queue = NULL;

    Queue = Testing_initialize_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    while ( 0 != Priority_Queue_Length(Queue) ) {
        if ( 0 != Priority_Queue_Remove(Queue) ) {
            TEST_PRINTF("%s",
                        "Test Failure - Failed to Remove() front item from Priority_Queue_t.");
            Priority_Queue_Release(Queue);
            TEST_FAILURE;
        }
    }

    Priority_Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Priority_Queue_Next(void) {

    Priority_Queue_t *    Queue = NULL;
    Priority_Queue_Item_t Item  = {0, NULL};
    size_t                Count = 0;

    Queue = Testing_initialize_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    PRIORITY_QUEUE_FOREACH(Queue, Item) {
        DEBUG_PRINTF("Priority_Queue_t iterator returned: Priority = %d, Value = \"%s\".",
                     Item.Priority, Item.Value);
        Count++;
    }

    if ( Count != Priority_Queue_Length(Queue) ) {
        TEST_PRINTF("Test Failure - Priority_Queue_t iterator only passed [ %lu/%lu ] items.",
                    (unsigned long)Count, (unsigned long)Priority_Queue_Length(Queue));
        Priority_Queue_Release(Queue);
        TEST_FAILURE;
    }

    Priority_Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Priority_Queue_DoCallback(void) {

    Priority_Queue_t *Queue          = NULL;
    int               CallbackRetVal = 0;

    Queue = Testing_initialize_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    CallbackRetVal = Priority_Queue_DoCallback(Queue, Test_CallbackFunc_Print);
    if ( 0 > CallbackRetVal ) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Callback() on Priority_Queue_t.");
        Priority_Queue_Release(Queue);
        TEST_FAILURE;
    } else if ( 0 < CallbackRetVal ) {
        TEST_PRINTF("Test Failure - [ %d/%lu ] Callback functions returned non-zero.",
                    CallbackRetVal, (unsigned long)Priority_Queue_Length(Queue));
        Priority_Queue_Release(Queue);
        TEST_FAILURE;
    }

    Priority_Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Priority_Queue_DoCallbackArg(void) {

    Priority_Queue_t *Queue          = NULL;
    int               CallbackRetVal = 0;

    Queue = Testing_initialize_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    CallbackRetVal = Priority_Queue_DoCallbackArg(Queue, Test_CallbackArgFunc_Print, NULL);
    if ( 0 > CallbackRetVal ) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Callback() on Priority_Queue_t.");
        Priority_Queue_Release(Queue);
        TEST_FAILURE;
    } else if ( 0 < CallbackRetVal ) {
        TEST_PRINTF("Test Failure - [ %d/%lu ] Callback functions returned non-zero.",
                    CallbackRetVal, (unsigned long)Priority_Queue_Length(Queue));
        Priority_Queue_Release(Queue);
        TEST_FAILURE;
    }

    Priority_Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Priority_Queue_Clear(void) {

    Priority_Queue_t *Queue = NULL;

    Queue = Testing_initialize_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    if ( 0 != Priority_Queue_Clear(Queue) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Clear() on Priority_Queue_t.");
        Priority_Queue_Release(Queue);
        TEST_FAILURE;
    }

    if ( !Priority_Queue_IsEmpty(Queue) ) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Priority_Queue_t reporting non-empty following successful Clear().");
        Priority_Queue_Release(Queue);
        TEST_FAILURE;
    }

    Priority_Queue_Release(Queue);
    TEST_SUCCESSFUL;
}
