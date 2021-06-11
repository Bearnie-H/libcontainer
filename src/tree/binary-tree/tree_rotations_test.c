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
#include "include/tree_rotations.h"

int Test_tree_rotations(void) {

    int FailedTests = 0;

    FailedTests += Test_Binary_Tree_rotateLeft();
    FailedTests += Test_Binary_Tree_rotateRight();

    return FailedTests;
}

int Test_Binary_Tree_rotateLeft() {

    size_t i = 0, j = 0;
    Binary_Tree_Node_t *Nodes[3] = {NULL, NULL, NULL};
    int Keys[3] = {1, 2, 3}, Values[3] = {10, 20, 30};

    for (i = 0; i < 3; i++) {
        Nodes[i] = Binary_Tree_Node_Create(Keys[i], sizeof(Values[i]), &(Values[i]), NULL);
        if (NULL == Nodes[i]) {
            TEST_PRINTF("%s", "Test Failure - Failed to create Nodes for testing.");
            for (j = 0; j < 3; j++) {
                Binary_Tree_Node_Release(Nodes[j]);
            }
            TEST_FAILURE;
        }
    }

    Nodes[0]->RightChild = Nodes[1];
    Nodes[1]->RightChild = Nodes[2];
    Nodes[1]->Parent = Nodes[0];
    Nodes[2]->Parent = Nodes[1];

    Nodes[0] = Binary_Tree_rotateLeft(Nodes[0]);

    if (2 != Nodes[0]->Key) {
        TEST_PRINTF("%s", "Test Failure - New Root key not equal to expectation.");
        Binary_Tree_Node_Release(Nodes[0]);
        TEST_FAILURE;
    }

    if (1 != Nodes[0]->LeftChild->Key) {
        TEST_PRINTF("%s", "Test Failure - New Left Child key not equal to expectation.");
        Binary_Tree_Node_Release(Nodes[0]);
        TEST_FAILURE;
    }

    if (3 != Nodes[0]->RightChild->Key) {
        TEST_PRINTF("%s", "Test Failure - New Right Child key not equal to expectation.");
        Binary_Tree_Node_Release(Nodes[0]);
        TEST_FAILURE;
    }

    Binary_Tree_Node_Release(Nodes[0]);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_rotateRight() {

    size_t i = 0, j = 0;
    Binary_Tree_Node_t *Nodes[3] = {NULL, NULL, NULL};
    int Keys[3] = {3, 2, 1}, Values[3] = {10, 20, 30};

    for (i = 0; i < 3; i++) {
        Nodes[i] = Binary_Tree_Node_Create(Keys[i], sizeof(Values[i]), &(Values[i]), NULL);
        if (NULL == Nodes[i]) {
            TEST_PRINTF("%s", "Test Failure - Failed to create Nodes for testing.");
            for (j = 0; j < 3; j++) {
                Binary_Tree_Node_Release(Nodes[j]);
            }
            TEST_FAILURE;
        }
    }

    Nodes[0]->LeftChild = Nodes[1];
    Nodes[1]->LeftChild = Nodes[2];
    Nodes[1]->Parent = Nodes[0];
    Nodes[2]->Parent = Nodes[1];

    Nodes[0] = Binary_Tree_rotateRight(Nodes[0]);

    if (2 != Nodes[0]->Key) {
        TEST_PRINTF("%s", "Test Failure - New Root key not equal to expectation.");
        Binary_Tree_Node_Release(Nodes[0]);
        TEST_FAILURE;
    }

    if (1 != Nodes[0]->LeftChild->Key) {
        TEST_PRINTF("%s", "Test Failure - New Left Child key not equal to expectation.");
        Binary_Tree_Node_Release(Nodes[0]);
        TEST_FAILURE;
    }

    if (3 != Nodes[0]->RightChild->Key) {
        TEST_PRINTF("%s", "Test Failure - New Right Child key not equal to expectation.");
        Binary_Tree_Node_Release(Nodes[0]);
        TEST_FAILURE;
    }

    Binary_Tree_Node_Release(Nodes[0]);
    TEST_SUCCESSFUL;
}
