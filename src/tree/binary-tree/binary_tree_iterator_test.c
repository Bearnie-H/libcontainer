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
#include "include/binary_tree.h"
#include "include/binary_tree_iterator.h"

/*
static void PrintPreOrder(Binary_Tree_Node_t *Node) {
    if (NULL != Node) {
        if (NULL == Node->Parent) {
            printf("Error: NULL Parent for Key (%d).\n", Node->Key);
        }
        printf("%d,", Node->Key);
        PrintPreOrder(Node->LeftChild);
        PrintPreOrder(Node->RightChild);
    }
}

static void PrintPostOrder(Binary_Tree_Node_t *Node) {
    if (NULL != Node) {
        if (NULL == Node->Parent) {
            printf("Error: NULL Parent for Key (%d).\n", Node->Key);
        }
        PrintPostOrder(Node->LeftChild);
        PrintPostOrder(Node->RightChild);
        printf("%d,", Node->Key);
    }
}
*/

static int CompareFunc_Size_t(const void *A, const void *B, __attribute__((unused)) size_t Size) {

    if ( *(const size_t *)A > *(const size_t *)B ) {
        return 1;
    } else if ( *(const size_t *)A < *(const size_t *)B ) {
        return -1;
    }

    return 0;
}

int Test_binary_tree_iterator(void) {

    int FailedTests = 0;

    FailedTests += Test_Binary_Tree_Iterate_Next();
    FailedTests += Test_Binary_Tree_Iterate_Previous();

    return FailedTests;
}

int Test_Binary_Tree_Iterate_Next(void) {

    int FailedTests = 0;

    FailedTests += Test_Binary_Tree_Iterate_Next_InOrder();
    FailedTests += Test_Binary_Tree_Iterate_Next_PreOrder();
    FailedTests += Test_Binary_Tree_Iterate_Next_PostOrder();

    return FailedTests;
}

