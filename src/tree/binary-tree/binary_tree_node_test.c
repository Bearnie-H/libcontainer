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

#include "../../logging/logging.h"
#include "include/binary_tree_node.h"

int Test_binary_tree_node(void) {

    int FailedTests = 0;

    FailedTests += Test_Binary_Tree_Node_Create();
    FailedTests += Test_Binary_Tree_Node_Update();

    return FailedTests;
}

int Test_Binary_Tree_Node_Create(void) {

    Binary_Tree_Node_t *Node = NULL;
    int                 Key = 0xBEEF, Value = 0xCAFE;

    Node = Binary_Tree_Node_Create(Key, sizeof(Value), &Value, NULL);
    if ( NULL == Node ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create new Binary_Tree_Node_t for testing.");
        TEST_FAILURE;
    }

    Binary_Tree_Node_Release(Node);
    TEST_SUCCESSFUL;
}
int Test_Binary_Tree_Node_Update(void) {

    Binary_Tree_Node_t *Node = NULL;
    int                 Key = 0xBEEF, Value = 0xCAFE, NewValue = 0xBABE;

    Node = Binary_Tree_Node_Create(Key, sizeof(Value), &Value, NULL);
    if ( NULL == Node ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create new Binary_Tree_Node_t for testing.");
        TEST_FAILURE;
    }

    if ( 0 != Binary_Tree_Node_Update(Node, &NewValue, sizeof(NewValue)) ) {
        TEST_PRINTF("%s", "Test Failure - Failed to update Node value.");
        Binary_Tree_Node_Release(Node);
        TEST_FAILURE;
    }

    if ( 0 != memcmp(Node->Value.ValueBytes, &NewValue, sizeof(NewValue)) ) {
        TEST_PRINTF("Test Failure - Node value (%d) not equal to expected value (%d).",
                    *(int *)(Node->Value.ValueRaw), NewValue);
        Binary_Tree_Node_Release(Node);
        TEST_FAILURE;
    }

    Binary_Tree_Node_Release(Node);
    TEST_SUCCESSFUL;
}
