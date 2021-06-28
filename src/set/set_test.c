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
#include "include/set.h"

static CompareFunc_t CompareFunc_Int;

static int CompareFunc_Int(const void *A, const void *B, __attribute__((unused)) size_t Size) {
    return (*(const int *)A - *(const int *)B);
}

static CallbackFunc_t    CallbackFunc_Int;
static CallbackArgFunc_t CallbackArgFunc_Int;

static int CallbackFunc_Int(void *Value) {

    if ( NULL == Value ) {
        DEBUG_PRINTF("%s", "Error: NULL Value* provided.");
        return 1;
    }

    DEBUG_PRINTF("Callback function called with integer [ %d ].", *(int *)Value);
    return 0;
}

static int CallbackArgFunc_Int(void *Value, void *Args) {

    if ( NULL == Value ) {
        DEBUG_PRINTF("%s", "Error: NULL Value* provided.");
        return 1;
    }

    if ( NULL == Args ) {
        DEBUG_PRINTF("Callback function called with integer [ %d ] and no Args.", *(int *)Value);
    } else {
        DEBUG_PRINTF("Callback function called with integer [ %d ] and Non-NULL Args.",
                     *(int *)Value);
    }
    return 0;
}

int Test_set(void) {

    int FailedTests = 0;

    FailedTests += Test_Set_Create();
    FailedTests += Test_Set_Length();
    FailedTests += Test_Set_Insert();
    FailedTests += Test_Set_ValueExists();
    FailedTests += Test_Set_Next();
    FailedTests += Test_Set_DoCallback();
    FailedTests += Test_Set_DoCallbackArgs();
    FailedTests += Test_Set_Remove();
    FailedTests += Test_Set_Clear();

    return FailedTests;
}

int Test_Set_Create(void) {

    Set_t *Set = NULL;

    Set = Set_Create(CompareFunc_Int, sizeof(int), NULL);
    if ( NULL == Set ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Set_t for testing.");
        TEST_FAILURE;
    }

    Set_Release(Set);
    TEST_SUCCESSFUL;
}

int Test_Set_Length(void) {

    Set_t *Set = NULL;

    Set = Set_Create(CompareFunc_Int, sizeof(int), NULL);
    if ( NULL == Set ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Set_t for testing.");
        TEST_FAILURE;
    }

    if ( 0 != Set_Length(Set) ) {
        TEST_PRINTF("%s", "Test Failure - Length of new Set_t not equal to 0.");
        Set_Release(Set);
        TEST_FAILURE;
    }

    Set_Release(Set);
    TEST_SUCCESSFUL;
}

int Test_Set_Insert(void) {

    Set_t *Set   = NULL;
    int    Count = 32, i = 0;

    Set = Set_Create(CompareFunc_Int, sizeof(int), NULL);
    if ( NULL == Set ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Set_t for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < Count; i++ ) {
        if ( 0 != Set_Insert(Set, &i, sizeof(i)) ) {
            TEST_PRINTF("Test Failure - Failed to insert item [ %d ] into Set_t.", i);
            Set_Release(Set);
            TEST_FAILURE;
        }
    }

    if ( (size_t)Count != Set_Length(Set) ) {
        TEST_PRINTF("Test Failure - Set length [ %lu ] not equal to expectation [ %lu ].",
                    (unsigned long)Set_Length(Set), (unsigned long)Count);
        Set_Release(Set);
        TEST_FAILURE;
    }

    Set_Release(Set);
    TEST_SUCCESSFUL;
}

int Test_Set_ValueExists(void) {

    Set_t *Set   = NULL;
    int    Count = 32, i = 0;

    Set = Set_Create(CompareFunc_Int, sizeof(int), NULL);
    if ( NULL == Set ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Set_t for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < Count; i++ ) {
        if ( 0 != Set_Insert(Set, &i, sizeof(i)) ) {
            TEST_PRINTF("Test Failure - Failed to insert item [ %d ] into Set_t.", i);
            Set_Release(Set);
            TEST_FAILURE;
        }
    }

    for ( i = 0; i < Count; i++ ) {
        if ( !Set_ValueExists(Set, &i, sizeof(i)) ) {
            TEST_PRINTF(
                "Test Failure - Item [ %d ] reported as successfully inserted, but was not found.",
                i);
            Set_Release(Set);
            TEST_FAILURE;
        }
    }

    Set_Release(Set);
    TEST_SUCCESSFUL;
}

