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
#include "include/list.h"

int Test_List_Create(void) {

    List_t *List = NULL;

    List = List_Create();
    if ( NULL == List ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_Clear(void) {

    List_t *List  = NULL;
    size_t  Count = 32, i = 0;
    int     Value = 0;

    List = List_Create();
    if ( NULL == List ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t* for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < Count; i++ ) {
        if ( 0 != List_Append(List, &Value, sizeof(Value)) ) {
            TEST_PRINTF("%s", "Test Failure - Failed to add item to List_t.");
            List_Release(List);
            TEST_FAILURE;
        }
    }

    if ( 0 != List_Clear(List) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to clear List_t.");
        List_Release(List);
        TEST_FAILURE;
    }

    if ( 0 != List_Length(List) ) {
        TEST_PRINTF("%s", "Test Failure - List_t length not 0 after being cleared.");
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}
