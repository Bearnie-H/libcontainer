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

#include <stdlib.h>
#include <stdio.h>

#include "include/list.h"
#include "../logging/logging.h"

static int TestCallbackFunc_PrintIntValue(void* Value) {

    int* IntValue = (int*)Value;

    /*
        Note, this will never be NULL, as promised by the library.
        This check is only here to suppress the compiler warning
        -Wunused-parameter.
    */
    if ( NULL == IntValue) {
        TEST_PRINTF("%s", "Error: NULL Value* provided.");
        return 1;
    }

    DEBUG_PRINTF("%d ", *(int*)IntValue);

    return 0;
}

static int TestCallbackArgFunc_PrintIntValue(void* Value, void* Args) {

    int* IntValue = (int*)Value;
    int* Divisor = (int*)Args;

    if ( NULL == Divisor ) {
        TEST_PRINTF("%s", "Error: NULL Divisor* provided.");
        return 1;
    }

    if ( 0 == (*IntValue % *Divisor)) {
        DEBUG_PRINTF("List value [ %d ] is divisible by [ %d ]!", *IntValue, *Divisor);
    } else {
        DEBUG_PRINTF("List value [ %d ] is NOT divisible by [ %d ]!", *IntValue, *Divisor);
    }

    return 0;
}

int Test_list_callbacks(void) {

    int FailedTests = 0;

    FailedTests += Test_List_DoCallback();
    FailedTests += Test_List_DoCallbackArg();

    return FailedTests;
}

int Test_List_DoCallback(void) {

    List_t* List = NULL;
    size_t Size = 64;
    int i = 0;

    List = List_Create();
    if ( NULL == List ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t* for testing.");
        TEST_FAILURE;
    }

    for(i=0;i<(int)Size;i++){
        if(0!=List_Append(List,&i,sizeof(i))){
            TEST_PRINTF("%s","Test Failure - Failed to append value to List_t.");
            List_Release(List);
            TEST_FAILURE;
        }
    }

    if(0!=List_DoCallback(List,TestCallbackFunc_PrintIntValue)){
        TEST_PRINTF("%s","Test Failure - Failed to perform List_DoCallback().");
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_DoCallbackArg(void) {

    List_t* List = NULL;
    size_t Size = 64;
    int i = 0, Divisor = 6;

    List = List_Create();
    if ( NULL == List ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t* for testing.");
        TEST_FAILURE;
    }

    for(i=0;i<(int)Size;i++){
        if(0!=List_Append(List,&i,sizeof(i))){
            TEST_PRINTF("%s","Test Failure - Failed to append value to List_t.");
            List_Release(List);
            TEST_FAILURE;
        }
    }

    if(0!=List_DoCallbackArg(List,TestCallbackArgFunc_PrintIntValue,&Divisor)){
        TEST_PRINTF("%s","Test Failure - Failed to perform List_DoCallbackArg().");
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}
