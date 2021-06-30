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
#include <string.h>

#include "../../logging/logging.h"
#include "include/binary_heap.h"

__attribute__((unused)) static void
PrettyPrintBinaryHeap(const char *Prefix, const Binary_Heap_t *Heap, size_t Index, bool IsLeft) {

    char NextLeftPrefix[128]  = {0x00};
    char NextRightPrefix[128] = {0x00};

    Binary_Heap_Node_t *Root = Array_GetElement(Heap->Items, Index);
    if ( NULL != Root ) {

        printf("%s", Prefix);

        printf("%s", (IsLeft) ? ("├──") : ("└──"));

        printf("(%d)\n", *(int *)Root->Key);

        snprintf(NextLeftPrefix, sizeof(NextLeftPrefix), "%s%s", Prefix,
                 (IsLeft) ? ("│   ") : ("    "));
        snprintf(NextRightPrefix, sizeof(NextRightPrefix), "%s%s", Prefix,
                 (IsLeft) ? ("│   ") : ("    "));

        PrettyPrintBinaryHeap(NextLeftPrefix, Heap, LEFT_CHILD_INDEX(Index), true);
        PrettyPrintBinaryHeap(NextRightPrefix, Heap, RIGHT_CHILD_INDEX(Index), false);
    }
}

__attribute__((unused)) static int Test_Binary_Heap_DoCallbackFunc(void *Value) {

    if ( NULL == Value ) {
        TEST_PRINTF("%s", "Error: NULL Value* passed to Callback.");
        return 1;
    }

    return 0;
}

__attribute__((unused)) static int Test_Binary_Heap_DoCallbackArgFunc(void *Value, void *Args) {

    if ( NULL == Value ) {
        TEST_PRINTF("%s", "Error: NULL Value* passed to Callback.");
        return 1;
    }

    if ( NULL == Args ) {
        DEBUG_PRINTF("%s", "NULL Args* passed to Callback.");
    }

    return 0;
}

int Test_binary_heap(void) {

    int FailedTests = 0;

    FailedTests += Test_binary_heap_node();

    FailedTests += Test_Binary_Heap_Create();
    FailedTests += Test_Binary_Heap_Length();
    FailedTests += Test_Binary_Heap_IsEmpty();
    FailedTests += Test_Binary_Heap_Push();
    FailedTests += Test_Binary_Heap_Peek();
    FailedTests += Test_Binary_Heap_Pop();
    FailedTests += Test_Binary_Heap_Remove();
    FailedTests += Test_Binary_Heap_Next();
    FailedTests += Test_Binary_Heap_DoCallback();
    FailedTests += Test_Binary_Heap_DoCallbackArg();
    FailedTests += Test_Binary_Heap_Clear();

    return FailedTests;
}

Binary_Heap_t *Testing_initialize_heap(void) {

    Binary_Heap_t *Heap  = NULL;
    int            Count = 32, i = 0;
    char           Value[32] = {0x00};

    Heap = Binary_Heap_Create(CompareFunc_Int_Ascending, NULL);
    if ( NULL == Heap ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Heap_t for testing.");
        return NULL;
    }

    for ( i = 0; i < Count; i++ ) {

        snprintf(Value, sizeof(Value), "Test Heap Value: %d", i);

        if ( Binary_Heap_Length(Heap) != (size_t)(i) ) {
            TEST_PRINTF("Test Failure - Heap Length (%lu) not equal to expectation (%d).",
                        (unsigned long)Binary_Heap_Length(Heap), i);
            Binary_Heap_Release(Heap);
            return NULL;
        }

        if ( 0 != Binary_Heap_Push(Heap, &i, sizeof(i), Value, strlen(Value), NULL) ) {
            TEST_PRINTF("Test Failure - Failed to push [ %d (\"%s\") ] to Binary_Heap_t.", i,
                        Value);
            Binary_Heap_Release(Heap);
            return NULL;
        }

        /*
        PrettyPrintBinaryHeap("", Heap, 0, false);
        printf("\n────────────────────────────────────────────────────────────\n");
        */
    }

    return Heap;
}

int Test_Binary_Heap_Create(void) {

    Binary_Heap_t *Heap = NULL;

    Heap = Binary_Heap_Create(CompareFunc_Int_Ascending, NULL);
    if ( NULL == Heap ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Heap_t for testing.");
        TEST_FAILURE;
    }

    Binary_Heap_Release(Heap);
    TEST_SUCCESSFUL;
}

