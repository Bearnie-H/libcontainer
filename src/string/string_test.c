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

int Test_string(void) {

    int FailedTests = 0;

    FailedTests += Test_String_Create();
    FailedTests += Test_String_CreateConst();
    FailedTests += Test_String_Length();
    FailedTests += Test_String_IsEmpty();
    FailedTests += Test_String_GetAtIndex();
    FailedTests += Test_String_GetFront();
    FailedTests += Test_String_GetBack();
    FailedTests += Test_String_Insert();
    FailedTests += Test_String_Prepend();
    FailedTests += Test_String_Append();
    FailedTests += Test_String_Replace();
    FailedTests += Test_String_Set();
    FailedTests += Test_String_ToCString();
    FailedTests += Test_String_Copy();
    FailedTests += Test_String_Substring();
    FailedTests += Test_String_Compare();
    FailedTests += Test_String_Clear();

    FailedTests += Test_string_fmt();

    return FailedTests;
}

int Test_String_Create(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Value.";

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_CreateConst(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Value.";

    String = String_CreateConst(StringValue);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Const String_t for testing.");
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_Length(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Value.";

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    if ((sizeof(StringValue) - 1) != String_Length(String)) {
        TEST_PRINTF("Test Failure - String Length (%d) not equal to expected value (%d).",
                    (int)String_Length(String), (int)(sizeof(StringValue) - 1));
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_IsEmpty(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Value.";

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    if (String_IsEmpty(String)) {
        TEST_PRINTF("%s", "Test Failure - String erroneously reporting as empty.");
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_GetAtIndex(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Value.";
    size_t CheckIndex = 4;

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    if (String_GetAtIndex(String, CheckIndex) != StringValue[CheckIndex]) {
        TEST_PRINTF(
            "Test Failure - Character at index [ %d ] (%c) not equal to expectation value (%c).",
            (int)CheckIndex, String_GetAtIndex(String, CheckIndex), StringValue[CheckIndex]);
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_GetFront(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Value.";

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    if (String_GetFront(String) != StringValue[0]) {
        TEST_PRINTF(
            "Test Failure - Character at index [ %d ] (%c) not equal to expectation value (%c).", 0,
            String_GetFront(String), StringValue[0]);
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_GetBack(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Value.";

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    if (String_GetBack(String) != StringValue[strlen(StringValue) - 1]) {
        TEST_PRINTF(
            "Test Failure - Character at index [ %d ] (%c) not equal to expectation value (%c).",
            (int)(sizeof(StringValue) - 1), String_GetBack(String),
            StringValue[(sizeof(StringValue) - 1)]);
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_Insert(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Value.", ToInsert[] = "ing",
         Expected[] = "Testing String Value.";
    size_t InsertIndex = 4;

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    if (0 != String_Insert(String, ToInsert, InsertIndex, sizeof(ToInsert) - 1)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Insert() operation.");
        String_Release(String);
        TEST_FAILURE;
    }

    if (0 != memcmp(String_ToCString(String), Expected, sizeof(Expected) - 1)) {
        TEST_PRINTF("Test Failure - String Contents (%s) not equal to expectation (%s).",
                    String_ToCString(String), Expected);
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_Prepend(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Value.", ToInsert[] = "ing",
         Expected[] = "ingTest String Value.";

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    if (0 != String_Prepend(String, ToInsert, sizeof(ToInsert) - 1)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Prepend() operation.");
        String_Release(String);
        TEST_FAILURE;
    }

    if (0 != memcmp(String_ToCString(String), Expected, sizeof(Expected) - 1)) {
        TEST_PRINTF("Test Failure - String Contents (%s) not equal to expectation (%s).",
                    String_ToCString(String), Expected);
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_Append(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Value.", ToInsert[] = "ing",
         Expected[] = "Test String Value.ing";

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    if (0 != String_Append(String, ToInsert, sizeof(ToInsert) - 1)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Append() operation.");
        String_Release(String);
        TEST_FAILURE;
    }

    if (0 != memcmp(String_ToCString(String), Expected, sizeof(Expected) - 1)) {
        TEST_PRINTF("Test Failure - String Contents (%s) not equal to expectation (%s).",
                    String_ToCString(String), Expected);
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_Replace(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Value.", ReplaceValue[] = "ZZZZ",
         Expectation[] = "ZZZZ String Value.";
    size_t ReplaceIndex = 0;

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    if (0 != String_Replace(String, ReplaceValue, ReplaceIndex, sizeof(ReplaceValue) - 1)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Replace() operation.");
        String_Release(String);
        TEST_FAILURE;
    }

    if (0 != memcmp(String_ToCString(String), Expectation, String_Length(String))) {
        TEST_PRINTF("Test Failure - String contents (%s) not equal to expectation (%s).",
                    String_ToCString(String), Expectation);
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_Set(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Value.", NewValue[] = "New String.";

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    if (0 != String_Set(String, NewValue, sizeof(NewValue) - 1)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Set() operation.");
        String_Release(String);
        TEST_FAILURE;
    }

    if (0 != memcmp(String_ToCString(String), NewValue, String_Length(String))) {
        TEST_PRINTF("Test Failure - String contents (%s) not equal to expectation (%s).",
                    String_ToCString(String), NewValue);
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_ToCString(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Val.";
    char *CString = NULL;

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    CString = String_ToCString(String);
    if (NULL == CString) {
        TEST_PRINTF("%s", "Test Failure - Failed to convert String_t to C-String.");
        String_Release(String);
        TEST_FAILURE;
    }

    if (0x00 != CString[String_Length(String)]) {
        TEST_PRINTF("%s", "Test Failure - ToCString() operation failed to NUL-Terminate string.");
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}

int Test_String_Copy(void) {

    String_t *String = NULL, *String2 = NULL;
    char StringValue[] = "Test String Value.";

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    String2 = String_Copy(String);
    if (NULL == String2) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Copy() operation.");
        String_Release(String);
        TEST_FAILURE;
    }

    if (0 != String_Clear(String)) {
        TEST_PRINTF("%s", "Test Failure - Failed to Clear() original String.");
        String_Release(String);
        String_Release(String2);
        TEST_FAILURE;
    }

    if (0 != memcmp(String_ToCString(String2), StringValue, String_Length(String2))) {
        TEST_PRINTF("Test Failure - Copied string contents (%s) not equal to expectation (%s).",
                    String_ToCString(String2), StringValue);
        String_Release(String);
        String_Release(String2);
        TEST_FAILURE;
    }

    String_Release(String);
    String_Release(String2);
    TEST_SUCCESSFUL;
}

int Test_String_Substring(void) {

    String_t *String = NULL, *Substring = NULL;
    char StringValue[] = "Test String Value.", SubstringExpectation[] = "String";
    size_t SubstringIndex = 5, SubstringLength = 6;

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    Substring = String_Substring(String, SubstringIndex, SubstringLength);
    if (NULL == Substring) {
        TEST_PRINTF("%s", "Test Failure - Failed to create new Substring.");
        String_Release(String);
        TEST_FAILURE;
    }

    if (0 != String_Clear(String)) {
        TEST_PRINTF("%s", "Test Failure - Failed to Clear() original String.");
        String_Release(String);
        String_Release(Substring);
        TEST_FAILURE;
    }

    if (0 != memcmp(String_ToCString(Substring), SubstringExpectation, String_Length(Substring))) {
        TEST_PRINTF("Test Failure - Copied string contents (%s) not equal to expectation (%s).",
                    String_ToCString(Substring), SubstringExpectation);
        String_Release(String);
        String_Release(Substring);
        TEST_FAILURE;
    }

    String_Release(String);
    String_Release(Substring);
    TEST_SUCCESSFUL;
}

int Test_String_Compare(void) {

    int FailedTests = 0;

    FailedTests += Test_String_Compare_Less();
    FailedTests += Test_String_Compare_Equal();
    FailedTests += Test_String_Compare_Greater();
    FailedTests += Test_String_Compare_NULL_A();
    FailedTests += Test_String_Compare_NULL_B();
    FailedTests += Test_String_Compare_NULL();

    return FailedTests;
}

int Test_String_Compare_Less(void) {

    String_t *String1 = NULL, *String2 = NULL;
    char Value1[] = "aaaaa", Value2[] = "bbbbb";

    String1 = String_Create(Value1, sizeof(Value1) - 1);
    if (NULL == String1) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t* for testing.");
        TEST_FAILURE;
    }

    String2 = String_Create(Value2, sizeof(Value2) - 1);
    if (NULL == String2) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t* for testing.");
        String_Release(String1);
        TEST_FAILURE;
    }

    if (0 <= String_Compare(String1, String2)) {
        TEST_PRINTF("Test Failure - String 1 (%s), and String 2 (%s) did not compare as expected.",
                    String_ToCString(String1), String_ToCString(String2));
        String_Release(String1);
        String_Release(String2);
        TEST_FAILURE;
    }

    String_Release(String1);
    String_Release(String2);
    TEST_SUCCESSFUL;
}

int Test_String_Compare_Equal(void) {

    String_t *String1 = NULL, *String2 = NULL;
    char Value1[] = "aaaaa", Value2[] = "aaaaa";

    String1 = String_Create(Value1, sizeof(Value1) - 1);
    if (NULL == String1) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t* for testing.");
        TEST_FAILURE;
    }

    String2 = String_Create(Value2, sizeof(Value2) - 1);
    if (NULL == String2) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t* for testing.");
        String_Release(String1);
        TEST_FAILURE;
    }

    if (0 != String_Compare(String1, String2)) {
        TEST_PRINTF("Test Failure - String 1 (%s), and String 2 (%s) did not compare as expected.",
                    String_ToCString(String1), String_ToCString(String2));
        String_Release(String1);
        String_Release(String2);
        TEST_FAILURE;
    }

    String_Release(String1);
    String_Release(String2);
    TEST_SUCCESSFUL;
}

int Test_String_Compare_Greater(void) {

    String_t *String1 = NULL, *String2 = NULL;
    char Value1[] = "bbbbb", Value2[] = "aaaaa";

    String1 = String_Create(Value1, sizeof(Value1) - 1);
    if (NULL == String1) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t* for testing.");
        TEST_FAILURE;
    }

    String2 = String_Create(Value2, sizeof(Value2) - 1);
    if (NULL == String2) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t* for testing.");
        String_Release(String1);
        TEST_FAILURE;
    }

    if (0 >= String_Compare(String1, String2)) {
        TEST_PRINTF("Test Failure - String 1 (%s), and String 2 (%s) did not compare as expected.",
                    String_ToCString(String1), String_ToCString(String2));
        String_Release(String1);
        String_Release(String2);
        TEST_FAILURE;
    }

    String_Release(String1);
    String_Release(String2);
    TEST_SUCCESSFUL;
}

int Test_String_Compare_NULL_A(void) {

    String_t *String1 = NULL, *String2 = NULL;
    char Value2[] = "bbbbb";

    String2 = String_Create(Value2, sizeof(Value2) - 1);
    if (NULL == String2) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t* for testing.");
        TEST_FAILURE;
    }

    if (0 >= String_Compare(String1, String2)) {
        TEST_PRINTF("Test Failure - String 1 (%s), and String 2 (%s) did not compare as expected.",
                    "NULL", String_ToCString(String2));
        String_Release(String1);
        String_Release(String2);
        TEST_FAILURE;
    }

    String_Release(String1);
    String_Release(String2);
    TEST_SUCCESSFUL;
}

int Test_String_Compare_NULL_B(void) {

    String_t *String1 = NULL, *String2 = NULL;
    char Value1[] = "aaaaa";

    String1 = String_Create(Value1, sizeof(Value1) - 1);
    if (NULL == String1) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t* for testing.");
        TEST_FAILURE;
    }

    if (0 <= String_Compare(String1, String2)) {
        TEST_PRINTF("Test Failure - String 1 (%s), and String 2 (%s) did not compare as expected.",
                    String_ToCString(String1), "NULL");
        String_Release(String1);
        String_Release(String2);
        TEST_FAILURE;
    }

    String_Release(String1);
    String_Release(String2);
    TEST_SUCCESSFUL;
}

int Test_String_Compare_NULL(void) {

    String_t *String1 = NULL, *String2 = NULL;

    if (0 != String_Compare(String1, String2)) {
        TEST_PRINTF("Test Failure - String 1 (%s), and String 2 (%s) did not compare as expected.",
                    "NULL", "NULL");
        String_Release(String1);
        String_Release(String2);
        TEST_FAILURE;
    }

    String_Release(String1);
    String_Release(String2);
    TEST_SUCCESSFUL;
}

int Test_String_Clear(void) {

    String_t *String = NULL;
    char StringValue[] = "Test String Value.";

    String = String_Create(StringValue, sizeof(StringValue) - 1);
    if (NULL == String) {
        TEST_PRINTF("%s", "Test Failure - Failed to create String_t for testing.");
        TEST_FAILURE;
    }

    if (0 != String_Clear(String)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Clear() operation.");
        String_Release(String);
        TEST_FAILURE;
    }

    if (!String_IsEmpty(String)) {
        TEST_PRINTF("%s", "Test Failure - String reporting non-empty after being cleared.");
        String_Release(String);
        TEST_FAILURE;
    }

    String_Release(String);
    TEST_SUCCESSFUL;
}
