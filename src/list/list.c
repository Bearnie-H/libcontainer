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
#include "include/list.h"

size_t List_Length(List_t *List) {

    if (NULL == List) {
        DEBUG_PRINTF("%s", "NULL List_t* provided, no length to report.");
        return 0;
    }

    return List->Length;
}

int List_Insert(List_t *List, const void *Element, size_t ElementSize, size_t Index) {

    List_Node_t *NewNode = NULL;
    List_Node_t *BaseNode = NULL;

    if ((NULL == List) || (NULL == Element)) {
        DEBUG_PRINTF("%s", "Error, NULL List_t* or Element provided.");
        return 1;
    }

    if ((List->Length < Index)) {
        DEBUG_PRINTF("Error, Index value of [ %d ] is out of bounds.", (int)Index);
        return 1;
    }

    if (0 == ElementSize) {
        DEBUG_PRINTF("%s", "Error, Invalid ElementSize of 0.");
        return 1;
    }

    NewNode = ListNode_Create(Element, ElementSize);
    if (NULL == NewNode) {
        DEBUG_PRINTF("%s", "Error, Failed to create new List_Node_t to add into List_t.");
        return 1;
    }

    if (0 == List->Length) {
        List->Head = NewNode;
        List->Tail = NewNode;
    } else if (0 == Index) {
        if (0 != ListNode_InsertBefore(List->Head, NewNode)) {
            DEBUG_PRINTF("%s", "Failed to insert new List_Node_t at beginning of the List_t.");
            ListNode_Release(NewNode);
            return 1;
        }
        List->Head = NewNode;
    } else if (List->Length == (size_t)Index) {
        if (0 != ListNode_InsertAfter(List->Tail, NewNode)) {
            DEBUG_PRINTF("%s", "Failed to insert new List_Node_t at the end of the List_t.");
            ListNode_Release(NewNode);
            return 1;
        }
        List->Tail = NewNode;
    } else {
        BaseNode = List_findNode(List, Index);
        if (0 != ListNode_InsertBefore(BaseNode, NewNode)) {
            DEBUG_PRINTF("%s", "Failed to insert new List_Node_t at the requested index.");
            ListNode_Release(NewNode);
            return 1;
        }
    }

    List->Length += 1;

    DEBUG_PRINTF("%s", "Successfully inserted new item into List_t.");
    return 0;
}

int List_Prepend(List_t *List, const void *Element, size_t ElementSize) {
    return List_Insert(List, Element, ElementSize, 0);
}

int List_Append(List_t *List, const void *Element, size_t ElementSize) {
    return List_Insert(List, Element, ElementSize, List->Length);
}

int List_RefInsert(List_t *List, const void *Element, ReleaseFunc_t *ReleaseFunc, size_t Index) {

    List_Node_t *NewNode = NULL;
    List_Node_t *BaseNode = NULL;

    if ((NULL == List) || (NULL == Element)) {
        DEBUG_PRINTF("%s", "Error, NULL List_t* or Element provided.");
        return 1;
    }

    if ((List->Length < Index)) {
        DEBUG_PRINTF("Error, Index value of [ %d ] is out of bounds.", (int)Index);
        return 1;
    }

    NewNode = ListNode_RefCreate(Element, ReleaseFunc);
    if (NULL == NewNode) {
        DEBUG_PRINTF("%s", "Error, Failed to create new List_Node_t to add into List_t.");
        return 1;
    }

    if (0 == List->Length) {
        List->Head = NewNode;
        List->Tail = NewNode;
    } else if (0 == Index) {
        if (0 != ListNode_InsertBefore(List->Head, NewNode)) {
            DEBUG_PRINTF("%s", "Failed to insert new List_Node_t at beginning of the List_t.");
            ListNode_Release(NewNode);
            return 1;
        }
        List->Head = NewNode;
    } else if (List->Length == (size_t)Index) {
        if (0 != ListNode_InsertAfter(List->Tail, NewNode)) {
            DEBUG_PRINTF("%s", "Failed to insert new List_Node_t at the end of the List_t.");
            ListNode_Release(NewNode);
            return 1;
        }
        List->Tail = NewNode;
    } else {
        BaseNode = List_findNode(List, Index);
        if (0 != ListNode_InsertBefore(BaseNode, NewNode)) {
            DEBUG_PRINTF("%s", "Failed to insert new List_Node_t at the requested index.");
            ListNode_Release(NewNode);
            return 1;
        }
    }

    List->Length += 1;

    DEBUG_PRINTF("%s", "Successfully inserted new item into List_t.");
    return 0;
}

int List_RefPrepend(List_t *List, const void *Element, ReleaseFunc_t *ReleaseFunc) {
    return List_RefInsert(List, Element, ReleaseFunc, 0);
}

int List_RefAppend(List_t *List, const void *Element, ReleaseFunc_t *ReleaseFunc) {
    return List_RefInsert(List, Element, ReleaseFunc, List->Length);
}

int List_Remove(List_t *List, size_t Index) {
    return List_removeNode(List, List_findNode(List, Index));
}

