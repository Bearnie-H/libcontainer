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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../logging/logging.h"
#include "include/binary_tree.h"
#include "include/tree_rotations.h"

/* Temporary Function for printing Binary Trees.
static void PrettyPrintBinaryTree(const char *Prefix, const Binary_Tree_Node_t *Root, bool IsLeft);
static void PrintBinaryTree(const Binary_Tree_Node_t *Root);

static void PrettyPrintBinaryTree(const char *Prefix, const Binary_Tree_Node_t *Root, bool IsLeft) {

    char NextLeftPrefix[64] = {0x00};
    char NextRightPrefix[64] = {0x00};

    if (NULL != Root) {

        printf("%s", Prefix);

        printf("%s", (IsLeft) ? ("├──") : ("└──"));

        printf("(%d,%d)\n", Root->Key, *(int *)(Root->Value.ValueBytes));

        snprintf(NextLeftPrefix, sizeof(NextLeftPrefix), "%s%s", Prefix,
                 (IsLeft) ? ("│   ") : ("    "));
        snprintf(NextRightPrefix, sizeof(NextRightPrefix), "%s%s", Prefix,
                 (IsLeft) ? ("│   ") : ("    "));

        PrettyPrintBinaryTree(NextLeftPrefix, Root->LeftChild, true);
        PrettyPrintBinaryTree(NextRightPrefix, Root->RightChild, false);
    }
}

static void PrintBinaryTree(const Binary_Tree_Node_t *Root) {

    if (NULL != Root) {
        PrintBinaryTree(Root->LeftChild);
        printf("%d ", Root->Key);
        PrintBinaryTree(Root->RightChild);
    }
}

*/

static int PrintBinaryTreeCallback(void *KeyValuePair) {

    int *Key = NULL, *Value = NULL;

    if (NULL == KeyValuePair) {
    }

    if (NULL == KeyValuePair) {
        TEST_PRINTF("%s", "Test Failure - NULL KeyValuePair* provided.");
        TEST_FAILURE;
    }

    Key = ((int *)((void **)KeyValuePair)[0]);
    Value = ((int *)((void **)KeyValuePair)[1]);

    if (NULL == Key) {
        TEST_PRINTF("%s", "Test Failure - NULL Key* provided.");
        TEST_FAILURE;
    }

    if (NULL == Value) {
        TEST_PRINTF("%s", "Test Failure - NULL Value* provided.");
        TEST_FAILURE;
    }

    DEBUG_PRINTF("(%d,%d) ", *Key, *Value);

    return 0;
}

static int PrintBinaryTreeCallbackArgs(void *KeyValuePair, void *Args) {

    if (NULL == Args) {
        DEBUG_PRINTF("%s", "Note: NULL Args* provided.");
    }

    int *Key = NULL, *Value = NULL;

    if (NULL == KeyValuePair) {
    }

    if (NULL == KeyValuePair) {
        TEST_PRINTF("%s", "Test Failure - NULL KeyValuePair* provided.");
        TEST_FAILURE;
    }

    Key = ((int *)((void **)KeyValuePair)[0]);
    Value = ((int *)((void **)KeyValuePair)[1]);

    if (NULL == Key) {
        TEST_PRINTF("%s", "Test Failure - NULL Key* provided.");
        TEST_FAILURE;
    }

    if (NULL == Value) {
        TEST_PRINTF("%s", "Test Failure - NULL Value* provided.");
        TEST_FAILURE;
    }

    DEBUG_PRINTF("(%d,%d) ", *Key, *Value);

    return 0;
}

int Test_binary_tree(void) {

    int FailedTests = 0;

    FailedTests += Test_binary_tree_node();
    FailedTests += Test_tree_rotations();

    FailedTests += Test_Binary_Tree_Create();
    FailedTests += Test_Binary_Tree_Insert();
    FailedTests += Test_Binary_Tree_Get();
    FailedTests += Test_Binary_Tree_Pop();
    FailedTests += Test_Binary_Tree_DoCallback();
    FailedTests += Test_Binary_Tree_DoCallbackArg();
    FailedTests += Test_Binary_Tree_Remove();
    FailedTests += Test_Binary_Tree_Balancing();
    FailedTests += Test_Binary_Tree_Clear();

    return FailedTests;
}

