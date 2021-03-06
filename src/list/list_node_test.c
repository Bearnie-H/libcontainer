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

#include <stdlib.h>
#include <string.h>

#include "../logging/logging.h"
#include "include/list_node.h"

int Test_list_node(void) {

    int FailedTests = 0;

    FailedTests += Test_ListNode_Create();
    FailedTests += Test_ListNode_RefCreate();
    FailedTests += Test_ListNode_InsertAfter();
    FailedTests += Test_ListNode_InsertBefore();
    FailedTests += Test_ListNode_UpdateValue();

    return FailedTests;
}

int Test_ListNode_Create(void) {

    List_Node_t *Node       = NULL;
    char         Contents[] = "Hello World";

    Node = ListNode_Create(Contents, sizeof(Contents));
    if ( NULL == Node ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_Node_t.");
        TEST_FAILURE;
    }

    ListNode_Release(Node);
    TEST_SUCCESSFUL;
}

int Test_ListNode_RefCreate(void) {

    List_Node_t *Outer      = NULL;
    List_Node_t *Inner      = NULL;
    char         Contents[] = "Hello World";

    Inner = ListNode_Create(Contents, sizeof(Contents));
    if ( NULL == Inner ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create inner List_Node_t.");
        TEST_FAILURE;
    }

    Outer = ListNode_RefCreate(Inner, (ReleaseFunc_t *)ListNode_Release);
    if ( NULL == Outer ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create outer List_Node_t.");
        TEST_FAILURE;
    }

    ListNode_Release(Outer);
    TEST_SUCCESSFUL;
}

int Test_ListNode_InsertAfter(void) {

    List_Node_t *Node1 = NULL, *Node2 = NULL;
    char         Contents[] = "Hello World";

    Node1 = ListNode_Create(Contents, sizeof(Contents));
    if ( NULL == Node1 ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Test Failure - Failed to allocate List_Node_t (Node1).");
#endif
        TEST_FAILURE;
    }

    Node2 = ListNode_Create(Contents, sizeof(Contents));
    if ( NULL == Node2 ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Test Failure - Failed to allocate List_Node_t (Node2).");
#endif
        ListNode_Release(Node1);
        TEST_FAILURE;
    }

    if ( 0 != ListNode_InsertAfter(Node1, Node2) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Test Failure - Failed to insert List_Node_t.");
#endif
        ListNode_Release(Node1);
        ListNode_Release(Node2);
        TEST_FAILURE;
    }

    if ( !((Node1->Next == Node2) && (Node1->Next->Previous == Node1) &&
           (Node2->Previous == Node1) && (Node2->Previous->Next == Node2)) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Test Failure - Inter-Node linking not equal to expectation.");
#endif
        ListNode_Release(Node1);
        ListNode_Release(Node2);
        TEST_FAILURE;
    }

    ListNode_Release(Node1);
    ListNode_Release(Node2);
    TEST_SUCCESSFUL;
}

int Test_ListNode_InsertBefore(void) {

    List_Node_t *Node1 = NULL, *Node2 = NULL;
    char         Contents[] = "Hello World";

    Node1 = ListNode_Create(Contents, sizeof(Contents));
    if ( NULL == Node1 ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Test Failure - Failed to allocate List_Node_t (Node1).");
#endif
        TEST_FAILURE;
    }

    Node2 = ListNode_Create(Contents, sizeof(Contents));
    if ( NULL == Node2 ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Test Failure - Failed to allocate List_Node_t (Node2).");
#endif
        ListNode_Release(Node1);
        TEST_FAILURE;
    }

    if ( 0 != ListNode_InsertBefore(Node1, Node2) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Test Failure - Failed to insert List_Node_t.");
#endif
        ListNode_Release(Node1);
        ListNode_Release(Node2);
        TEST_FAILURE;
    }

    if ( !((Node1->Previous == Node2) && (Node1->Previous->Next == Node1) &&
           (Node2->Next == Node1) && (Node2->Next->Previous == Node2)) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Test Failure - Inter-Node linking not equal to expectation.");
#endif
        ListNode_Release(Node1);
        ListNode_Release(Node2);
        TEST_FAILURE;
    }

    ListNode_Release(Node1);
    ListNode_Release(Node2);
    TEST_SUCCESSFUL;
}

int Test_ListNode_UpdateValue(void) {

    List_Node_t *Node            = NULL;
    int          InitialContents = 10, NewContents = 5;

    Node = ListNode_Create(&InitialContents, sizeof(InitialContents));
    if ( NULL == Node ) {
        TEST_PRINTF("%s", "Test Failure - Failed to create List_Node_t.");
        TEST_FAILURE;
    }

    if ( 0 != ListNode_UpdateValue(Node, &NewContents, sizeof(NewContents), NULL) ) {
        TEST_PRINTF("%s", "Test Failure - Error setting new Node value.");
        ListNode_Release(Node);
        TEST_FAILURE;
    }

    if ( 0 != memcmp(Node->Contents.ContentBytes, &NewContents, sizeof(NewContents)) ) {
        TEST_PRINTF("Test Failure - Node contents (%d) not equal to expected value (%d).",
                    *(int *)(Node->Contents.ContentRaw), NewContents);
        ListNode_Release(Node);
        TEST_FAILURE;
    }

    ListNode_Release(Node);
    TEST_SUCCESSFUL;
}
