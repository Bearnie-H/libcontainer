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

#include "../../include/libcontainer.h"
#include "../logging/logging.h"
#include "include/CompareFunc.h"

int Test_CompareFunc(void) {

    int FailedTests = 0;

    FailedTests += Test_CompareFunc_Int_Equal();
    FailedTests += Test_CompareFunc_Int_Greater();
    FailedTests += Test_CompareFunc_Int_Lesser();
    FailedTests += Test_CompareFunc_String_Equal();
    FailedTests += Test_CompareFunc_String_Greater();
    FailedTests += Test_CompareFunc_String_Lesser();

    return FailedTests;
}

int Test_CompareFunc_Int_Equal(void) {

    int A = 0, B = 0;

    if ( 0 != CompareFunc_Int_Ascending(&A, &B, 0) ) {
        TEST_PRINTF("Test Failure - A (%d) compared unequal to B (%d).", A, B);
        TEST_FAILURE;
    }

    TEST_SUCCESSFUL;
}

int Test_CompareFunc_Int_Greater(void) {

    int A = 10, B = 0;

    if ( 0 >= CompareFunc_Int_Ascending(&A, &B, 0) ) {
        TEST_PRINTF("Test Failure - A (%d) compared less than or equal to B (%d).", A, B);
        TEST_FAILURE;
    }

    TEST_SUCCESSFUL;
}

int Test_CompareFunc_Int_Lesser(void) {

    int A = 0, B = 10;

    if ( 0 <= CompareFunc_Int_Ascending(&A, &B, 0) ) {
        TEST_PRINTF("Test Failure - A (%d) compared greater than or equal to B (%d).", A, B);
        TEST_FAILURE;
    }

    TEST_SUCCESSFUL;
}

int Test_CompareFunc_String_Equal(void) {

    const char A[] = "MMMMMMMMMM", B[] = "MMMMMMMMMM";

    if ( 0 != CompareFunc_String_Ascending(A, B, 0) ) {
        TEST_PRINTF("Test Failure - A (%s) compared unequal to B (%s).", A, B);
        TEST_FAILURE;
    }

    TEST_SUCCESSFUL;
}

int Test_CompareFunc_String_Greater(void) {

    const char A[] = "MMMMMMMMMM", B[] = "AAAAAAAAAA";

    if ( 0 >= CompareFunc_String_Ascending(A, B, 0) ) {
        TEST_PRINTF("Test Failure - A (%s) compared less than or equal to B (%s).", A, B);
        TEST_FAILURE;
    }

    TEST_SUCCESSFUL;
}

int Test_CompareFunc_String_Lesser(void) {

    const char A[] = "MMMMMMMMMM", B[] = "ZZZZZZZZZZ";

    if ( 0 <= CompareFunc_String_Ascending(A, B, 0) ) {
        TEST_PRINTF("Test Failure - A (%s) compared greater than or equal to B (%s).", A, B);
        TEST_FAILURE;
    }

    TEST_SUCCESSFUL;
}