void *List_GetElement(List_t *List, size_t Index) {

    List_Node_t *Node = NULL;

    if (NULL == List) {
        DEBUG_PRINTF("%s", "Error, NULL List_t* provided.");
        return NULL;
    }

    if ((0 == List->Length) || (List->Length <= Index)) {
        DEBUG_PRINTF("Error, Index value of [ %d ] is out of bounds.", (int)Index);
        return NULL;
    }

    Node = List_findNode(List, Index);

    DEBUG_PRINTF("Successfully retrieved pointer to List_t item at index [ %d ].", (int)Index);
    return (void *)Node->Contents;
}

int List_SetElement(List_t *List, const void *Element, size_t ElementSize, size_t Index) {

    List_Node_t *Node = NULL;

    if ((NULL == List) || (NULL == Element)) {
        DEBUG_PRINTF("%s", "Error, NULL List_t* or Element provided.");
        return 1;
    }

    if ((0 == List->Length) || (List->Length <= (size_t)Index)) {
        DEBUG_PRINTF("Error, Index value of [ %d ] is out of bounds.", (int)Index);
        return 1;
    }

    if (0 == ElementSize) {
        DEBUG_PRINTF("%s", "Error, Illegal ElementSize of 0.");
        return 1;
    }

    Node = List_findNode(List, Index);

    /* Since we're setting the node contents to a non-reference type, the
     * ReleaseFunc is always just free(). */
    if (0 != ListNode_UpdateValue(Node, Element, ElementSize, (ReleaseFunc_t *)free)) {
        DEBUG_PRINTF("%s", "Error, Failed to update Node contents.");
        return 1;
    }

    DEBUG_PRINTF("Successfully updated contents of item at index [ %d ].", (int)Index);
    return 0;
}

int List_RefSetElement(List_t *List, const void *Element, ReleaseFunc_t *ReleaseFunc,
                       size_t Index) {

    List_Node_t *Node = NULL;

    if ((NULL == List) || (NULL == Element)) {
        DEBUG_PRINTF("%s", "Error, NULL List_t* or Element provided.");
        return 1;
    }

    if ((0 == List->Length) || (List->Length <= (size_t)Index)) {
        DEBUG_PRINTF("Error, Index value of [ %d ] is out of bounds.", (int)Index);
        return 1;
    }

    if (NULL == ReleaseFunc) {
        DEBUG_PRINTF("%s", "Error, NULL ReleaseFunc provided.");
        return 1;
    }

    Node = List_findNode(List, Index);

    if (0 != ListNode_UpdateValue(Node, Element, 0, ReleaseFunc)) {
        DEBUG_PRINTF("%s", "Error, Failed to update Node contents.");
        return 1;
    }

    DEBUG_PRINTF("Successfully updated contents of item at index [ %d ].", (int)Index);
    return 0;
}

void *List_PopElement(List_t *List, size_t Index) {

    List_Node_t *Node = NULL;
    void *NodeContents = NULL;

    if (NULL == List) {
        DEBUG_PRINTF("%s", "Error, NULL List_t* provided.");
        return NULL;
    }

    if ((0 == List->Length) || (List->Length <= (size_t)Index)) {
        DEBUG_PRINTF("Error, Index value of [ %d ] is out of bounds.", (int)Index);
        return NULL;
    }

    Node = List_findNode(List, Index);
    NodeContents = Node->Contents;

    if (Node == List->Head) {
        List->Head = List->Head->Next;
    }

    if (Node == List->Tail) {
        List->Tail = List->Tail->Previous;
    }

    ListNode_Delete(Node);

    List->Length -= 1;

    DEBUG_PRINTF("Successfully popped and returned contents for Node at index [ %d ].", (int)Index);
    return NodeContents;
}

void *List_PopFront(List_t *List) { return List_PopElement(List, 0); }

void *List_PopBack(List_t *List) { return List_PopElement(List, List->Length - 1); }

/* Private Function Definitions. */
List_Node_t *List_findNode(List_t *List, size_t Index) {

    size_t i = 0;
    List_Node_t *Node = NULL;

    if (0 == Index) {
        return List->Head;
    } else if ((List->Length - 1) == Index) {
        return List->Tail;
    }

    for (i = 0, Node = List->Head; i < Index; i++, Node = Node->Next)
        ;

    return Node;
}

int List_removeNode(List_t *List, List_Node_t *Node) {

    List_Node_t *CheckNode = NULL;

    if ((NULL == List) || (NULL == Node)) {
        DEBUG_PRINTF("%s", "Error: NULL List* or Node* provided.");
        return 1;
    }

    /* Make sure this node is actually within the list. */
    for (CheckNode = List->Head; CheckNode != NULL && CheckNode != Node;
         CheckNode = CheckNode->Next)
        ;

    if (NULL == CheckNode) {
        DEBUG_PRINTF("%s", "Error: Given List_Node_t* is not within the List_t.");
        return 1;
    }

    if (List->Head == Node) {
        List->Head = List->Head->Next;
    }

    if (List->Tail == Node) {
        List->Tail = List->Tail->Previous;
    }

    ListNode_Release(Node);
    List->Length -= 1;

    DEBUG_PRINTF("%s", "Successfully removed Node from List_t.");
    return 0;
}
