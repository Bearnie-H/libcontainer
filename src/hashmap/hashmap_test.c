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
#include "include/hashmap.h"

/*
    Temporary function for visualizing the distribution properties of the hash function used.

static void PrintHashmapBucketUtilization(Hashmap_t *Map) {

    size_t BucketCount = 0, i = 0;
    List_t *Bucket = NULL;

    printf("Hashmap Bucket Lengths: [ ");
    BucketCount = Array_Length(Map->Buckets);
    for (i = 0; i < BucketCount; i++) {
        Bucket = (List_t *)Array_GetElement(Map->Buckets, i);
        printf("%ld ", (unsigned long)List_Length(Bucket));
    }
    printf("]\n");

    return;
}

*/

int Test_hashmap(void) {

    int FailedTests = 0;

    FailedTests += Test_hashmap_entry();

    FailedTests += Test_Hashmap_DoubleKey();
    FailedTests += Test_Hashmap_StringKey();

    FailedTests += Test_hashmap_callbacks();

    FailedTests += Test_hashmap_iterator();

    return FailedTests;
}

int Test_Hashmap_DoubleKey(void) {

    int FailedTests = 0;

    FailedTests += Test_Hashmap_Create_DoubleKey();
    FailedTests += Test_Hashmap_Insert_DoubleKey();
    FailedTests += Test_Hashmap_Overwrite_DoubleKey();
    FailedTests += Test_Hashmap_Retrieve_DoubleKey();
    FailedTests += Test_Hashmap_KeyExists_DoubleKey();
    FailedTests += Test_Hashmap_Remove_DoubleKey();
    FailedTests += Test_Hashmap_Pop_DoubleKey();
    FailedTests += Test_Hashmap_Clear_DoubleKey();

    return FailedTests;
}

int Test_Hashmap_StringKey(void) {

    int FailedTests = 0;

    FailedTests += Test_Hashmap_Create_StringKey();
    FailedTests += Test_Hashmap_Insert_StringKey();
    FailedTests += Test_Hashmap_Overwrite_StringKey();
    FailedTests += Test_Hashmap_Retrieve_StringKey();
    FailedTests += Test_Hashmap_KeyExists_StringKey();
    FailedTests += Test_Hashmap_Remove_StringKey();
    FailedTests += Test_Hashmap_Pop_StringKey();
    FailedTests += Test_Hashmap_Clear_StringKey();

    return FailedTests;
}