int Test_Binary_Tree_Create() {

    Binary_Tree_t *Tree = NULL;

    Tree = Binary_Tree_Create(sizeof(int), NULL);
    if (NULL == Tree) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Tree_t* for testing.");
        TEST_FAILURE;
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_Insert() {

    Binary_Tree_t *Tree = NULL;
    size_t Count = 16, i = 0;
    int Key = 0, Value = 0, Modulo = 1000;

    Tree = Binary_Tree_Create(sizeof(Value), NULL);
    if (NULL == Tree) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Tree_t* for testing.");
        TEST_FAILURE;
    }

    for (i = Count; i > 0; i--) {

        Key = (int)i;
        Value = rand() % Modulo;

        if (0 != Binary_Tree_Insert(Tree, Key, &Value)) {
            TEST_PRINTF(
                "Test Failure - Failed to insert Key (%d) Value (%d) pair into Tree (item #%lu).",
                Key, Value, (unsigned long)i);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    /* This test is fully validated by the successful execution of the Get, Pop, and Remove tests.
     */

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_Get() {

    Binary_Tree_t *Tree = NULL;
    size_t Count = 32, i = 0;
    int Key = 0, Value = 0, *CheckValue = NULL;

    Tree = Binary_Tree_Create(sizeof(Value), NULL);
    if (NULL == Tree) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Tree_t* for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {

        Key = (int)i;
        Value = (int)i * 2;

        if (0 != Binary_Tree_Insert(Tree, Key, &Value)) {
            TEST_PRINTF(
                "Test Failure - Failed to insert Key (%d) Value (%d) pair into Tree (item #%lu).",
                Key, Value, (unsigned long)i);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    for (i = 0; i < Count; i++) {
        Key = (int)i;
        Value = (int)i * 2;

        CheckValue = (int *)Binary_Tree_Get(Tree, Key);
        if (NULL == CheckValue) {
            TEST_PRINTF("Test Failure - Failed to get item with Key [ %d ] from Tree.", Key);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }

        if (*CheckValue != Value) {
            TEST_PRINTF("Test Failure - Item with Key [ %d ] has unexpected value, got [ %d ] but "
                        "expected [ %d ] ",
                        Key, *CheckValue, Value);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_Pop() {

    Binary_Tree_t *Tree = NULL;
    size_t Count = 32, i = 0;
    int Key = 0, Value = 0, *CheckValue = NULL;

    Tree = Binary_Tree_Create(sizeof(Value), NULL);
    if (NULL == Tree) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Tree_t* for testing.");
        TEST_FAILURE;
    }

    while (Binary_Tree_Length(Tree) != Count) {

        Key = rand() % (int)Count;
        Value = Key * 2;

        if (0 != Binary_Tree_Insert(Tree, Key, &Value)) {
            TEST_PRINTF(
                "Test Failure - Failed to insert Key (%d) Value (%d) pair into Tree (item #%lu).",
                Key, Value, (unsigned long)i);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    while (0 != Binary_Tree_Length(Tree)) {

        Key = rand() % (int)Count;
        if (!Binary_Tree_KeyExists(Tree, Key)) {
            continue;
        }
        Value = Key * 2;

        CheckValue = (int *)Binary_Tree_Pop(Tree, Key);
        if (NULL == CheckValue) {
            TEST_PRINTF("Test Failure - Failed to get item with Key [ %d ] from Tree.", Key);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }

        if (*CheckValue != Value) {
            TEST_PRINTF("Test Failure - Item with Key [ %d ] has unexpected value, got [ %d ] but "
                        "expected [ %d ] ",
                        Key, *CheckValue, Value);
            Binary_Tree_Release(Tree);
            free(CheckValue);
            TEST_FAILURE;
        }
        free(CheckValue);
    }

    if (NULL != Tree->Root) {
        TEST_PRINTF("%s", "Test Failure - Empty Tree has non-NULL Root.");
        Binary_Tree_Release(Tree);
        TEST_FAILURE;
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_DoCallback() {

    Binary_Tree_t *Tree = NULL;
    size_t Count = 32, i = 0;
    int Key = 0, Value = 0, Modulo = 1000;

    Tree = Binary_Tree_Create(sizeof(Value), NULL);
    if (NULL == Tree) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Tree_t* for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {

        Key = (int)i;
        Value = rand() % Modulo;

        if (0 != Binary_Tree_Insert(Tree, Key, &Value)) {
            TEST_PRINTF(
                "Test Failure - Failed to insert Key (%d) Value (%d) pair into Tree (item #%lu).",
                Key, Value, (unsigned long)i);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    if (0 != Binary_Tree_DoCallback(Tree, PrintBinaryTreeCallback)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform InOrder Binary_Tree_DoCallback().");
        Binary_Tree_Release(Tree);
        TEST_FAILURE;
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_DoCallbackArg() {

    Binary_Tree_t *Tree = NULL;
    size_t Count = 32, i = 0;
    int Key = 0, Value = 0, Modulo = 1000;

    Tree = Binary_Tree_Create(sizeof(Value), NULL);
    if (NULL == Tree) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Tree_t* for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {

        Key = (int)i;
        Value = rand() % Modulo;

        if (0 != Binary_Tree_Insert(Tree, Key, &Value)) {
            TEST_PRINTF(
                "Test Failure - Failed to insert Key (%d) Value (%d) pair into Tree (item #%lu).",
                Key, Value, (unsigned long)i);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    if (0 != Binary_Tree_DoCallbackArg(Tree, PrintBinaryTreeCallbackArgs, NULL)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform InOrder Binary_Tree_DoCallbackArg().");
        Binary_Tree_Release(Tree);
        TEST_FAILURE;
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_Remove() {

    Binary_Tree_t *Tree = NULL;
    size_t Count = 32, i = 0;
    int Key = 0, Value = 0;

    Tree = Binary_Tree_Create(sizeof(Value), NULL);
    if (NULL == Tree) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Tree_t* for testing.");
        TEST_FAILURE;
    }

    while (Binary_Tree_Length(Tree) != Count) {

        Key = rand() % (int)Count;
        Value = (int)i * 2;

        if (0 != Binary_Tree_Insert(Tree, Key, &Value)) {
            TEST_PRINTF(
                "Test Failure - Failed to insert Key (%d) Value (%d) pair into Tree (item #%lu).",
                Key, Value, (unsigned long)i);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    while (0 != Binary_Tree_Length(Tree)) {

        Key = rand() % (int)Count;
        if (!Binary_Tree_KeyExists(Tree, Key)) {
            continue;
        }

        if (0 != Binary_Tree_Remove(Tree, Key)) {
            TEST_PRINTF("Test Failure - Failed to remove item with Key [ %d ] from Tree.", Key);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }

        if (Binary_Tree_KeyExists(Tree, Key)) {
            TEST_PRINTF("Test Failure - Key [ %d ] found in Tree after removal.", Key);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    if (NULL != Tree->Root) {
        TEST_PRINTF("%s", "Test Failure - Empty Tree has non-NULL Root.");
        Binary_Tree_Release(Tree);
        TEST_FAILURE;
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_Balancing() {

    Binary_Tree_t *Tree = NULL;
    size_t OperationCount = 1 << 10, i = 0;
    int Key = 0, Value = 0, OperationType = 0;

    Tree = Binary_Tree_Create(sizeof(Value), NULL);
    if (NULL == Tree) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Tree_t* for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < OperationCount; i++) {
        OperationType = rand() & 0x01;
        if (0 == OperationType) {
            /* Do Insert operation. */
            Key = rand();
            Value = rand();
            if (0 != Binary_Tree_Insert(Tree, Key, &Value)) {
                TEST_PRINTF("Test Failure - Failed to insert Key (%d) Value (%d) pair into Tree.",
                            Key, Value);
                Binary_Tree_Release(Tree);
                TEST_FAILURE;
            }
            if (!Binary_Tree_KeyExists(Tree, Key)) {
                TEST_PRINTF("Test Failure - Insertion of item with Key [ %d ] reported success, "
                            "but failed to be found in Tree.",
                            Key);
                Binary_Tree_Release(Tree);
                TEST_FAILURE;
            }
        } else {
            /* Do Remove operation. */
            Key = rand();
            if (0 != Binary_Tree_Remove(Tree, Key)) {
                TEST_PRINTF("Test Failure - Error while attempting to remove item with Key [ %d ] "
                            "from Tree.",
                            Key);
                Binary_Tree_Release(Tree);
                TEST_FAILURE;
            }
            if (Binary_Tree_KeyExists(Tree, Key)) {
                TEST_PRINTF("Test Failure - Removal of item with Key [ %d ] reported success, but "
                            "key was found in Tree.",
                            Key);
                Binary_Tree_Release(Tree);
                TEST_FAILURE;
            }
        }
        if (!Binary_Tree_isAVLTree(Tree->Root)) {
            TEST_PRINTF("%s", "Test Failure - Tree failed to satisfy AVL criteria.");
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_Clear(void) {

    Binary_Tree_t *Tree = NULL;
    size_t Count = 16, i = 0;
    int Key = 0, Value = 0, Modulo = 1000;

    Tree = Binary_Tree_Create(sizeof(Value), NULL);
    if (NULL == Tree) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary_Tree_t* for testing.");
        TEST_FAILURE;
    }

    for (i = Count; i > 0; i--) {

        Key = (int)i;
        Value = rand() % Modulo;

        if (0 != Binary_Tree_Insert(Tree, Key, &Value)) {
            TEST_PRINTF(
                "Test Failure - Failed to insert Key (%d) Value (%d) pair into Tree (item #%lu).",
                Key, Value, (unsigned long)i);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    if (0 != Binary_Tree_Clear(Tree)) {
        TEST_PRINTF("%s", "Test Failure - Failed to clear Binary tree.");
        Binary_Tree_Release(Tree);
        TEST_FAILURE;
    }

    if (0 != Binary_Tree_Length(Tree)) {
        TEST_PRINTF("%s",
                    "Test Failure - Binary Tree contains non-zero items after being cleared.");
        Binary_Tree_Release(Tree);
        TEST_FAILURE;
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}