int Test_Binary_Tree_Iterate_Next_InOrder(void) {

    Binary_Tree_t *            Tree      = NULL;
    size_t                     ItemCount = 16, i = 0;
    Binary_Tree_KeyValuePair_t KeyValuePair = {0, NULL};
    Binary_Tree_Direction_t    Direction    = Direction_InOrder;
    size_t                     Expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    Tree = Binary_Tree_Create((CompareFunc_t *)CompareFunc_Size_t, sizeof(size_t), NULL,
                              Policy_Overwrite);
    if ( NULL == Tree ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary Tree for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < ItemCount; i++ ) {
        if ( 0 != Binary_Tree_Insert(Tree, &i, sizeof(i), &i, sizeof(i), NULL) ) {
            TEST_PRINTF("%s", "Test Failure - Failed to insert item.");
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    for ( i = 0, KeyValuePair = Binary_Tree_Next(Tree, Direction); NULL != KeyValuePair.Value;
          i++, KeyValuePair   = Binary_Tree_Next(Tree, Direction) ) {
        if ( *(size_t *)KeyValuePair.Value != Expected[i] ) {
            TEST_PRINTF("Test Failure - Tree Value (%lu) not equal to expectation (%lu).",
                        (unsigned long)*(size_t *)KeyValuePair.Value, (unsigned long)Expected[i]);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    if ( i != ItemCount ) {
        TEST_PRINTF("Test Failure - Iteration did not pass by all expected items (Count = %ld).",
                    (unsigned long)i);
        Binary_Tree_Release(Tree);
        TEST_FAILURE;
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_Iterate_Next_PreOrder(void) {

    Binary_Tree_t *            Tree      = NULL;
    size_t                     ItemCount = 16, i = 0;
    Binary_Tree_KeyValuePair_t KeyValuePair = {0, NULL};
    Binary_Tree_Direction_t    Direction    = Direction_PreOrder;
    size_t                     Expected[] = {7, 3, 1, 0, 2, 5, 4, 6, 11, 9, 8, 10, 13, 12, 14, 15};

    Tree = Binary_Tree_Create((CompareFunc_t *)CompareFunc_Size_t, sizeof(size_t), NULL,
                              Policy_Overwrite);
    if ( NULL == Tree ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary Tree for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < ItemCount; i++ ) {
        if ( 0 != Binary_Tree_Insert(Tree, &i, sizeof(i), &i, sizeof(i), NULL) ) {
            TEST_PRINTF("%s", "Test Failure - Failed to insert item.");
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    for ( i = 0, KeyValuePair = Binary_Tree_Next(Tree, Direction); NULL != KeyValuePair.Value;
          i++, KeyValuePair   = Binary_Tree_Next(Tree, Direction) ) {

        if ( *(size_t *)KeyValuePair.Value != Expected[i] ) {
            TEST_PRINTF("Test Failure - Tree Value (%lu) not equal to expectation (%lu).",
                        (unsigned long)*(size_t *)KeyValuePair.Value, (unsigned long)Expected[i]);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    if ( i != ItemCount ) {
        TEST_PRINTF("Test Failure - Iteration did not pass by all expected items (Count = %ld).",
                    (unsigned long)i);
        Binary_Tree_Release(Tree);
        TEST_FAILURE;
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_Iterate_Next_PostOrder(void) {

    Binary_Tree_t *            Tree      = NULL;
    size_t                     ItemCount = 16, i = 0;
    Binary_Tree_KeyValuePair_t KeyValuePair = {0, NULL};
    Binary_Tree_Direction_t    Direction    = Direction_PostOrder;
    size_t                     Expected[] = {0, 2, 1, 4, 6, 5, 3, 8, 10, 9, 12, 15, 14, 13, 11, 7};

    Tree = Binary_Tree_Create((CompareFunc_t *)CompareFunc_Size_t, sizeof(size_t), NULL,
                              Policy_Overwrite);
    if ( NULL == Tree ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary Tree for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < ItemCount; i++ ) {
        if ( 0 != Binary_Tree_Insert(Tree, &i, sizeof(i), &i, sizeof(i), NULL) ) {
            TEST_PRINTF("%s", "Test Failure - Failed to insert item.");
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    for ( i = 0, KeyValuePair = Binary_Tree_Next(Tree, Direction); NULL != KeyValuePair.Value;
          i++, KeyValuePair   = Binary_Tree_Next(Tree, Direction) ) {

        if ( *(size_t *)KeyValuePair.Value != Expected[i] ) {
            TEST_PRINTF("Test Failure - Tree Value (%lu) not equal to expectation (%lu).",
                        (unsigned long)*(size_t *)KeyValuePair.Value, (unsigned long)Expected[i]);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    if ( i != ItemCount ) {
        TEST_PRINTF("Test Failure - Iteration did not pass by all expected items (Count = %ld).",
                    (unsigned long)i);
        Binary_Tree_Release(Tree);
        TEST_FAILURE;
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_Iterate_Previous(void) {

    int FailedTests = 0;

    FailedTests += Test_Binary_Tree_Iterate_Previous_InOrder();
    FailedTests += Test_Binary_Tree_Iterate_Previous_PreOrder();
    FailedTests += Test_Binary_Tree_Iterate_Previous_PostOrder();

    return FailedTests;
}

int Test_Binary_Tree_Iterate_Previous_InOrder(void) {

    Binary_Tree_t *            Tree      = NULL;
    size_t                     ItemCount = 16, i = 0;
    Binary_Tree_KeyValuePair_t KeyValuePair = {0, NULL};
    Binary_Tree_Direction_t    Direction    = Direction_InOrder;
    size_t                     Expected[] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    Tree = Binary_Tree_Create((CompareFunc_t *)CompareFunc_Size_t, sizeof(size_t), NULL,
                              Policy_Overwrite);
    if ( NULL == Tree ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary Tree for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < ItemCount; i++ ) {
        if ( 0 != Binary_Tree_Insert(Tree, &i, sizeof(i), &i, sizeof(i), NULL) ) {
            TEST_PRINTF("%s", "Test Failure - Failed to insert item.");
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    for ( i = 0, KeyValuePair = Binary_Tree_Previous(Tree, Direction); NULL != KeyValuePair.Value;
          i++, KeyValuePair   = Binary_Tree_Previous(Tree, Direction) ) {

        if ( (unsigned long)*(size_t *)KeyValuePair.Value != Expected[i] ) {
            TEST_PRINTF("Test Failure - Returned value (%d) not equal to expectation (%d).",
                        (int)*(size_t *)KeyValuePair.Value, (int)Expected[i]);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    if ( i != ItemCount ) {
        TEST_PRINTF("Test Failure - Iterator failed to stop at all items (Count = %lu).",
                    (unsigned long)i);
        Binary_Tree_Release(Tree);
        TEST_FAILURE;
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_Iterate_Previous_PreOrder(void) {

    Binary_Tree_t *            Tree      = NULL;
    size_t                     ItemCount = 16, i = 0;
    Binary_Tree_KeyValuePair_t KeyValuePair = {0, NULL};
    Binary_Tree_Direction_t    Direction    = Direction_PreOrder;
    size_t                     Expected[] = {15, 14, 12, 13, 10, 8, 9, 11, 6, 4, 5, 2, 0, 1, 3, 7};

    Tree = Binary_Tree_Create((CompareFunc_t *)CompareFunc_Size_t, sizeof(size_t), NULL,
                              Policy_Overwrite);
    if ( NULL == Tree ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary Tree for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < ItemCount; i++ ) {
        if ( 0 != Binary_Tree_Insert(Tree, &i, sizeof(i), &i, sizeof(i), NULL) ) {
            TEST_PRINTF("%s", "Test Failure - Failed to insert item.");
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    for ( i = 0, KeyValuePair = Binary_Tree_Previous(Tree, Direction); NULL != KeyValuePair.Value;
          i++, KeyValuePair   = Binary_Tree_Previous(Tree, Direction) ) {

        if ( (unsigned long)*(size_t *)KeyValuePair.Value != Expected[i] ) {
            TEST_PRINTF("Test Failure - Returned value (%d) not equal to expectation (%d).",
                        (int)*(size_t *)KeyValuePair.Value, (int)Expected[i]);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    if ( i != ItemCount ) {
        TEST_PRINTF("Test Failure - Iterator failed to stop at all items (Count = %lu).",
                    (unsigned long)i);
        Binary_Tree_Release(Tree);
        TEST_FAILURE;
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}

int Test_Binary_Tree_Iterate_Previous_PostOrder(void) {

    Binary_Tree_t *            Tree      = NULL;
    size_t                     ItemCount = 16, i = 0;
    Binary_Tree_KeyValuePair_t KeyValuePair = {0, NULL};
    Binary_Tree_Direction_t    Direction    = Direction_PostOrder;
    size_t                     Expected[] = {7, 11, 13, 14, 15, 12, 9, 10, 8, 3, 5, 6, 4, 1, 2, 0};

    Tree = Binary_Tree_Create((CompareFunc_t *)CompareFunc_Size_t, sizeof(size_t), NULL,
                              Policy_Overwrite);
    if ( NULL == Tree ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Binary Tree for testing.");
        TEST_FAILURE;
    }

    for ( i = 0; i < ItemCount; i++ ) {
        if ( 0 != Binary_Tree_Insert(Tree, &i, sizeof(i), &i, sizeof(i), NULL) ) {
            TEST_PRINTF("%s", "Test Failure - Failed to insert item.");
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    for ( i = 0, KeyValuePair = Binary_Tree_Previous(Tree, Direction); NULL != KeyValuePair.Value;
          i++, KeyValuePair   = Binary_Tree_Previous(Tree, Direction) ) {

        if ( (unsigned long)*(size_t *)KeyValuePair.Value != Expected[i] ) {
            TEST_PRINTF("Test Failure - Returned value (%d) not equal to expectation (%d).",
                        (int)*(size_t *)KeyValuePair.Value, (int)Expected[i]);
            Binary_Tree_Release(Tree);
            TEST_FAILURE;
        }
    }

    if ( i != ItemCount ) {
        TEST_PRINTF("Test Failure - Iterator failed to stop at all items (Count = %lu).",
                    (unsigned long)i);
        Binary_Tree_Release(Tree);
        TEST_FAILURE;
    }

    Binary_Tree_Release(Tree);
    TEST_SUCCESSFUL;
}
