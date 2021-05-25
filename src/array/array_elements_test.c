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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/include/error.h"
#include "include/array.h"

int Test_Array_Prepend(void) {

    Array_t *Array = NULL;
    const char Elements[] = "Hello World!";
    const char Expected[] = "HHello World!";

    Array = Array_Create(0, sizeof(char));
    if (NULL == Array) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t.");
        return 1;
    }

    if (0 != Array_InsertN(Array, Elements, 0, (sizeof(Elements) - 1))) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to insert initial contents to Array_t.");
        Array_Release(Array);
        return 1;
    }

    if (0 != Array_Prepend(Array, Elements)) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to prepend additional element to Array_t.");
        Array_Release(Array);
        return 1;
    }

    if (0 != memcmp(Array->Contents, Expected, Array_Length(Array))) {
        TEST_PRINTF("Test Failure - Array contents [ %s ] does not match "
                    "expected value [ %s ].",
                    (char *)Array->Contents, Expected);
        Array_Release(Array);
        return 1;
    }

    Array_Release(Array);
    TEST_PRINTF("%s", "Test Successful.");
    return 0;
}

int Test_Array_Append(void) {

    Array_t *Array = NULL;
    const char Elements[] = "Hello World!";
    const char Expected[] = "Hello World!H";

    Array = Array_Create(0, sizeof(char));
    if (NULL == Array) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t.");
        return 1;
    }

    if (0 != Array_InsertN(Array, Elements, 0, (sizeof(Elements) - 1))) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to insert initial contents to Array_t.");
        Array_Release(Array);
        return 1;
    }

    if (0 != Array_Append(Array, Elements)) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to append additional element to Array_t.");
        Array_Release(Array);
        return 1;
    }

    if (0 != memcmp(Array->Contents, Expected, Array_Length(Array))) {
        TEST_PRINTF("Test Failure - Array contents [ %s ] does not match "
                    "expected value [ %s ].",
                    (char *)Array->Contents, Expected);
        Array_Release(Array);
        return 1;
    }

    Array_Release(Array);
    TEST_PRINTF("%s", "Test Successful.");
    return 0;
}

int Test_Array_Insert(void) {

    Array_t *Array = NULL;
    const char Elements[] = "Hello World!";
    const int InsertIndex = 5;
    const char Expected[] = "HelloH World!";

    Array = Array_Create(0, sizeof(char));
    if (NULL == Array) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t.");
        return 1;
    }

    if (0 != Array_InsertN(Array, Elements, 0, (sizeof(Elements) - 1))) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to insert initial contents to Array_t.");
        Array_Release(Array);
        return 1;
    }

    if (0 != Array_Insert(Array, Elements, InsertIndex)) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to insert additional element to Array_t.");
        Array_Release(Array);
        return 1;
    }

    if (0 != memcmp(Array->Contents, Expected, Array_Length(Array))) {
        TEST_PRINTF("Test Failure - Array contents [ %s ] does not match "
                    "expected value [ %s ].",
                    (char *)Array->Contents, Expected);
        Array_Release(Array);
        return 1;
    }

    Array_Release(Array);
    TEST_PRINTF("%s", "Test Successful.");
    return 0;
}

int Test_Array_InsertN(void) {

    Array_t *Array = NULL;
    const char Elements[] = "Hello World!";
    const int InsertIndex = 5;
    const char Expected[] = "HelloHello World! World!";

    Array = Array_Create(0, sizeof(char));
    if (NULL == Array) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t.");
        return 1;
    }

    if (0 != Array_InsertN(Array, Elements, 0, (sizeof(Elements) - 1))) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to insert initial contents to Array_t.");
        Array_Release(Array);
        return 1;
    }

    if (0 !=
        Array_InsertN(Array, Elements, InsertIndex, (sizeof(Elements) - 1))) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to insert additional elements to Array_t.");
        Array_Release(Array);
        return 1;
    }

    if (0 != memcmp(Array->Contents, Expected, Array_Length(Array))) {
        TEST_PRINTF("Test Failure - Array contents [ %s ] does not match "
                    "expected value [ %s ].",
                    (char *)Array->Contents, Expected);
        Array_Release(Array);
        return 1;
    }

    Array_Release(Array);
    TEST_PRINTF("%s", "Test Successful.");
    return 0;
}

