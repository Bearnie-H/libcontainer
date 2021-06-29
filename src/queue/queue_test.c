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
#include "include/queue.h"

static CallbackFunc_t    CallbackFunc_Int;
static CallbackArgFunc_t CallbackArgFunc_Int;

static int CallbackFunc_Int(void *Value) {

    if ( NULL == Value ) {
        DEBUG_PRINTF("%s", "Error: NULL Value* provided.");
        return 1;
    }

    DEBUG_PRINTF("Queue Value: [ %d ].", *(int *)Value);

    return 0;
}

static int CallbackArgFunc_Int(void *Value, void *Args) {

    if ( NULL == Value ) {
        DEBUG_PRINTF("%s", "Error: NULL Value* provided.");
        return 1;
    }

    if ( NULL == Args ) {
        DEBUG_PRINTF("Queue Value: [ %d ] (and NULL Args).", *(int *)Value);
    } else {
        DEBUG_PRINTF("Queue Value: [ %d ] (and Non-NULL Args).", *(int *)Value);
    }

    return 0;
}

int Test_queue(void) {

    int FailedTests = 0;

    FailedTests += Test_Queue_Create();
    FailedTests += Test_Queue_Length();
    FailedTests += Test_Queue_IsEmpty();
    FailedTests += Test_Queue_Push();
    FailedTests += Test_Queue_Peek();
    FailedTests += Test_Queue_Pop();
    FailedTests += Test_Queue_DoCallback();
    FailedTests += Test_Queue_DoCallbackArg();
    FailedTests += Test_Queue_Clear();

    return FailedTests;
}

static Queue_t *Test_prepare_queue(void) {

    Queue_t *Queue = NULL;
    int      Value = 0, Count = 32, i = 0;

    Queue = Queue_Create(sizeof(int), NULL);
    if ( NULL == Queue ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Queue_t for testing.");
        return NULL;
    }

    for ( i = 0; i < Count; i++ ) {

        Value = i * 2;
        if ( 0 != Queue_Push(Queue, &(Value), 0) ) {
            TEST_PRINTF("Test Failure - Failed to add value [ %d ] to Queue_t.", Value);
            Queue_Release(Queue);
            return NULL;
        }

        if ( Queue_Length(Queue) != (size_t)(i + 1) ) {
            TEST_PRINTF("Test Failure - Queue reported length as [ %lu ] when expected was [ %d ].",
                        (unsigned long)Queue_Length(Queue), i);
            Queue_Release(Queue);
            return NULL;
        }

        if ( Queue_IsEmpty(Queue) ) {
            TEST_PRINTF("Test Failure - Queue reported empty when expected length was [ %d ].", i);
            Queue_Release(Queue);
            return NULL;
        }
    }

    return Queue;
}

int Test_Queue_Create(void) {

    Queue_t *Queue = NULL;

    Queue = Queue_Create(sizeof(int), NULL);
    if ( NULL == Queue ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Queue_t for testing.");
        TEST_FAILURE;
    }

    Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Queue_Length(void) {

    Queue_t *Queue = NULL;

    Queue = Queue_Create(sizeof(int), NULL);
    if ( NULL == Queue ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Queue_t for testing.");
        TEST_FAILURE;
    }

    if ( 0 != Queue_Length(Queue) ) {
        TEST_PRINTF("Test Failure - Newly created Queue_t reporting non-zero length (%lu).",
                    (unsigned long)Queue_Length(Queue));
        Queue_Release(Queue);
        TEST_FAILURE;
    }

    Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Queue_IsEmpty(void) {

    Queue_t *Queue = NULL;

    Queue = Queue_Create(sizeof(int), NULL);
    if ( NULL == Queue ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Queue_t for testing.");
        TEST_FAILURE;
    }

    if ( !Queue_IsEmpty(Queue) ) {
        TEST_PRINTF("%s", "Test Failure - Newly created Queue reporting non-empty.");
        Queue_Release(Queue);
        TEST_FAILURE;
    }

    Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Queue_Push(void) {

    Queue_t *Queue = NULL;

    Queue = Test_prepare_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Queue_Peek(void) {

    Queue_t *Queue     = NULL;
    int *    PeekValue = NULL, ExpectedValue = 0;

    Queue = Test_prepare_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    PeekValue = Queue_Peek(Queue);
    if ( NULL == PeekValue ) {
        TEST_PRINTF("%s", "Test Failure - Failed to Peek() value from front of Queue_t.");
        Queue_Release(Queue);
        TEST_FAILURE;
    }

    if ( *PeekValue != ExpectedValue ) {
        TEST_PRINTF("Test Failure - Peeked Value (%d) not equal to expectation (%d)", *PeekValue,
                    ExpectedValue);
        Queue_Release(Queue);
        TEST_FAILURE;
    }

    Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Queue_Pop(void) {

    Queue_t *Queue       = NULL;
    int *    PoppedValue = NULL, ExpectedValue = 0;

    Queue = Test_prepare_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    while ( 0 != Queue_Length(Queue) ) {

        PoppedValue = Queue_Pop(Queue);
        if ( NULL == PoppedValue ) {
            TEST_PRINTF("%s", "Test Failure - Failed to Pop() value from front of Queue_t.");
            Queue_Release(Queue);
            TEST_FAILURE;
        }

        if ( *PoppedValue != ExpectedValue ) {
            TEST_PRINTF("Test Failure - Popped Value (%d) not equal to expectation (%d)",
                        *PoppedValue, ExpectedValue);
            Queue_Release(Queue);
            free(PoppedValue);
            TEST_FAILURE;
        }

        free(PoppedValue);
        ExpectedValue += 2;
    }

    Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Queue_DoCallback(void) {

    Queue_t *Queue = NULL;

    Queue = Test_prepare_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    if ( 0 != Queue_DoCallback(Queue, CallbackFunc_Int) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Callback() on Queue_t.");
        Queue_Release(Queue);
        TEST_FAILURE;
    }

    Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Queue_DoCallbackArg(void) {

    Queue_t *Queue = NULL;

    Queue = Test_prepare_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    if ( 0 != Queue_DoCallbackArg(Queue, CallbackArgFunc_Int, NULL) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Callback() on Queue_t.");
        Queue_Release(Queue);
        TEST_FAILURE;
    }

    Queue_Release(Queue);
    TEST_SUCCESSFUL;
}

int Test_Queue_Clear(void) {

    Queue_t *Queue = NULL;

    Queue = Test_prepare_queue();
    if ( NULL == Queue ) {
        TEST_FAILURE;
    }

    if ( 0 != Queue_Clear(Queue) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Clear() on Queue_t.");
        Queue_Release(Queue);
        TEST_FAILURE;
    }

    if ( 0 != Queue_Length(Queue) ) {
        TEST_PRINTF(
            "Test Failure - Queue_t is reporting non-zero length of [ %lu ] following Clear().",
            (unsigned long)Queue_Length(Queue));
        Queue_Release(Queue);
        TEST_FAILURE;
    }

    if ( !Queue_IsEmpty(Queue) ) {
        TEST_PRINTF("%s", "Test Failure - Queue_t reporting non-empty following Clear().");
        Queue_Release(Queue);
        TEST_FAILURE;
    }

    Queue_Release(Queue);
    TEST_SUCCESSFUL;
}
