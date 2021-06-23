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
#include "include/array.h"

static int ArrayCompareFunc_Int(const void *A, const void *B) {
    return (*(const int *)A) - (*(const int *)B);
}

static int ArrayCompareFunc_Double(const void *A, const void *B) {
    if ( *(const double *)A < *(const double *)B ) {
        return -1;
    } else if ( *(const double *)B < *(const double *)A ) {
        return 1;
    }
    return 0;
}

int Test_array_sort(void) {

    int FailedTests = 0;

    FailedTests += Test_Array_Sort_Int();
    FailedTests += Test_Array_Sort_Double();

    return FailedTests;
}

int Test_Array_Sort_Int(void) {

    Array_t *Array     = NULL;
    size_t   ArraySize = 100, i = 0;
    int      Value = 0, *Lower = NULL, *Upper = NULL;

    Array = Array_Create(0, sizeof(int));
    if ( NULL == Array ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < ArraySize; i++ ) {
        Value = rand() % 1000;
        if ( 0 != Array_Append(Array, &Value) ) {
            TEST_PRINTF("Test Failure - Failed to append value [ %d ] to Array.", Value);
            Array_Release(Array);
            TEST_FAILURE;
        }
    }

    if ( 0 != Array_Sort(Array, ArrayCompareFunc_Int) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Sort() operation on Array_t.");
        Array_Release(Array);
        TEST_FAILURE;
    }

    for ( i = 0; i < (ArraySize - 1); i++ ) {
        Lower = Array_GetElement(Array, i);
        Upper = Array_GetElement(Array, i + 1);

        if ( (NULL == Lower) || (NULL == Upper) ) {
            TEST_PRINTF("%s", "Test Failure - Failed to check if Array is in Sorted order.");
            Array_Release(Array);
            TEST_FAILURE;
        }

        if ( *Lower > *Upper ) {
            TEST_PRINTF("Test Failure - Array is not in Sorted order, %d < %d?!", *Lower, *Upper);
            Array_Release(Array);
            TEST_FAILURE;
        }
    }

    Array_Release(Array);
    TEST_SUCCESSFUL;
}

int Test_Array_Sort_Double(void) {

    Array_t *Array     = NULL;
    size_t   ArraySize = 100, i = 0;
    double   Value = 0, *Lower = NULL, *Upper = NULL;

    Array = Array_Create(0, sizeof(double));
    if ( NULL == Array ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < ArraySize; i++ ) {
        Value = (rand() % 1000) / (double)25;
        if ( 0 != Array_Append(Array, &Value) ) {
            TEST_PRINTF("Test Failure - Failed to append value [ %lf ] to Array.", Value);
            Array_Release(Array);
            TEST_FAILURE;
        }
    }

    if ( 0 != Array_Sort(Array, ArrayCompareFunc_Double) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Sort() operation on Array_t.");
        Array_Release(Array);
        TEST_FAILURE;
    }

    for ( i = 0; i < (ArraySize - 1); i++ ) {
        Lower = Array_GetElement(Array, i);
        Upper = Array_GetElement(Array, i + 1);

        if ( (NULL == Lower) || (NULL == Upper) ) {
            TEST_PRINTF("%s", "Test Failure - Failed to check if Array is in Sorted order.");
            Array_Release(Array);
            TEST_FAILURE;
        }

        if ( *Lower > *Upper ) {
            TEST_PRINTF("Test Failure - Array is not in Sorted order, %lf < %lf?!", *Lower, *Upper);
            Array_Release(Array);
            TEST_FAILURE;
        }
    }

    Array_Release(Array);
    TEST_SUCCESSFUL;
}