int Test_Array_Remove(void) {

    Array_t *Array = NULL;
    const char Elements[] = "Hello World!";
    const int RemoveIndex = 5;
    const char Expected[] = "HelloWorld!";

    Array = Array_Create(0, sizeof(char));
    if (NULL == Array) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t.");
        return 1;
    }

    if (0 != Array_InsertN(Array, Elements, 0, (sizeof(Elements) - 1))) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to insert initial contents to Array_t.");
        Array_Release(Array);
        return 1;
    }

    if (0 != Array_Remove(Array, RemoveIndex)) {
        TEST_PRINTF("Test Failure - Failed to remove element at "
                    "index [ %d ] from Array_t.",
                    RemoveIndex);
        Array_Release(Array);
        return 1;
    }

    if (0 != memcmp(Array->Contents, Expected, Array_Length(Array))) {
        TEST_PRINTF("Test Failure - Array contents [ %s ] does not match "
                    "expected value [ %s ].",
                    (char *)Array->Contents, Expected);
        Array_Release(Array);
        return 1;
    }

    Array_Release(Array);
    TEST_PRINTF("%s", "Test Successful.");
    return 0;
}

int Test_Array_RemoveN(void) {

    Array_t *Array = NULL;
    const char Elements[] = "Hello World!";
    const int RemoveIndex = 5;
    const char Expected[] = "Hello";

    Array = Array_Create(0, sizeof(char));
    if (NULL == Array) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t.");
        return 1;
    }

    if (0 != Array_InsertN(Array, Elements, 0, (sizeof(Elements) - 1))) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to insert initial contents to Array_t.");
        Array_Release(Array);
        return 1;
    }

    if (0 != Array_RemoveN(Array, RemoveIndex, Array_Length(Array))) {
        TEST_PRINTF("Test Failure - Failed to remove elements starting at "
                    "index [ %d ] from Array_t.",
                    RemoveIndex);
        Array_Release(Array);
        return 1;
    }

    if (0 != memcmp(Array->Contents, Expected, Array_Length(Array))) {
        TEST_PRINTF("Test Failure - Array contents [ %s ] does not match "
                    "expected value [ %s ].",
                    (char *)Array->Contents, Expected);
        Array_Release(Array);
        return 1;
    }

    Array_Release(Array);
    TEST_PRINTF("%s", "Test Successful.");
    return 0;
}

int Test_Array_GetElement(void) {

    Array_t *Array = NULL;
    const char Contents[] = "Hello World!";
    char *Element = NULL;
    int Index = 5;
    const char Expected = ' ';

    Array = Array_Create(0, sizeof(char));
    if (NULL == Array) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t.");
        return 1;
    }

    if (0 != Array_InsertN(Array, Contents, 0, (sizeof(Contents) - 1))) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to insert initial contents to Array_t.");
        Array_Release(Array);
        return 1;
    }

    Element = (char *)Array_GetElement(Array, Index);
    if (NULL == Element) {
        TEST_PRINTF(
            "Test Failure - Failed to retrieve element at index [ %d ].",
            Index);
        Array_Release(Array);
        return 1;
    }

    if (*Element != Expected) {
        TEST_PRINTF("Test Failure - Returned element (%c) not equal to "
                    "expected element (%c).",
                    *Element, Expected);
        Array_Release(Array);
        return 1;
    }

    Array_Release(Array);
    TEST_PRINTF("%s", "Test Successful.");
    return 0;
}

int Test_Array_SetElement(void) {

    Array_t *Array = NULL;
    const char Contents[] = "Hello World!";
    int Index = 5;
    const char NewValue = '?';
    const char Expected[] = "Hello?World!";

    Array = Array_Create(0, sizeof(char));
    if (NULL == Array) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Array_t.");
        return 1;
    }

    if (0 != Array_InsertN(Array, Contents, 0, (sizeof(Contents) - 1))) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to insert initial contents to Array_t.");
        Array_Release(Array);
        return 1;
    }

    if (0 != Array_SetElement(Array, &(NewValue), Index)) {
        TEST_PRINTF("Test Failure - Failed to set element at index [ %d ].",
                    Index);
        Array_Release(Array);
        return 1;
    }

    if (0 != memcmp(Array->Contents, Expected, Array_Length(Array))) {
        TEST_PRINTF("Test Failure - Final contents (%s) not equal to expected "
                    "value (%s).",
                    Array->Contents, Expected);
        Array_Release(Array);
        return 1;
    }

    Array_Release(Array);
    TEST_PRINTF("%s", "Test Successful.");
    return 0;
}

