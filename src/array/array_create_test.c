/*
    Copyright (c) 2021 Bearnie-H

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
*/

#include <stdlib.h>

#include "../logging/logging.h"
#include "include/array.h"

int Test_Array_Create(void) {

    Array_t *Array = NULL;

    Array = Array_Create(0, sizeof(int));
    if (NULL == Array) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t*.");
        TEST_FAILURE;
    }

    Array_Release(Array);
    TEST_SUCCESSFUL;
}

int Test_Array_RefCreate(void) {

    Array_t *Array = NULL;

    Array = Array_RefCreate(0, sizeof(int), (ReleaseFunc_t *)Array_Release);
    if (NULL == Array) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t*.");
        TEST_FAILURE;
    }

    Array_Release(Array);
    TEST_SUCCESSFUL;
}

int Test_Array_Clear(void) {

    Array_t *Array = NULL;
    size_t Count = 32, i = 0;
    int Value = 0;

    Array = Array_Create(0, sizeof(Value));
    if (NULL == Array) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        Value = (int)i;
        if (0 != Array_Append(Array, &Value)) {
            TEST_PRINTF("%s", "Test Failure - Failed to add item to Array.");
            Array_Release(Array);
            TEST_FAILURE;
        }
    }

    if (0 != Array_Clear(Array)) {
        TEST_PRINTF("%s", "Test Failure - Failed to clear Array.");
        Array_Release(Array);
        TEST_FAILURE;
    }

    if (0 != Array_Length(Array)) {
        TEST_PRINTF("%s", "Test Failure - Array Length not equal to 0 after Array_Clear().");
        Array_Release(Array);
        TEST_FAILURE;
    }

    Array_Release(Array);
    TEST_SUCCESSFUL;
}
