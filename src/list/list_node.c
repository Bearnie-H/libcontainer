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

List_Node_t *ListNode_Create(const void *Contents, size_t Size) {

    List_Node_t *Node = NULL;

    if (NULL == Contents) {
        DEBUG_PRINTF("%s", "Error, NULL Contents pointer provided.");
        return NULL;
    }

    if (0 == Size) {
        DEBUG_PRINTF("%s", "Error, Element cannot have a size of 0 bytes.");
        return NULL;
    }

    Node = (List_Node_t *)malloc(sizeof(List_Node_t));
    if (NULL == Node) {
        DEBUG_PRINTF("%s", "Error, Failed to allocate memory for new List_Node_t.");
        return NULL;
    }

    Node->Next = NULL;
    Node->Previous = NULL;
    Node->IsReference = false;
    Node->ReleaseFunc = free;

    Node->Contents = (uint8_t *)malloc(sizeof(uint8_t) * Size);
    if (NULL == Node->Contents) {
        DEBUG_PRINTF("%s", "Error, Failed to allocate memory for new List_Node_t->Contents.");
        free(Node);
        return NULL;
    }

    memcpy(Node->Contents, Contents, Size);
    Node->Size = Size;

    DEBUG_PRINTF("%s", "Successfully created new List_Node_t.");
    return Node;
}

List_Node_t *ListNode_RefCreate(const void *Contents, ReleaseFunc_t ReleaseFunc) {

    List_Node_t *Node = NULL;

    if (NULL == Contents) {
        DEBUG_PRINTF("%s", "Error, NULL Contents pointer provided.");
        return NULL;
    }

    if (NULL == ReleaseFunc) {
        DEBUG_PRINTF("%s", "NULL ReleaseFunc provided, defaulting to free().");
        ReleaseFunc = free;
    }

    Node = (List_Node_t *)malloc(sizeof(List_Node_t));
    if (NULL == Node) {
        DEBUG_PRINTF("%s", "Error, Failed to allocate memory for new List_Node_t.");
        return NULL;
    }

    Node->Next = NULL;
    Node->Previous = NULL;
    Node->Contents = (uint8_t *)Contents;
    Node->Size = 0; /* Size can be 0 for ref-types, as this Node owns 0 bytes of
                       memory. */
    Node->IsReference = true;
    Node->ReleaseFunc = ReleaseFunc;

    DEBUG_PRINTF("%s", "Successfully created new List_Node_t.");
    return Node;
}

void ListNode_Release(List_Node_t *Node) {

    if (NULL == Node) {
        DEBUG_PRINTF("%s", "NULL List_Node_t* provided, nothing to release.");
        return;
    }

    if (NULL != Node->Contents) {
        Node->ReleaseFunc(Node->Contents);
    }

    ListNode_Delete(Node);

    DEBUG_PRINTF("%s", "Successfully released List_Node_t.");
    return;
}

int ListNode_InsertAfter(List_Node_t *Base, List_Node_t *ToInsert) {

    List_Node_t *Next = NULL;

    if ((NULL == Base) || (NULL == ToInsert)) {
        DEBUG_PRINTF("%s", "Error, NULL List_Node_t* provided for either Base or ToInsert.");
        return 1;
    }

    Next = Base->Next;

    Base->Next = ToInsert;
    ToInsert->Next = Next;

    ToInsert->Previous = Base;

    if (NULL != Next) {
        Next->Previous = ToInsert;
    }

    DEBUG_PRINTF("%s", "Successfully added new List_Node_t* after requested List_Node_t*.");
    return 0;
}

int ListNode_InsertBefore(List_Node_t *Base, List_Node_t *ToInsert) {

    List_Node_t *Previous = NULL;

    if ((NULL == Base) || (NULL == ToInsert)) {
        DEBUG_PRINTF("%s", "Error, NULL List_Node_t* provided for either Base or ToInsert.");
        return 1;
    }

    Previous = Base->Previous;

    Base->Previous = ToInsert;
    ToInsert->Previous = Previous;

    if (NULL != Previous) {
        Previous->Next = ToInsert;
    }
    ToInsert->Next = Base;

    DEBUG_PRINTF("%s", "Successfully added new List_Node_t* before requested List_Node_t*.");
    return 0;
}

void ListNode_Delete(List_Node_t *Node) {

    if (NULL == Node) {
        DEBUG_PRINTF("%s", "NULL List_Node_t* provided, nothing to delete.");
        return;
    }

    if (NULL != Node->Previous) {
        Node->Previous->Next = Node->Next;
    }

    if (NULL != Node->Next) {
        Node->Next->Previous = Node->Previous;
    }

    Node->Contents = NULL;
    Node->IsReference = false;
    Node->Next = NULL;
    Node->Previous = NULL;
    Node->Size = 0;
    Node->ReleaseFunc = NULL;
    free(Node);

    DEBUG_PRINTF("%s", "Successfully deleted List_Node_t but not its contents.");
    return;
}

int ListNode_UpdateValue(List_Node_t *Node, const void *Element, size_t ElementSize,
                         ReleaseFunc_t ReleaseFunc) {

    void *NewContents = NULL;
    bool NewNodeIsReference = false;

    if ((NULL == Node) || (NULL == Element)) {
        DEBUG_PRINTF("%s", "Error, NULL Node* or Element* provided.");
        return 1;
    }

    if (0 == ElementSize) {
        NewNodeIsReference = true;
        NewContents = (void *)Element;
        if (NULL == ReleaseFunc) {
            DEBUG_PRINTF("%s", "Error, NULL ReleaseFunc provided for "
                               "Reference-Type contents.");
            return 1;
        }
    } else {
        NewNodeIsReference = false;
        ReleaseFunc = (ReleaseFunc_t)free;
        NewContents = (uint8_t *)malloc(sizeof(uint8_t) * ElementSize);
        if (NULL == NewContents) {
            DEBUG_PRINTF("%s", "Error, Failed to allocate memory to hold new Node contents.");
            return 1;
        }
        memcpy(NewContents, Element, ElementSize);
    }

    /* Release the existing contents of the node. */
    Node->ReleaseFunc(Node->Contents);

    /* Actually update the Node to the desired new contents. */
    Node->Contents = NewContents;
    Node->IsReference = NewNodeIsReference;
    Node->ReleaseFunc = ReleaseFunc;
    Node->Size = ElementSize;

    DEBUG_PRINTF("%s", "Successfully updated List_Node_t contents to new value.");
    return 0;
}
