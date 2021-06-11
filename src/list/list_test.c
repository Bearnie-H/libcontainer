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

#include "../logging/logging.h"
#include "include/list.h"

int Test_list(void) {

    int FailedTests = 0;

    /* Test the functions on the nodes specifically, to ensure all List tests
     * can use them safely. */
    FailedTests += Test_list_node();

    /* Test the list creation function */
    FailedTests += Test_List_Create();

    /*  Test the non-trivial operations on the List_t. */
    FailedTests += Test_List_Insert();
    FailedTests += Test_List_RefInsert();
    FailedTests += Test_List_Prepend();
    FailedTests += Test_List_RefPrepend();
    FailedTests += Test_List_Append();
    FailedTests += Test_List_RefAppend();
    FailedTests += Test_List_Remove();
    FailedTests += Test_List_RemoveAll();
    FailedTests += Test_List_GetElement();
    FailedTests += Test_List_SetElement();
    FailedTests += Test_List_RefSetElement();
    FailedTests += Test_List_PopElement();
    FailedTests += Test_List_PopFront();
    FailedTests += Test_List_PopBack();

    FailedTests += Test_List_Clear();

    FailedTests += Test_list_callbacks();

    return FailedTests;
}

int Test_List_Insert(void) {

    List_t *List = NULL;
    size_t Count = 10, i = 0;
    List_Node_t *Node = NULL;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    for (i = 0, Node = List->Head; i < Count; i++, Node = Node->Next) {
        if (0 != memcmp(Node->Contents, &i, sizeof(i))) {
            List_Release(List);
            TEST_PRINTF("Test Failure - List element at index [ %d ] not equal to expected value, "
                        "Got: [ %d ], Expected: [ %d ].",
                        (int)i, (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_RefInsert(void) {

    List_t *List = NULL;
    size_t i = 0;
    const size_t Count = 10, RefIndex = 5;
    char *RefItem = NULL;
    const char RefContents[] = "Hello.";
    List_Node_t *Node = NULL;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    RefItem = (char *)malloc(sizeof(char) * sizeof(RefContents));
    if (NULL == RefItem) {
        List_Release(List);
        TEST_PRINTF("%s",
                    "Test Failure - Failed to create Reference-Type item to insert into List_t.");
        TEST_FAILURE;
    }

    strcpy(RefItem, RefContents);

    if (0 != List_RefInsert(List, RefItem, (ReleaseFunc_t *)free, RefIndex)) {
        List_Release(List);
        TEST_PRINTF("Test Failure - Failed to insert element [ %s ] at index [ %d ] of List_t.",
                    RefItem, (int)RefIndex);
        TEST_FAILURE;
    }

    for (i = 0, Node = List->Head; i < RefIndex; i++, Node = Node->Next)
        ;

    if (0 != memcmp(RefItem, Node->Contents, sizeof(RefContents))) {
        TEST_PRINTF(
            "Test Failure - Contents at index [ %d ] (%s) do not match expected value (%s).",
            (int)RefIndex, RefItem, (char *)Node->Contents);
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_Prepend(void) {

    List_t *List = NULL;
    size_t Count = 10, i = 0;
    size_t NewItem = 0xBEEF;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    if (0 != List_Prepend(List, &NewItem, sizeof(NewItem))) {
        List_Release(List);
        TEST_PRINTF("Test Failure - Failed to prepend [ %d ] to List_t.", (int)NewItem);
        TEST_FAILURE;
    }

    if (0 != memcmp(List->Head->Contents, &NewItem, sizeof(NewItem))) {
        TEST_PRINTF("Test Failure - List_t Head contents (%d) not equal to expected value (%d).",
                    (int)**(size_t **)&List->Head->Contents, (int)NewItem);
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_RefPrepend(void) {

    List_t *List = NULL;
    size_t i = 0;
    const size_t Count = 10;
    char *RefItem = NULL;
    const char RefContents[] = "Hello.";
    List_Node_t *Node = NULL;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    RefItem = (char *)malloc(sizeof(char) * sizeof(RefContents));
    if (NULL == RefItem) {
        List_Release(List);
        TEST_PRINTF("%s",
                    "Test Failure - Failed to create Reference-Type item to insert into List_t.");
        TEST_FAILURE;
    }

    strcpy(RefItem, RefContents);

    if (0 != List_RefPrepend(List, RefItem, (ReleaseFunc_t *)free)) {
        List_Release(List);
        TEST_PRINTF("Test Failure - Failed to prepend element [ %s ] to List_t.", RefItem);
        TEST_FAILURE;
    }

    Node = List->Head;

    if (0 != memcmp(RefItem, Node->Contents, sizeof(RefContents))) {
        TEST_PRINTF("Test Failure - Contents at List Head (%s) do not match expected value (%s).",
                    (char *)Node->Contents, RefItem);
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_Append(void) {

    List_t *List = NULL;
    size_t Count = 10, i = 0;
    size_t NewItem = 0xBEEF;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    if (0 != List_Append(List, &NewItem, sizeof(NewItem))) {
        List_Release(List);
        TEST_PRINTF("Test Failure - Failed to append [ %d ] to List_t.", (int)NewItem);
        TEST_FAILURE;
    }

    if (NewItem != **(size_t **)&(List->Tail->Contents)) {
        TEST_PRINTF("Test Failure - List_t Tail contents (%d) not equal to expected value (%d).",
                    (int)**(size_t **)&(List->Tail->Contents), (int)NewItem);
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_RefAppend(void) {

    List_t *List = NULL;
    size_t i = 0;
    const size_t Count = 10;
    char *RefItem = NULL;
    const char RefContents[] = "Hello.";
    List_Node_t *Node = NULL;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    RefItem = (char *)malloc(sizeof(char) * sizeof(RefContents));
    if (NULL == RefItem) {
        List_Release(List);
        TEST_PRINTF("%s",
                    "Test Failure - Failed to create Reference-Type item to insert into List_t.");
        TEST_FAILURE;
    }

    strcpy(RefItem, RefContents);

    if (0 != List_RefAppend(List, RefItem, (ReleaseFunc_t *)free)) {
        List_Release(List);
        TEST_PRINTF("Test Failure - Failed to append element [ %s ] to List_t.", RefItem);
        TEST_FAILURE;
    }

    Node = List->Tail;

    if (0 != memcmp(RefItem, Node->Contents, sizeof(RefContents))) {
        TEST_PRINTF("Test Failure - Contents at List Tail (%s) do not match expected value (%s).",
                    (char *)Node->Contents, RefItem);
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_Remove(void) {

    List_t *List = NULL;
    size_t Count = 10, i = 0, RemoveIndex = 8;
    size_t *CheckItem = NULL;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    if (0 != List_Remove(List, RemoveIndex)) {
        List_Release(List);
        TEST_PRINTF("Test Failure - Failed to remove item at index [ %d ].", (int)RemoveIndex);
        TEST_FAILURE;
    }

    CheckItem = List_GetElement(List, RemoveIndex);
    if (NULL == CheckItem) {
        TEST_PRINTF("Test Failure - Failed to get new item at index [ %d ].", (int)RemoveIndex);
        List_Release(List);
        TEST_FAILURE;
    }

    if (*CheckItem != RemoveIndex + 1) {
        TEST_PRINTF(
            "Test Failure - New item at index [ %d ] (%d) not equal to expected value (%d).",
            (int)RemoveIndex, (int)*CheckItem, (int)(RemoveIndex + 1));
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_RemoveAll(void) {

    List_t *List = NULL;
    size_t Count = 10, i = 0, RemoveIndex = 0;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    for (i = 0; i < Count - 1; i++) {
        if (0 != List_Remove(List, RemoveIndex)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to remove item at index [ %d ] (%d/%d).",
                        (int)RemoveIndex, (int)i, (int)Count);
            TEST_FAILURE;
        }
    }

    if (List->Head != List->Tail) {
        List_Release(List);
        TEST_PRINTF("%s",
                    "Test Failure - List->Head and List->Tail not equal for list of length 1.");
        TEST_FAILURE;
    }

    if (0 != List_Remove(List, RemoveIndex)) {
        List_Release(List);
        TEST_PRINTF("Test Failure - Failed to remove item at index [ %d ] (%d/%d).",
                    (int)RemoveIndex, (int)i, (int)Count);
        TEST_FAILURE;
    }

    if ((NULL != List->Head) || (NULL != List->Tail)) {
        List_Release(List);
        TEST_PRINTF("%s",
                    "Test Failure - List->Head and List->Tail not NULL for list of length 0.");
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_GetElement(void) {

    List_t *List = NULL;
    size_t Count = 10, i = 0, CheckIndex = 4, *ElementValue = NULL;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    ElementValue = (size_t *)List_GetElement(List, CheckIndex);
    if (NULL == ElementValue) {
        List_Release(List);
        TEST_PRINTF("Test Failure - Failed to get element at index [ %d ].", (int)CheckIndex);
        TEST_FAILURE;
    }

    if (*ElementValue != CheckIndex) {
        TEST_PRINTF("Test Failure - Value at index [ %d ] (%d) not equal to expected value (%d).",
                    (int)CheckIndex, (int)*ElementValue, (int)CheckIndex);
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_SetElement(void) {

    List_t *List = NULL;
    size_t Count = 10, i = 0, NewItem = 0xBEEF, UpdateIndex = 6, *Element = NULL;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    if (0 != List_SetElement(List, &NewItem, sizeof(NewItem), UpdateIndex)) {
        List_Release(List);
        TEST_PRINTF("Test Failure - Failed to update value at index [ %d ].", (int)UpdateIndex);
        TEST_FAILURE;
    }

    Element = (size_t *)List_GetElement(List, UpdateIndex);
    if (NULL == Element) {
        List_Release(List);
        TEST_PRINTF("Test Failure - Failed to get item at index [ %d ] to check value.",
                    (int)UpdateIndex);
        TEST_FAILURE;
    }

    if (*Element != NewItem) {
        TEST_PRINTF("Test Failure - Item at index [ %d ] (%d) does not match expected value (%d).",
                    (int)UpdateIndex, (int)*Element, (int)NewItem);
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_RefSetElement(void) {

    List_t *List = NULL;
    size_t i = 0, Count = 10, UpdateIndex = 6;
    char *RefItem = NULL;
    const char RefContents[] = "Hello.";
    char *CheckItem = NULL;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    RefItem = (char *)malloc(sizeof(char) * sizeof(RefContents));
    if (NULL == RefItem) {
        List_Release(List);
        TEST_PRINTF("%s",
                    "Test Failure - Failed to create Reference-Type item to insert into List_t.");
        TEST_FAILURE;
    }

    strcpy(RefItem, RefContents);

    if (0 != List_RefSetElement(List, RefItem, (ReleaseFunc_t *)free, UpdateIndex)) {
        TEST_PRINTF("Test Failure - Failed to update element at index [ %d ] (%s) in List_t.",
                    (int)UpdateIndex, RefItem);
        List_Release(List);
        TEST_FAILURE;
    }

    CheckItem = (char *)List_GetElement(List, UpdateIndex);
    if (NULL == CheckItem) {
        TEST_PRINTF("Test Failure - Failed to get pointer to item at index [ %d ].",
                    (int)UpdateIndex);
        List_Release(List);
        TEST_FAILURE;
    }

    if (0 != memcmp(CheckItem, RefItem, sizeof(RefContents))) {
        TEST_PRINTF("Test Failure - Item at index [ %d ] (%s) not equal to expected value (%s).",
                    (int)UpdateIndex, CheckItem, RefItem);
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_PopElement(void) {

    List_t *List = NULL;
    size_t Count = 10, i = 0, PopIndex = 5, *PoppedValue = NULL;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    PoppedValue = List_PopElement(List, PopIndex);
    if (NULL == PoppedValue) {
        List_Release(List);
        TEST_PRINTF("Test Failure - Failed to pop item at index [ %d ].", (int)PopIndex);
        TEST_FAILURE;
    }

    if (*PoppedValue != PopIndex) {
        TEST_PRINTF(
            "Test Failure - Value popped from index [ %d ] (%d) does not equal expectation (%d).",
            (int)PopIndex, (int)*PoppedValue, (int)PopIndex);
        List_Release(List);
        free(PoppedValue);
        TEST_FAILURE;
    }

    free(PoppedValue);

    if (List_Length(List) != (size_t)(Count - 1)) {
        TEST_PRINTF("%s", "Test Failure - List Length not updated following Pop operation");
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_PopFront(void) {

    List_t *List = NULL;
    size_t Count = 10, i = 0, *PoppedValue = NULL;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    PoppedValue = List_PopFront(List);
    if (NULL == PoppedValue) {
        List_Release(List);
        TEST_PRINTF("%s", "Test Failure - Failed to pop item at front of List_t.");
        TEST_FAILURE;
    }

    if (*PoppedValue != 0) {
        TEST_PRINTF("Test Failure - Value popped from front (%d) does not equal expectation (%d).",
                    (int)*PoppedValue, 0);
        List_Release(List);
        free(PoppedValue);
        TEST_FAILURE;
    }

    free(PoppedValue);

    if (List_Length(List) != (size_t)(Count - 1)) {
        TEST_PRINTF("%s", "Test Failure - List Length not updated following Pop operation");
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}

int Test_List_PopBack(void) {

    List_t *List = NULL;
    size_t Count = 10, i = 0, *PoppedValue = NULL;

    List = List_Create();
    if (NULL == List) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_t.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != List_Insert(List, &i, sizeof(i), i)) {
            List_Release(List);
            TEST_PRINTF("Test Failure - Failed to insert element [ %d ] at index [ %d ] to "
                        "initialize List_t.",
                        (int)i, (int)i);
            TEST_FAILURE;
        }
    }

    PoppedValue = List_PopBack(List);
    if (NULL == PoppedValue) {
        List_Release(List);
        TEST_PRINTF("%s", "Test Failure - Failed to pop item at back of List_t");
        TEST_FAILURE;
    }

    if (*PoppedValue != (Count - 1)) {
        TEST_PRINTF(
            "Test Failure - Value popped from back of List_t (%d) does not equal expectation (%d).",
            (int)*PoppedValue, (int)(Count - 1));
        List_Release(List);
        free(PoppedValue);
        TEST_FAILURE;
    }

    free(PoppedValue);

    if (List_Length(List) != (size_t)(Count - 1)) {
        TEST_PRINTF("%s", "Test Failure - List Length not updated following Pop operation");
        List_Release(List);
        TEST_FAILURE;
    }

    List_Release(List);
    TEST_SUCCESSFUL;
}
