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

#include "../logging/logging.h"
#include "include/string.h"
#include "include/string_fmt.h"

int Test_string_fmt(void) {

    int FailedTests = 0;

    FailedTests += Test_String_Createf();
    FailedTests += Test_String_SPrintf();
    FailedTests += Test_String_Appendf();

    return FailedTests;
}

int Test_String_Createf(void) {

    String_t * String        = NULL;
    const char fmt[]         = "Hello [ %s ]. After 9 comes %d.";
    char       ArgString[]   = "World";
    int        ArgNumber     = 10;
    const char Expectation[] = "Hello [ World ]. After 9 comes 10.";

    String = String_Createf(fmt, ArgString, ArgNumber);
    if ( NULL == String ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t from format string.");
        TEST_FAILURE;
    }

    if ( (sizeof(Expectation) - 1) != String_Length(String) ) {
        TEST_PRINTF("Test Failure - Resulting String length (%d) is not as expected (%d).",
                    (int)String_Length(String), (int)(sizeof(Expectation) - 1));
        String_Release(String);
        TEST_FAILURE;
    }

    if ( 0 != memcmp(String_ToCString(String), Expectation, String_Length(String)) ) {
        TEST_PRINTF("Test Failure - String Contents (%s) not equal to expectation (%s).",
                    String_ToCString(String), Expectation);
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_SPrintf(void) {

    String_t * String        = NULL;
    const char fmt[]         = "Hello [ %s ]. After 9 comes %d.";
    char       ArgString[]   = "World";
    int        ArgNumber     = 10;
    const char Expectation[] = "Hello [ World ]. After 9 comes 10.";

    String = String_Create(NULL, 0);
    if ( NULL == String ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    if ( 0 != String_SPrintf(String, fmt, ArgString, ArgNumber) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to write formatted string to String_t.");
        String_Release(String);
        TEST_FAILURE;
    }

    if ( (sizeof(Expectation) - 1) != String_Length(String) ) {
        TEST_PRINTF("Test Failure - Resulting String length (%d) is not as expected (%d).",
                    (int)String_Length(String), (int)(sizeof(Expectation) - 1));
        String_Release(String);
        TEST_FAILURE;
    }

    if ( 0 != memcmp(String_ToCString(String), Expectation, String_Length(String)) ) {
        TEST_PRINTF("Test Failure - String Contents (%s) not equal to expectation (%s).",
                    String_ToCString(String), Expectation);
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_Appendf(void) {

    String_t * String        = NULL;
    char       Initial[]     = "Message: ";
    const char fmt[]         = "Hello [ %s ]. After 9 comes %d.";
    char       ArgString[]   = "World";
    int        ArgNumber     = 10;
    const char Expectation[] = "Message: Hello [ World ]. After 9 comes 10.";

    String = String_Create(Initial, sizeof(Initial) - 1);
    if ( NULL == String ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    if ( 0 != String_Appendf(String, fmt, ArgString, ArgNumber) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to write formatted string to String_t.");
        String_Release(String);
        TEST_FAILURE;
    }

    if ( (sizeof(Expectation) - 1) != String_Length(String) ) {
        TEST_PRINTF("Test Failure - Resulting String length (%d) is not as expected (%d).",
                    (int)String_Length(String), (int)(sizeof(Expectation) - 1));
        String_Release(String);
        TEST_FAILURE;
    }

    if ( 0 != memcmp(String_ToCString(String), Expectation, String_Length(String)) ) {
        TEST_PRINTF("Test Failure - String Contents (%s) not equal to expectation (%s).",
                    String_ToCString(String), Expectation);
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}
