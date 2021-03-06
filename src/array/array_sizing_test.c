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

int Test_Array_Grow(void) {

    Array_t *Array           = NULL;
    size_t   DesiredCapacity = 24;
    size_t   InitialCapacity = 16;

    Array = Array_Create(InitialCapacity, sizeof(int));
    if ( NULL == Array ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t.");
        TEST_FAILURE;
    }

    if ( 0 != Array_Grow(Array, DesiredCapacity) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Array_Grow().");
        Array_Release(Array);
        TEST_FAILURE;
    }

    if ( Array->Capacity < DesiredCapacity ) {
        TEST_PRINTF("Test Failure - Actual Array_t Capacity (%lu) is not large "
                    "enough to hold desired extra length (%lu).",
                    (unsigned long)Array->Capacity,
                    (unsigned long)(InitialCapacity + DesiredCapacity));
        Array_Release(Array);
        TEST_FAILURE;
    }

    Array_Release(Array);
    TEST_SUCCESSFUL;
}