int Test_Hashmap_Create_DoubleKey(void) {

    Hashmap_t *Map = NULL;
    /* Declare Variables. */

    Map = Hashmap_Create(HashFunc_Double, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Insert_DoubleKey(void) {

    Hashmap_t *Map = NULL;
    double KeyValue = 3.1415, TempKey = 0.0;
    int Value = 0xBEEF, TempValue = 0;
    int Count = 256, i = 0;

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

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Overwrite_DoubleKey(void) {

    Hashmap_t *Map = NULL;
    double KeyValue = 3.1415;
    int InitialValue = 0xBEEF, NewValue = 0xCAFE;
    int *CheckValue = NULL;

    Map = Hashmap_Create(HashFunc_Double, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    if (0 != Hashmap_Insert(Map, &KeyValue, &InitialValue, 0, sizeof(InitialValue), NULL)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform first Insert.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (0 != Hashmap_Insert(Map, &KeyValue, &NewValue, 0, sizeof(NewValue), NULL)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform second Insert.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    CheckValue = (int *)Hashmap_Retrieve(Map, &KeyValue, 0);
    if (NULL == CheckValue) {
        TEST_PRINTF("%s", "Test Failure - Failed to retrieve updated value.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (*CheckValue != NewValue) {
        TEST_PRINTF("Test Failure - Retrieved value (%d) not equal to expectation (%d).",
                    *CheckValue, NewValue);
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Retrieve_DoubleKey(void) {

    Hashmap_t *Map = NULL;
    double KeyValue = 3.1415, TempKey = 0.0;
    int Value = 0xBEEF, TempValue = 0;
    int Count = 256, i = 0;
    int *RetrievedValue = NULL;

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

    for (i = 0; i < Count; i++) {
        TempKey = i * KeyValue;
        TempValue = i * Value;
        RetrievedValue = (int *)Hashmap_Retrieve(Map, &TempKey, 0);
        if (NULL == RetrievedValue) {
            TEST_PRINTF("%s", "Test Failure - Failed to perform Retrieve operation.");
            Hashmap_Release(Map);
            TEST_FAILURE;
        }
        if (*RetrievedValue != TempValue) {
            TEST_PRINTF("Test Failure - Retrieved Value (%d) not equal to original (%d).",
                        *RetrievedValue, Value);
            Hashmap_Release(Map);
            TEST_FAILURE;
        }
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_KeyExists_DoubleKey(void) {

    Hashmap_t *Map = NULL;
    double ValidKey = 3.1415, InvalidKey = 2.71828;
    int Value = 0xCAFE;

    Map = Hashmap_Create(HashFunc_Double, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    if (0 != Hashmap_Insert(Map, &ValidKey, &Value, 0, sizeof(Value), NULL)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Insert operation.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (!Hashmap_KeyExists(Map, &ValidKey, 0)) {
        TEST_PRINTF("%s", "Test Failure - Inserted key not found in Hashmap.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (Hashmap_KeyExists(Map, &InvalidKey, 0)) {
        TEST_PRINTF("%s", "Test Failure - Key found in Hashmap which was never inserted.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Remove_DoubleKey(void) {

    Hashmap_t *Map = NULL;
    double KeyValue = 3.1415;
    int Value = 0xBEEF;

    Map = Hashmap_Create(HashFunc_Double, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    if (0 != Hashmap_Insert(Map, &KeyValue, &Value, 0, sizeof(Value), NULL)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Insert operation.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (0 != Hashmap_Remove(Map, &KeyValue, 0)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Remove operation.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (Hashmap_KeyExists(Map, &KeyValue, 0)) {
        TEST_PRINTF("%s", "Test Failure - Removed key found in Hashmap.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Pop_DoubleKey(void) {

    Hashmap_t *Map = NULL;
    double KeyValue = 3.1415;
    int Value = 0xCAFE;
    int *PoppedValue = NULL;

    Map = Hashmap_Create(HashFunc_Double, sizeof(double), NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    if (0 != Hashmap_Insert(Map, &KeyValue, &Value, 0, sizeof(Value), NULL)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Insert.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    PoppedValue = (int *)Hashmap_Pop(Map, &KeyValue, 0);
    if (NULL == PoppedValue) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Pop.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (*PoppedValue != Value) {
        TEST_PRINTF("Test Failure - Popped Value (%d) not equal to original value (%d).",
                    *PoppedValue, Value);
        Hashmap_Release(Map);
        free(PoppedValue);
        TEST_FAILURE;
    }

    if (0 != Hashmap_Length(Map)) {
        TEST_PRINTF("Test Failure - Hashmap Length (%ld) not equal to expectation (%d).",
                    (unsigned long)Hashmap_Length(Map), 0);
        Hashmap_Release(Map);
        free(PoppedValue);
        TEST_FAILURE;
    }

    free(PoppedValue);
    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Clear_DoubleKey(void) {

    Hashmap_t *Map = NULL;
    double KeyValue = 3.1415, TempKey = 0.0;
    int Value = 0xBEEF, TempValue = 0;
    int Count = 256, i = 0;

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

    if (0 != Hashmap_Clear(Map)) {
        TEST_PRINTF("%s", "Test Failure - Failed to clear Hashmap_t.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (0 != Hashmap_Length(Map)) {
        TEST_PRINTF("%s", "Test Failure - Hashmap Length not equal to 0 after being cleared.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Create_StringKey(void) {

    Hashmap_t *Map = NULL;

    Map = Hashmap_Create(NULL, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Insert_StringKey(void) {

    Hashmap_t *Map = NULL;
    char KeyValue[16] = {0x00};
    int Value = 0xBEEF, TempValue = 0, Count = 256, i = 0;

    Map = Hashmap_Create(NULL, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        snprintf(KeyValue, sizeof(KeyValue) - 1, "Key %d", i);
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

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Overwrite_StringKey(void) {

    Hashmap_t *Map = NULL;
    char KeyValue[] = "Overwrite Key";
    int InitialValue = 0xBEEF, NewValue = 0xCAFE;
    int *CheckValue = NULL;

    Map = Hashmap_Create(NULL, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    if (0 != Hashmap_Insert(Map, KeyValue, &InitialValue, strlen(KeyValue), sizeof(InitialValue),
                            NULL)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform first Insert.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (0 != Hashmap_Insert(Map, KeyValue, &NewValue, strlen(KeyValue), sizeof(NewValue), NULL)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform second Insert.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    CheckValue = (int *)Hashmap_Retrieve(Map, KeyValue, 0);
    if (NULL == CheckValue) {
        TEST_PRINTF("%s", "Test Failure - Failed to retrieve updated value.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (*CheckValue != NewValue) {
        TEST_PRINTF("Test Failure - Retrieved value (%d) not equal to expectation (%d).",
                    *CheckValue, NewValue);
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Retrieve_StringKey(void) {

    Hashmap_t *Map = NULL;
    char KeyValue[16] = {0x00};
    int Value = 0xBEEF, TempValue = 0, Count = 256, i = 0;
    int *RetrievedValue = NULL;

    Map = Hashmap_Create(NULL, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        snprintf(KeyValue, sizeof(KeyValue) - 1, "Key %d", i);
        TempValue = Value * i;
        if (0 !=
            Hashmap_Insert(Map, KeyValue, &TempValue, strlen(KeyValue), sizeof(TempValue), NULL)) {
            TEST_PRINTF("Test Failure - Failed to insert key (%s) and value (%d) to Hashmap",
                        KeyValue, TempValue);
            Hashmap_Release(Map);
            TEST_FAILURE;
        }
    }

    for (i = 0; i < Count; i++) {
        snprintf(KeyValue, sizeof(KeyValue) - 1, "Key %d", i);
        TempValue = Value * i;
        RetrievedValue = Hashmap_Retrieve(Map, KeyValue, 0);
        if (NULL == RetrievedValue) {
            TEST_PRINTF("%s", "Test Failure - Failed to perform Retrieve operation.");
            Hashmap_Release(Map);
            TEST_FAILURE;
        }
        if (*RetrievedValue != TempValue) {
            TEST_PRINTF("Test Failure - Retrieved Value (%d) not equal to original (%d).",
                        *RetrievedValue, Value);
            Hashmap_Release(Map);
            TEST_FAILURE;
        }
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_KeyExists_StringKey(void) {

    Hashmap_t *Map = NULL;
    char ValidKey[] = "ValidKey", InvalidKey[] = "InvalidKey";
    int Value = 0xCAFE;

    Map = Hashmap_Create(NULL, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    if (0 != Hashmap_Insert(Map, ValidKey, &Value, strlen(ValidKey), sizeof(Value), NULL)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Insert operation.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (!Hashmap_KeyExists(Map, ValidKey, 0)) {
        TEST_PRINTF("%s", "Test Failure - Inserted key not found in Hashmap.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (Hashmap_KeyExists(Map, InvalidKey, 0)) {
        TEST_PRINTF("%s", "Test Failure - Key found in Hashmap which was never inserted.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Remove_StringKey(void) {

    Hashmap_t *Map = NULL;
    char KeyValue[] = "Test Key";
    int Value = 0xCAFE;

    Map = Hashmap_Create(NULL, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    if (0 != Hashmap_Insert(Map, KeyValue, &Value, strlen(KeyValue), sizeof(Value), NULL)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Insert operation.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (0 != Hashmap_Remove(Map, KeyValue, 0)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Remove operation.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (Hashmap_KeyExists(Map, KeyValue, 0)) {
        TEST_PRINTF("%s", "Test Failure - Removed key found in Hashmap.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Pop_StringKey(void) {

    Hashmap_t *Map = NULL;
    char KeyValue[] = "Pop Test";
    int Value = 0xCAFE;
    int *PoppedValue = NULL;

    Map = Hashmap_Create(NULL, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    if (0 != Hashmap_Insert(Map, KeyValue, &Value, strlen(KeyValue), sizeof(Value), NULL)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Insert.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    PoppedValue = (int *)Hashmap_Pop(Map, KeyValue, 0);
    if (NULL == PoppedValue) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Pop.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (*PoppedValue != Value) {
        TEST_PRINTF("Test Failure - Popped Value (%d) not equal to original value (%d).",
                    *PoppedValue, Value);
        Hashmap_Release(Map);
        free(PoppedValue);
        TEST_FAILURE;
    }

    if (0 != Hashmap_Length(Map)) {
        TEST_PRINTF("Test Failure - Hashmap Length (%ld) not equal to expectation (%d).",
                    (unsigned long)Hashmap_Length(Map), 0);
        Hashmap_Release(Map);
        free(PoppedValue);
        TEST_FAILURE;
    }

    free(PoppedValue);
    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Clear_StringKey(void) {

    Hashmap_t *Map = NULL;
    char KeyValue[16] = {0x00};
    int Value = 0xBEEF, TempValue = 0, Count = 256, i = 0;

    Map = Hashmap_Create(NULL, 0, NULL);
    if (NULL == Map) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        snprintf(KeyValue, sizeof(KeyValue) - 1, "Key %d", i);
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

    if (0 != Hashmap_Clear(Map)) {
        TEST_PRINTF("%s", "Test Failure - Failed to clear Hashmap_t.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    if (0 != Hashmap_Length(Map)) {
        TEST_PRINTF("%s", "Test Failure - Hashmap Length not equal to 0 after being cleared.");
        Hashmap_Release(Map);
        TEST_FAILURE;
    }

    Hashmap_Release(Map);
    TEST_SUCCESSFUL;
}