int Test_Binary_Heap_Length(void) {

    Binary_Heap_t *Heap = NULL;

    Heap = Binary_Heap_Create(CompareFunc_Int_Ascending, NULL);
    if ( NULL == Heap ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Heap_t for testing.");
        TEST_FAILURE;
    }

    if ( 0 != Binary_Heap_Length(Heap) ) {
        TEST_PRINTF("%s", "Test Failure - Newly created Binary_Heap_t reports non-zero length.");
        Binary_Heap_Release(Heap);
        TEST_FAILURE;
    }

    Binary_Heap_Release(Heap);
    TEST_SUCCESSFUL;
}

int Test_Binary_Heap_IsEmpty(void) {

    Binary_Heap_t *Heap = NULL;

    Heap = Binary_Heap_Create(CompareFunc_Int_Ascending, NULL);
    if ( NULL == Heap ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Heap_t for testing.");
        TEST_FAILURE;
    }

    if ( !Binary_Heap_IsEmpty(Heap) ) {
        TEST_PRINTF("%s", "Test Failure - Newly created Binary_Heap_t reports non-empty.");
        Binary_Heap_Release(Heap);
        TEST_FAILURE;
    }

    Binary_Heap_Release(Heap);
    TEST_SUCCESSFUL;
}

int Test_Binary_Heap_Push(void) {

    Binary_Heap_t *Heap = NULL;

    Heap = Testing_initialize_heap();
    if ( NULL == Heap ) {
        TEST_FAILURE;
    }

    Binary_Heap_Release(Heap);
    TEST_SUCCESSFUL;
}

int Test_Binary_Heap_Peek(void) {

    Binary_Heap_t *            Heap            = NULL;
    Binary_Heap_KeyValuePair_t KeyValuePair    = {NULL, NULL};
    int                        ExpectedKey     = 31;
    char                       ExpectedValue[] = "Test Heap Value: 31";

    Heap = Testing_initialize_heap();
    if ( NULL == Heap ) {
        TEST_FAILURE;
    }

    KeyValuePair = Binary_Heap_Peek(Heap);
    if ( (NULL == KeyValuePair.Key) || (NULL == KeyValuePair.Value) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to Peek root of Binary_Heap_t.");
        Binary_Heap_Release(Heap);
        TEST_FAILURE;
    }

    if ( *(int *)(KeyValuePair.Key) != ExpectedKey ) {
        TEST_PRINTF("Test Failure - Peeked Key (%d) not equal to expectation (%d).",
                    *(int *)(KeyValuePair.Key), ExpectedKey);
        Binary_Heap_Release(Heap);
        TEST_FAILURE;
    }

    if ( 0 != memcmp(ExpectedValue, KeyValuePair.Value, strlen(ExpectedValue)) ) {
        TEST_PRINTF("Test Failure - Peeked Value (%s) not equal to expectation (%s).",
                    (char *)(KeyValuePair.Value), ExpectedValue);
        Binary_Heap_Release(Heap);
        TEST_FAILURE;
    }

    Binary_Heap_Release(Heap);
    TEST_SUCCESSFUL;
}

int Test_Binary_Heap_Pop(void) {

    Binary_Heap_t *            Heap              = NULL;
    Binary_Heap_KeyValuePair_t KeyValuePair      = {NULL, NULL};
    int                        ExpectedKey       = 31;
    char                       ExpectedValue[32] = {0x00};

    Heap = Testing_initialize_heap();
    if ( NULL == Heap ) {
        TEST_FAILURE;
    }

    while ( 0 < Binary_Heap_Length(Heap) ) {

        snprintf(ExpectedValue, sizeof(ExpectedValue), "Test Heap Value: %d", ExpectedKey);

        KeyValuePair = Binary_Heap_Pop(Heap);
        if ( (NULL == KeyValuePair.Key) || (NULL == KeyValuePair.Value) ) {
            TEST_PRINTF("%s", "Test Failure - Failed to Pop root of Binary_Heap_t.");
            Binary_Heap_Release(Heap);
            exit(1);
            TEST_FAILURE;
        }

        if ( *(int *)(KeyValuePair.Key) != ExpectedKey ) {
            TEST_PRINTF("Test Failure - Peeked Key (%d) not equal to expectation (%d).",
                        *(int *)(KeyValuePair.Key), ExpectedKey);
            Binary_Heap_Release(Heap);
            free(KeyValuePair.Key);
            free(KeyValuePair.Value);
            TEST_FAILURE;
        }

        if ( 0 != memcmp(ExpectedValue, KeyValuePair.Value, strlen(ExpectedValue)) ) {
            TEST_PRINTF("Test Failure - Peeked Value (%s) not equal to expectation (%s).",
                        (char *)(KeyValuePair.Value), ExpectedValue);
            Binary_Heap_Release(Heap);
            free(KeyValuePair.Key);
            free(KeyValuePair.Value);
            TEST_FAILURE;
        }

        free(KeyValuePair.Key);
        free(KeyValuePair.Value);
        ExpectedKey -= 1;
    }

    Binary_Heap_Release(Heap);
    TEST_SUCCESSFUL;
}

