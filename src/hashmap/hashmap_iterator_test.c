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
#include "include/hashmap.h"

int Test_hashmap_iterator(void) {

    int FailedTests = 0;

    FailedTests += Test_Hashmap_Keys_DoubleKey();
    FailedTests += Test_Hashmap_Keys_StringKey();

    return FailedTests;
}

int Test_Hashmap_Keys_DoubleKey(void) {

    Hashmap_t *Map = NULL;
    Array_t *Keys = NULL;
    double KeyValue = 3.1415, TempKey = 0.0;
    int Value = 0xBEEF, TempValue = 0, Count = 256, i = 0;
    size_t KeyIndex = 0;

    Map = Hashmap_Create(HashFunc_Double, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        TempKey = i * KeyValue;
        TempValue = i * Value;
        if (0 != Hashmap_Insert(Map, &TempKey, &TempValue, 0, sizeof(Value), NULL)) {
            TEST_PRINTF("%s", "Test Failure - Failed to perform Insert operation.");
            Hashmap_Release(Map);
            TEST_FAILURE;
        }
    }

    if ((size_t)Count != Hashmap_Length(Map)) {
        TEST_PRINTF("Test Failure - Hashmap Length (%ld) not equal to expected value (%d).",
                    (unsigned long)Hashmap_Length(Map), 1);
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    Keys = Hashmap_Keys(Map);
    if (NULL == Keys) {
        TEST_PRINTF("%s", "Test Failure - Failed to retrieve Hashmap Keys array.");
        Hashmap_Release(Map);
        Array_Release(Keys);
        TEST_FAILURE;
    }

    for (KeyIndex = 0; KeyIndex < Array_Length(Keys); KeyIndex++) {
        if (!Hashmap_KeyExists(Map, Array_GetElement(Keys, KeyIndex), 0)) {
            TEST_PRINTF("%s", "Test Failure - Key returned in Keys array, but does not report as "
                              "found in Hashmap.");
            Hashmap_Release(Map);
            Array_Release(Keys);
            TEST_FAILURE;
        }
    }

    Hashmap_Release(Map);
    Array_Release(Keys);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Keys_StringKey(void) {

    Hashmap_t *Map = NULL;
    Array_t *Keys = NULL;
    char KeyValue[16] = {0x00};
    int Value = 0xBEEF, TempValue = 0, Count = 256, i = 0;
    size_t KeyIndex = 0;

    Map = Hashmap_Create(NULL, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        snprintf(KeyValue, sizeof(KeyValue), "Key %d", i);
        TempValue = Value * i;
        if (0 !=
            Hashmap_Insert(Map, KeyValue, &TempValue, strlen(KeyValue), sizeof(TempValue), NULL)) {
            TEST_PRINTF("Test Failure - Failed to insert key (%s) and value (%d) to Hashmap",
                        KeyValue, TempValue);
            Hashmap_Release(Map);
            TEST_FAILURE;
        }
    }

    if ((size_t)Count != Hashmap_Length(Map)) {
        TEST_PRINTF("Test Failure - Hashmap Length(%ld) not equal to expected value (%ld)",
                    (unsigned long)Hashmap_Length(Map), (unsigned long)Count);
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    Keys = Hashmap_Keys(Map);
    if (NULL == Keys) {
        TEST_PRINTF("%s", "Test Failure - Failed to retrieve Hashmap Keys array.");
        Hashmap_Release(Map);
        Array_Release(Keys);
        TEST_FAILURE;
    }

    for (KeyIndex = 0; KeyIndex < Array_Length(Keys); KeyIndex++) {
        if (!Hashmap_KeyExists(Map, Array_GetElement(Keys, KeyIndex), 0)) {
            TEST_PRINTF("%s", "Test Failure - Key returned in Keys array, but does not report as "
                              "found in Hashmap.");
            Hashmap_Release(Map);
            Array_Release(Keys);
            TEST_FAILURE;
        }
    }

    Hashmap_Release(Map);
    Array_Release(Keys);
    TEST_SUCCESSFUL;
}
