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
#include "include/hashmap.h"

static int TestCallbackFunc_PrintIntValue(void *Value) {

    int **KeyValuePair = (int **)Value;
    int *MapKey = KeyValuePair[0];
    int *MapValue = KeyValuePair[1];

    if ((NULL == MapKey) || (NULL == MapValue)) {
        TEST_PRINTF("%s", "Error: NULL MapKey* or MapValue* provided.");
        return 1;
    }

    DEBUG_PRINTF("Hashmap Key: %d --- Hashmap Value: %d", *MapKey, *MapValue);

    return 0;
}

static int TestCallbackArgFunc_PrintIntValue(void *Value, void *Args) {

    int **KeyValuePair = (int **)Value;
    int *MapKey = KeyValuePair[0];
    int *MapValue = KeyValuePair[1];
    int *Divisor = (int *)Args;

    if (NULL == Divisor) {
        TEST_PRINTF("%s", "Error: NULL Divisor* provided.");
        return 1;
    }

    if ((NULL == MapKey) || (NULL == MapValue)) {
        TEST_PRINTF("%s", "Error: NULL MapKey* or MapValue* provided.");
        return 1;
    }

    if (0 == (*MapValue % *Divisor)) {
        DEBUG_PRINTF("Hashmap value [ %d ] (key = %d) is divisible by [ %d ]!", *MapValue, *MapKey,
                     *Divisor);
    } else {
        DEBUG_PRINTF("Hashmap value [ %d ] (key = %d) is NOT divisible by [ %d ]!", *MapValue,
                     *MapKey, *Divisor);
    }

    return 0;
}

int Test_hashmap_callbacks(void) {

    int FailedTests = 0;

    FailedTests += Test_Hashmap_DoCallbackFunc();
    FailedTests += Test_Hashmap_DoCallbackArgFunc();

    return FailedTests;
}

int Test_Hashmap_DoCallbackFunc(void) {

    Hashmap_t *Map = NULL;
    size_t Size = 64;
    int i = 0, Key = 0;
    ;

    Map = Hashmap_Create(HashFunc_Int, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t* for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < (int)Size; i++) {
        Key = rand();
        if (0 != Hashmap_Insert(Map, &Key, &i, 0, sizeof(i), NULL)) {
            TEST_PRINTF("Test Failure - Failed to insert Key (%d) - Value (%d) pair into Hashmap.",
                        Key, i);
            Hashmap_Release(Map);
            TEST_FAILURE;
        }
    }

    if (0 != Hashmap_DoCallback(Map, TestCallbackFunc_PrintIntValue)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Hashmap_DoCallback().");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}
int Test_Hashmap_DoCallbackArgFunc(void) {

    Hashmap_t *Map = NULL;
    size_t Size = 64;
    int i = 0, Key = 0, Divisor = 7;

    Map = Hashmap_Create(HashFunc_Int, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t* for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < (int)Size; i++) {
        Key = rand();
        if (0 != Hashmap_Insert(Map, &Key, &i, 0, sizeof(i), NULL)) {
            TEST_PRINTF("Test Failure - Failed to insert Key (%d) - Value (%d) pair into Hashmap.",
                        Key, i);
            Hashmap_Release(Map);
            TEST_FAILURE;
        }
    }

    if (0 != Hashmap_DoCallbackArg(Map, TestCallbackArgFunc_PrintIntValue, &Divisor)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Hashmap_DoCallbackArg().");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}
