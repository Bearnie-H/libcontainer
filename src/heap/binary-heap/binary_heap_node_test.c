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

#include "../../logging/logging.h"
#include "include/binary_heap_node.h"

int Test_binary_heap_node(void) {

    int FailedTests = 0;

    FailedTests += Test_Binary_Heap_Node_Create();
    FailedTests += Test_Binary_Heap_Node_UpdateKey();
    FailedTests += Test_Binary_Heap_Node_UpdateValue();

    return FailedTests;
}

int Test_Binary_Heap_Node_Create(void) {

    Binary_Heap_Node_t *Node    = NULL;
    int                 Key     = 0xBEEF;
    char                Value[] = "Testing Value";

    Node = Binary_Heap_Node_Create(&Key, Value, sizeof(Key), strlen(Value), NULL, NULL);
    if ( NULL == Node ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Heap_Node_t for testing.");
        TEST_FAILURE;
    }

    Binary_Heap_Node_Release(Node);
    TEST_SUCCESSFUL;
}

int Test_Binary_Heap_Node_UpdateKey(void) {

    Binary_Heap_Node_t *Node = NULL;
    int                 Key = 0xBEEF, NewKey = 0xCAFE;
    char                Value[] = "Testing Value";

    Node = Binary_Heap_Node_Create(&Key, Value, sizeof(Key), strlen(Value), NULL, NULL);
    if ( NULL == Node ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Heap_Node_t for testing.");
        TEST_FAILURE;
    }

    if ( 0 != Binary_Heap_Node_UpdateKey(Node, &NewKey, sizeof(NewKey)) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to update the Key for a Binary_Heap_Node_t.");
        Binary_Heap_Node_Release(Node);
        TEST_FAILURE;
    }

    Binary_Heap_Node_Release(Node);
    TEST_SUCCESSFUL;
}

int Test_Binary_Heap_Node_UpdateValue(void) {

    Binary_Heap_Node_t *Node    = NULL;
    int                 Key     = 0xBEEF;
    char                Value[] = "Testing Value", NewValue[] = "Updated Value";

    Node = Binary_Heap_Node_Create(&Key, Value, sizeof(Key), strlen(Value), NULL, NULL);
    if ( NULL == Node ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Heap_Node_t for testing.");
        TEST_FAILURE;
    }

    if ( 0 != Binary_Heap_Node_UpdateKey(Node, NewValue, strlen(NewValue)) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to update the Value for a Binary_Heap_Node_t.");
        Binary_Heap_Node_Release(Node);
        TEST_FAILURE;
    }

    Binary_Heap_Node_Release(Node);
    TEST_SUCCESSFUL;
}
