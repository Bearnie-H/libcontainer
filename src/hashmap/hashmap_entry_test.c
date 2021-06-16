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
#include "include/hashmap_entry.h"

int Test_hashmap_entry(void) {

    int FailedTests = 0;

    FailedTests += Test_Hashmap_Entry_Create();
    FailedTests += Test_Hashmap_Entry_Update();

    return FailedTests;
}

int Test_Hashmap_Entry_Create(void) {

    Hashmap_Entry_t *Entry = NULL;
    double Key = 3.1415;
    int Value = 0xCAFE;

    Entry = Hashmap_Entry_Create(&Key, &Value, sizeof(Key), sizeof(Value), 1, NULL, NULL);
    if (NULL == Entry) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_Entry_t for testing.");
        TEST_FAILURE;
    }

    Hashmap_Entry_Release(Entry);
    TEST_SUCCESSFUL;
}

int Test_Hashmap_Entry_Update(void) {

    Hashmap_Entry_t *Entry = NULL;
    double Key = 3.1415;
    int Value = 0xCAFE;
    double NewValue = 2.71828;
    double Epsilon = 0.000001;

    Entry = Hashmap_Entry_Create(&Key, &Value, sizeof(Key), sizeof(Value), 1, NULL, NULL);
    if (NULL == Entry) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Hashmap_Entry_t for testing.");
        TEST_FAILURE;
    }

    if (0 != Hashmap_Entry_Update(Entry, &NewValue, sizeof(NewValue), NULL)) {
        TEST_PRINTF("%s", "Test Failure - Failed to update Hashmap_Entry_t to new value.");
        Hashmap_Entry_Release(Entry);
        TEST_FAILURE;
    }

    if ((*(double *)(Entry->Value.ValueRaw) - NewValue) > Epsilon) {
        TEST_PRINTF("Test Failure - Hashmap_Entry_t Value (%f) not equal to expectation (%f).",
                    *(double *)(Entry->Value.ValueRaw), NewValue);
        Hashmap_Entry_Release(Entry);
        TEST_FAILURE;
    }

    Hashmap_Entry_Release(Entry);
    TEST_SUCCESSFUL;
}
