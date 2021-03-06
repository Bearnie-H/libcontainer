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

    if ( NULL == Contents ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error, NULL Contents pointer provided.");
#endif
        return NULL;
    }

    if ( 0 == Size ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error, Element cannot have a size of 0 bytes.");
#endif
        return NULL;
    }

    Node = (List_Node_t *)calloc(1, sizeof(List_Node_t));
    if ( NULL == Node ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error, Failed to allocate memory for new List_Node_t.");
#endif
        return NULL;
    }

    Node->Next        = NULL;
    Node->Previous    = NULL;
    Node->ReleaseFunc = free;

    Node->Contents.ContentBytes = (uint8_t *)calloc(1, sizeof(uint8_t) * Size);
    if ( NULL == Node->Contents.ContentBytes ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error, Failed to allocate memory for new List_Node_t->Contents.");
#endif
        free(Node);
        return NULL;
    }

    memcpy(Node->Contents.ContentBytes, Contents, Size);
    Node->Size = Size;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully created new List_Node_t.");
#endif
    return Node;
}

List_Node_t *ListNode_RefCreate(void *Contents, ReleaseFunc_t *ReleaseFunc) {

    List_Node_t *Node = NULL;

    if ( NULL == Contents ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error, NULL Contents pointer provided.");
#endif
        return NULL;
    }

    if ( NULL == ReleaseFunc ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "NULL ReleaseFunc provided, defaulting to free().");
#endif
        ReleaseFunc = free;
    }

    Node = (List_Node_t *)calloc(1, sizeof(List_Node_t));
    if ( NULL == Node ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error, Failed to allocate memory for new List_Node_t.");
#endif
        return NULL;
    }

    Node->Next                = NULL;
    Node->Previous            = NULL;
    Node->Contents.ContentRaw = Contents;
    Node->Size                = 0; /* Size can be 0 for ref-types, as this Node owns 0 bytes of
                                      memory. */
    Node->ReleaseFunc         = ReleaseFunc;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully created new List_Node_t.");
#endif
    return Node;
}

void ListNode_Release(List_Node_t *Node) {

    if ( NULL == Node ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "NULL List_Node_t* provided, nothing to release.");
#endif
        return;
    }

    if ( (NULL != Node->Contents.ContentRaw) && (NULL != Node->ReleaseFunc) ) {
        Node->ReleaseFunc(Node->Contents.ContentRaw);
    }

    ListNode_Delete(Node);

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully released List_Node_t.");
#endif
    return;
}

int ListNode_InsertAfter(List_Node_t *Base, List_Node_t *ToInsert) {

    List_Node_t *Next = NULL;

    if ( (NULL == Base) || (NULL == ToInsert) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error, NULL List_Node_t* provided for either Base or ToInsert.");
#endif
        return 1;
    }

    Next = Base->Next;

    Base->Next     = ToInsert;
    ToInsert->Next = Next;

    ToInsert->Previous = Base;

    if ( NULL != Next ) {
        Next->Previous = ToInsert;
    }

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully added new List_Node_t* after requested List_Node_t*.");
#endif
    return 0;
}

int ListNode_InsertBefore(List_Node_t *Base, List_Node_t *ToInsert) {

    List_Node_t *Previous = NULL;

    if ( (NULL == Base) || (NULL == ToInsert) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error, NULL List_Node_t* provided for either Base or ToInsert.");
#endif
        return 1;
    }

    Previous = Base->Previous;

    Base->Previous     = ToInsert;
    ToInsert->Previous = Previous;

    if ( NULL != Previous ) {
        Previous->Next = ToInsert;
    }
    ToInsert->Next = Base;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully added new List_Node_t* before requested List_Node_t*.");
#endif
    return 0;
}

void ListNode_Delete(List_Node_t *Node) {

    if ( NULL == Node ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "NULL List_Node_t* provided, nothing to delete.");
#endif
        return;
    }

    if ( NULL != Node->Previous ) {
        Node->Previous->Next = Node->Next;
    }

    if ( NULL != Node->Next ) {
        Node->Next->Previous = Node->Previous;
    }

    ZERO_CONTAINER(Node, List_Node_t);
    free(Node);

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully deleted List_Node_t.");
#endif
    return;
}

int ListNode_UpdateValue(List_Node_t *Node, void *Element, size_t ElementSize,
                         ReleaseFunc_t *ReleaseFunc) {

    void *NewContents = NULL;

    if ( (NULL == Node) || (NULL == Element) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error, NULL Node* or Element* provided.");
#endif
        return 1;
    }

    if ( 0 == ElementSize ) {
        NewContents = (void *)Element;
        if ( NULL == ReleaseFunc ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error, NULL ReleaseFunc provided for "
                               "Reference-Type contents.");
#endif
            return 1;
        }
    } else {
        ReleaseFunc = (ReleaseFunc_t *)free;
        NewContents = (uint8_t *)calloc(1, sizeof(uint8_t) * ElementSize);
        if ( NULL == NewContents ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error, Failed to allocate memory to hold new Node contents.");
#endif
            return 1;
        }
        memcpy(NewContents, Element, ElementSize);
    }

    /* Release the existing contents of the node. */
    Node->ReleaseFunc(Node->Contents.ContentRaw);

    /* Actually update the Node to the desired new contents. */
    Node->Contents.ContentRaw = NewContents;
    Node->ReleaseFunc         = ReleaseFunc;
    Node->Size                = ElementSize;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully updated List_Node_t contents to new value.");
#endif
    return 0;
}