int Test_Set_Next(void) {

    Set_t *Set   = NULL;
    int    Count = 32, i = 0, *CheckValue;

    Set = Set_Create(CompareFunc_Int, sizeof(int), NULL);
    if ( NULL == Set ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Set_t for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < Count; i++ ) {
        if ( 0 != Set_Insert(Set, &i, sizeof(i)) ) {
            TEST_PRINTF("Test Failure - Failed to insert item [ %d ] into Set_t.", i);
            Set_Release(Set);
            TEST_FAILURE;
        }
    }

    i = 0;
    SET_FOREACH(Set, CheckValue) {
        if ( *CheckValue != i ) {
            TEST_PRINTF("Test Failure - Retrieved value (%d) not equal to expectation (%d).",
                        *CheckValue, i);
            Set_Release(Set);
            TEST_FAILURE;
        }
        i++;
    }

    if ( i != Count ) {
        TEST_PRINTF("Test Failure - Set_t iterator only reached (%d) of (%d) items.", i, Count);
        Set_Release(Set);
        TEST_FAILURE;
    }

    Set_Release(Set);
    TEST_SUCCESSFUL;
}

int Test_Set_DoCallback(void) {

    Set_t *Set   = NULL;
    int    Count = 32, i = 0;

    Set = Set_Create(CompareFunc_Int, sizeof(int), NULL);
    if ( NULL == Set ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Set_t for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < Count; i++ ) {
        if ( 0 != Set_Insert(Set, &i, sizeof(i)) ) {
            TEST_PRINTF("Test Failure - Failed to insert item [ %d ] into Set_t.", i);
            Set_Release(Set);
            TEST_FAILURE;
        }
    }

    if ( 0 != Set_DoCallback(Set, CallbackFunc_Int) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Callback() on Set_t.");
        Set_Release(Set);
        TEST_FAILURE;
    }

    Set_Release(Set);
    TEST_SUCCESSFUL;
}

int Test_Set_DoCallbackArgs(void) {

    Set_t *Set   = NULL;
    int    Count = 32, i = 0;

    Set = Set_Create(CompareFunc_Int, sizeof(int), NULL);
    if ( NULL == Set ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Set_t for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < Count; i++ ) {
        if ( 0 != Set_Insert(Set, &i, sizeof(i)) ) {
            TEST_PRINTF("Test Failure - Failed to insert item [ %d ] into Set_t.", i);
            Set_Release(Set);
            TEST_FAILURE;
        }
    }

    if ( 0 != Set_DoCallbackArgs(Set, CallbackArgFunc_Int, NULL) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform CallbackArgs() on Set_t.");
        Set_Release(Set);
        TEST_FAILURE;
    }

    Set_Release(Set);
    TEST_SUCCESSFUL;
}

int Test_Set_Remove(void) {

    Set_t *Set   = NULL;
    int    Count = 32, i = 0;

    Set = Set_Create(CompareFunc_Int, sizeof(int), NULL);
    if ( NULL == Set ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Set_t for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < Count; i++ ) {
        if ( 0 != Set_Insert(Set, &i, sizeof(i)) ) {
            TEST_PRINTF("Test Failure - Failed to insert item [ %d ] into Set_t.", i);
            Set_Release(Set);
            TEST_FAILURE;
        }
    }

    for ( i = 0; i < Count; i++ ) {
        if ( 0 != Set_Remove(Set, &i, sizeof(i)) ) {
            TEST_PRINTF("Test Failure - Failed to remove item [ %d ] from Set_t.", i);
            Set_Release(Set);
            TEST_FAILURE;
        }
    }

    if ( 0 != Set_Length(Set) ) {
        TEST_PRINTF("%s", "Test Failure - Set Length not equal to 0 after removing all items.");
        Set_Release(Set);
        TEST_FAILURE;
    }

    Set_Release(Set);
    TEST_SUCCESSFUL;
}

int Test_Set_Clear(void) {

    Set_t *Set   = NULL;
    int    Count = 32, i = 0;

    Set = Set_Create(CompareFunc_Int, sizeof(int), NULL);
    if ( NULL == Set ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Set_t for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < Count; i++ ) {
        if ( 0 != Set_Insert(Set, &i, sizeof(i)) ) {
            TEST_PRINTF("Test Failure - Failed to insert item [ %d ] into Set_t.", i);
            Set_Release(Set);
            TEST_FAILURE;
        }
    }

    if ( 0 != Set_Clear(Set) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Clear() on Set_t.");
        Set_Release(Set);
        TEST_FAILURE;
    }

    if ( 0 != Set_Length(Set) ) {
        TEST_PRINTF("%s", "Test Failure - Set Length not equal to 0 after Clear().");
        Set_Release(Set);
        TEST_FAILURE;
    }

    Set_Release(Set);
    TEST_SUCCESSFUL;
}