int Array_Ref_InsertN(void) {

    Array_t *Outer = NULL;
    Array_t *Inner = NULL;
    char Contents[3] = "";
    int Index = 0;
    int SubArrayCount = 16;

    Outer = Array_RefCreate(0, sizeof(Array_t *), (ReleaseFunc_t)Array_Release);
    if (NULL == Outer) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to create outer Array_t of Array_t's.");
        return 1;
    }

    for (Index = 0; Index < SubArrayCount; Index++) {
        Inner = Array_Create(0, sizeof(char));
        if (NULL == Inner) {
            TEST_PRINTF("Test Failure - Failed to create inner Array_t [ %d ].",
                        Index);
            Array_Release(Outer);
            return 1;
        }

        snprintf(Contents, sizeof(Contents), "%d", Index);

        if (0 != Array_InsertN(Inner, Contents, 0, strlen(Contents))) {
            TEST_PRINTF(
                "Test Failure - Failed to set contents for Array_t [ %d ].",
                Index);
            Array_Release(Outer);
            return 1;
        }

        if (0 !=
            Array_InsertN(Outer, &(Inner), ((Index >= 1) ? (1) : (0)), 1)) {
            TEST_PRINTF("Test Failure - Failed to insert inner Array_t [ %d ] "
                        "into outer Array_t.",
                        Index);
            Array_Release(Outer);
            return 1;
        }
    }

    for (Index = 0; Index < SubArrayCount; Index++) {
        Inner = Array_GetElement(Outer, Index);
        if (NULL == Inner) {
            TEST_PRINTF("Test Failure - Failed to retrieve inner Array_t at "
                        "index [ %d ].",
                        Index);
            Array_Release(Outer);
            return 1;
        }

        if (0 == Index) {
            snprintf(Contents, sizeof(Contents), "%d", Index);
        } else {
            snprintf(Contents, sizeof(Contents), "%d", SubArrayCount - Index);
        }

        if (0 != memcmp(Contents, (char *)Inner->Contents, strlen(Contents))) {
            TEST_PRINTF("Test Failure - Array_t at index [ %d ] contents (%s) "
                        "does not match expectation (%s).",
                        Index, Inner->Contents, Contents);
            Array_Release(Outer);
            return 1;
        }
    }

    /* How to actually validate this worked? */

    Array_Release(Outer);
    TEST_PRINTF("%s", "Test Successful.");
    return 0;
}

int Array_Ref_RemoveN(void) {

    Array_t *Outer = NULL;
    Array_t *Inner = NULL;
    char Contents[3] = "";
    int Index = 0;
    int RemoveIndex = 5;
    int SubArrayCount = 16;

    Outer = Array_RefCreate(0, sizeof(Array_t *), (ReleaseFunc_t)Array_Release);
    if (NULL == Outer) {
        TEST_PRINTF(
            "%s",
            "Test Failure - Failed to create outer Array_t of Array_t's.");
        return 1;
    }

    for (Index = 0; Index < SubArrayCount; Index++) {
        Inner = Array_Create(0, sizeof(char));
        if (NULL == Inner) {
            TEST_PRINTF("Test Failure - Failed to create inner Array_t [ %d ].",
                        Index);
            Array_Release(Outer);
            return 1;
        }

        snprintf(Contents, sizeof(Contents), "%d", Index);

        if (0 != Array_InsertN(Inner, Contents, 0, strlen(Contents))) {
            TEST_PRINTF(
                "Test Failure - Failed to set contents for Array_t [ %d ].",
                Index);
            Array_Release(Outer);
            return 1;
        }

        if (0 !=
            Array_InsertN(Outer, &(Inner), ((Index >= 1) ? (1) : (0)), 1)) {
            TEST_PRINTF("Test Failure - Failed to insert inner Array_t [ %d ] "
                        "into outer Array_t.",
                        Index);
            Array_Release(Outer);
            return 1;
        }
    }

    if (0 != Array_RemoveN(Outer, 5, 1)) {
        TEST_PRINTF("%s", "Test Failure - Failed to remove inner Array_t from "
                          "outer Array_t.");
        Array_Release(Outer);
        return 1;
    }

    for (Index = 0; Index < (int)Array_Length(Outer); Index++) {
        Inner = Array_GetElement(Outer, Index);
        if (NULL == Inner) {
            TEST_PRINTF("Test Failure - Failed to retrieve inner Array_t at "
                        "index [ %d ].",
                        Index);
            Array_Release(Outer);
            return 1;
        }

        if (0 == Index) {
            snprintf(Contents, sizeof(Contents), "%d", Index);
        } else if (Index >= RemoveIndex) {
            snprintf(Contents, sizeof(Contents), "%d",
                     SubArrayCount - Index - 1);
        } else {
            snprintf(Contents, sizeof(Contents), "%d", SubArrayCount - Index);
        }

        if (0 != memcmp(Contents, (char *)Inner->Contents, strlen(Contents))) {
            TEST_PRINTF("Test Failure - Array_t at index [ %d ] contents (%s) "
                        "does not match expectation (%s).",
                        Index, Inner->Contents, Contents);
            Array_Release(Outer);
            return 1;
        }
    }

    /* How to validate this actually worked? */

    Array_Release(Outer);
    TEST_PRINTF("%s", "Test Successful.");
    return 0;
}
