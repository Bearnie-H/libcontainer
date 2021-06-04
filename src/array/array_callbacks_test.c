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

#define LIBCONTAINER_ENABLE_ARRAY

#include "../logging/logging.h"
#include "include/array.h"

static int TestCallbackFunc_PrintIntValue(void *Value) {

    int *IntValue = (int *)Value;

    /*
        Note, this will never be NULL, as promised by the library.
        This check is only here to suppress the compiler warning
        -Wunused-parameter.
    */
    if (NULL == IntValue) {
        TEST_PRINTF("%s", "Error: NULL Value* provided.");
        return 1;
    }

    DEBUG_PRINTF("%d ", *(int *)IntValue);

    return 0;
}

static int TestCallbackArgFunc_PrintIntValue(void *Value, void *Args) {

    int *IntValue = (int *)Value;
    int *Divisor = (int *)Args;

    if (NULL == Divisor) {
        TEST_PRINTF("%s", "Error: NULL Divisor* provided.");
        return 1;
    }

    if (0 == (*IntValue % *Divisor)) {
        DEBUG_PRINTF("Array value [ %d ] is divisible by [ %d ]!", *IntValue, *Divisor);
    } else {
        DEBUG_PRINTF("Array value [ %d ] is NOT divisible by [ %d ]!", *IntValue, *Divisor);
    }

    return 0;
}

int Test_array_callbacks(void) {

    int FailedTests = 0;

    FailedTests += Test_Array_DoCallback();
    FailedTests += Test_Array_DoCallbackArg();

    return FailedTests;
}

int Test_Array_DoCallback(void) {

    Array_t *Array = NULL;
    size_t Size = 64;
    int i = 0;

    Array = Array_Create(Size, sizeof(int));
    if (NULL == Array) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < (int)Size; i++) {
        if (0 != Array_Append(Array, &i)) {
            TEST_PRINTF("Test Failure - Failed to append value [ %d ] to Array_t.", i);
            Array_Release(Array);
            TEST_FAILURE;
        }
    }

    if (0 != Array_DoCallback(Array, TestCallbackFunc_PrintIntValue)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Callback on Array elements.");
        Array_Release(Array);
        TEST_FAILURE;
    }

    Array_Release(Array);
    TEST_SUCCESSFUL;
}

int Test_Array_DoCallbackArg(void) {

    Array_t *Array = NULL;
    size_t Size = 64;
    int i = 0;
    int Divisor = 5;

    Array = Array_Create(Size, sizeof(int));
    if (NULL == Array) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < (int)Size; i++) {
        if (0 != Array_Append(Array, &i)) {
            TEST_PRINTF("Test Failure - Failed to append value [ %d ] to Array_t.", i);
            Array_Release(Array);
            TEST_FAILURE;
        }
    }

    if (0 != Array_DoCallbackArg(Array, TestCallbackArgFunc_PrintIntValue, &Divisor)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Callback on Array elements.");
        Array_Release(Array);
        TEST_FAILURE;
    }

    Array_Release(Array);
    TEST_SUCCESSFUL;
}