int Test_Binary_Heap_Remove(void) {

    Binary_Heap_t *Heap = NULL;

    Heap = Testing_initialize_heap();
    if ( NULL == Heap ) {
        TEST_FAILURE;
    }

    while ( 0 < Binary_Heap_Length(Heap) ) {
        if ( 0 != Binary_Heap_Remove(Heap) ) {
            TEST_PRINTF("%s",
                        "Test Failure - Failed to perform Remove() operation on Binary_Heap_t.");
            Binary_Heap_Release(Heap);
            TEST_FAILURE;
        }
    }

    Binary_Heap_Release(Heap);
    TEST_SUCCESSFUL;
}

int Test_Binary_Heap_Next(void) {

    Binary_Heap_t *            Heap         = NULL;
    Binary_Heap_KeyValuePair_t KeyValuePair = {NULL, NULL};
    size_t                     Count        = 0;

    Heap = Testing_initialize_heap();
    if ( NULL == Heap ) {
        TEST_FAILURE;
    }

    BINARY_HEAP_FOREACH(Heap, KeyValuePair) {
        DEBUG_PRINTF("Retrieved Key-Value Pair (%d - \"%s\") from Binary_Heap_t.",
                     *(int *)KeyValuePair.Key, (char *)KeyValuePair.Value);
        Count++;
    }

    if ( Count != Binary_Heap_Length(Heap) ) {
        TEST_PRINTF("Test Failure - Iteration did not reach all items reported in Binary_Heap_t. "
                    "Reached (%lu), but Heap contains (%lu).",
                    (unsigned long)Count, (unsigned long)Binary_Heap_Length(Heap));
    }

    Binary_Heap_Release(Heap);
    TEST_SUCCESSFUL;
}

int Test_Binary_Heap_DoCallback(void) {

    Binary_Heap_t *Heap           = NULL;
    int            CallbackRetVal = 0;

    Heap = Testing_initialize_heap();
    if ( NULL == Heap ) {
        TEST_FAILURE;
    }

    CallbackRetVal = Binary_Heap_DoCallback(Heap, Test_Binary_Heap_DoCallbackFunc);
    if ( 0 > CallbackRetVal ) {
        TEST_PRINTF("%s", "Error: Failed to perform Callback() on Binary_Heap_t items.");
        Binary_Heap_Release(Heap);
        TEST_FAILURE;
    } else if ( 0 < CallbackRetVal ) {
        TEST_PRINTF(
            "Test Failure - Callback() returned non-zero for [ %d ] items in Binary_Heap_t.",
            CallbackRetVal);
        Binary_Heap_Release(Heap);
        TEST_FAILURE;
    }

    Binary_Heap_Release(Heap);
    TEST_SUCCESSFUL;
}

int Test_Binary_Heap_DoCallbackArg(void) {

    Binary_Heap_t *Heap           = NULL;
    int            CallbackRetVal = 0;

    Heap = Testing_initialize_heap();
    if ( NULL == Heap ) {
        TEST_FAILURE;
    }

    CallbackRetVal = Binary_Heap_DoCallbackArg(Heap, Test_Binary_Heap_DoCallbackArgFunc, NULL);
    if ( 0 > CallbackRetVal ) {
        TEST_PRINTF("%s", "Error: Failed to perform Callback() on Binary_Heap_t items.");
        Binary_Heap_Release(Heap);
        TEST_FAILURE;
    } else if ( 0 < CallbackRetVal ) {
        TEST_PRINTF(
            "Test Failure - Callback() returned non-zero for [ %d ] items in Binary_Heap_t.",
            CallbackRetVal);
        Binary_Heap_Release(Heap);
        TEST_FAILURE;
    }

    Binary_Heap_Release(Heap);
    TEST_SUCCESSFUL;
}

int Test_Binary_Heap_Clear(void) {

    Binary_Heap_t *Heap = NULL;

    Heap = Testing_initialize_heap();
    if ( NULL == Heap ) {
        TEST_FAILURE;
    }

    if ( 0 != Binary_Heap_Clear(Heap) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Clear() operation on Binary_Heap_t.");
        Binary_Heap_Release(Heap);
        TEST_FAILURE;
    }

    Binary_Heap_Release(Heap);
    TEST_SUCCESSFUL;
}
