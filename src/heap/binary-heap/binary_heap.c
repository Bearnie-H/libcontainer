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

#include <stdlib.h>
#include <string.h>

#include "../../logging/logging.h"
#include "include/binary_heap.h"

Binary_Heap_t *Binary_Heap_Create(CompareFunc_t *KeyCompareFunc, ReleaseFunc_t *KeyReleaseFunc) {

    Binary_Heap_t *Heap = NULL;

    if ( NULL == KeyCompareFunc ) {
        DEBUG_PRINTF("%s", "Note: NULL KeyCompareFunc* provided, defaulting to memcmp().");
        KeyCompareFunc = memcmp;
    }

    if ( NULL == KeyReleaseFunc ) {
        DEBUG_PRINTF("%s", "Note: NULL KeyReleaseFunc* provided, defaulting to free().");
        KeyReleaseFunc = free;
    }

    Heap = (Binary_Heap_t *)calloc(1, sizeof(Binary_Heap_t));
    if ( NULL == Heap ) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Binary_Heap_t.");
        return NULL;
    }

    Heap->Items = Array_RefCreate(0, (ReleaseFunc_t *)Binary_Heap_Node_Release);
    if ( NULL == Heap->Items ) {
        DEBUG_PRINTF("%s",
                     "Error: Failed to allocate internal memory to hold Binary_Heap_t items.");
        free(Heap);
        return NULL;
    }

    Heap->KeyCompareFunc = KeyCompareFunc;
    Heap->KeyReleaseFunc = KeyReleaseFunc;

    DEBUG_PRINTF("%s", "Successfully created and initialized new Binary_Heap_t.");
    return Heap;
}

size_t Binary_Heap_Length(Binary_Heap_t *Heap) {

    if ( NULL == Heap ) {
        DEBUG_PRINTF("%s", "Warning: NULL Heap* provided.");
        return 0;
    }

    return Array_Length(Heap->Items);
}

bool Binary_Heap_IsEmpty(Binary_Heap_t *Heap) {
    return (0 == Binary_Heap_Length(Heap));
}

Binary_Heap_KeyValuePair_t Binary_Heap_Peek(Binary_Heap_t *Heap) {

    Binary_Heap_KeyValuePair_t KeyValuePair = {NULL, NULL};
    Binary_Heap_Node_t *       Node         = NULL;

    if ( NULL == Heap ) {
        DEBUG_PRINTF("%s", "Error: NULL Heap* provided.");
        return KeyValuePair;
    }

    if ( Binary_Heap_IsEmpty(Heap) ) {
        DEBUG_PRINTF("%s", "Warning: Binary_Heap_t is empty.");
        return KeyValuePair;
    }

    Node = Array_GetElement(Heap->Items, 0);
    if ( NULL == Node ) {
        DEBUG_PRINTF("%s", "Error: Failed to get root node of Binary_Heap_t.");
        return KeyValuePair;
    }

    KeyValuePair.Key   = Node->Key;
    KeyValuePair.Value = Node->Value;

    DEBUG_PRINTF("%s", "Successfully Peeked item at root of Binary_Heap_t.");
    return KeyValuePair;
}

Binary_Heap_KeyValuePair_t Binary_Heap_Pop(Binary_Heap_t *Heap) {

    Binary_Heap_KeyValuePair_t KeyValuePair = {NULL, NULL};
    Binary_Heap_Node_t *       Node         = NULL;

    if ( NULL == Heap ) {
        DEBUG_PRINTF("%s", "Error: NULL Heap* provided.");
        return KeyValuePair;
    }

    if ( Binary_Heap_IsEmpty(Heap) ) {
        DEBUG_PRINTF("%s", "Warning: Binary_Heap_t is empty.");
        return KeyValuePair;
    }

    Array_swapItems(Heap->Items, 0, Array_Length(Heap->Items) - 1);
    Node = Array_PopElement(Heap->Items, Array_Length(Heap->Items) - 1);
    if ( NULL == Node ) {
        /* We have to swap the items back, so we don't leave the heap in an invalid state. */
        Array_swapItems(Heap->Items, 0, Array_Length(Heap->Items) - 1);
        DEBUG_PRINTF("%s", "Error: Failed to remove item from Binary_Heap_t.");
        return KeyValuePair;
    }

    Iterator_Invalidate(&(Heap->Items->Iterator));

    KeyValuePair.Key   = Node->Key;
    KeyValuePair.Value = Node->Value;

    Node->KeyReleaseFunc   = NULL;
    Node->ValueReleaseFunc = NULL;

    Binary_Heap_Node_Release(Node);

    if ( 0 != Binary_Heap_siftDown(Heap) ) {
        DEBUG_PRINTF("%s", "Error: Error occurred during Sift-Down process.");
        return KeyValuePair;
    }

    DEBUG_PRINTF("%s", "Successfully removed Root from Binary_Heap_t.");
    return KeyValuePair;
}

int Binary_Heap_Remove(Binary_Heap_t *Heap) {

    Binary_Heap_Node_t *Node = NULL;

    if ( NULL == Heap ) {
        DEBUG_PRINTF("%s", "Error: NULL Heap* provided.");
        return 1;
    }

    if ( Binary_Heap_IsEmpty(Heap) ) {
        DEBUG_PRINTF("%s", "Warning: Binary_Heap_t is empty.");
        return 0;
    }

    Array_swapItems(Heap->Items, 0, Array_Length(Heap->Items) - 1);
    Node = Array_PopElement(Heap->Items, Array_Length(Heap->Items) - 1);
    if ( NULL == Node ) {
        /* We have to swap the items back, so we don't leave the heap in an invalid state. */
        Array_swapItems(Heap->Items, 0, Array_Length(Heap->Items) - 1);
        DEBUG_PRINTF("%s", "Error: Failed to remove item from Binary_Heap_t.");
        return 1;
    }

    Iterator_Invalidate(&(Heap->Items->Iterator));

    Binary_Heap_Node_Release(Node);

    if ( 0 != Binary_Heap_siftDown(Heap) ) {
        DEBUG_PRINTF("%s", "Error: Error occurred during Sift-Down process.");
        return 1;
    }

    DEBUG_PRINTF("%s", "Successfully removed Root from Binary_Heap_t.");
    return 0;
}

int Binary_Heap_Push(Binary_Heap_t *Heap, void *Key, size_t KeySize, void *Value, size_t ValueSize,
                     ReleaseFunc_t *ValueReleaseFunc) {

    Binary_Heap_Node_t *NewNode = NULL;

    if ( NULL == Heap ) {
        DEBUG_PRINTF("%s", "Error: NULL Heap* provided.");
        return 1;
    }

    NewNode = Binary_Heap_Node_Create(Key, Value, KeySize, ValueSize, Heap->KeyReleaseFunc,
                                      ValueReleaseFunc);

    if ( 0 != Array_Append(Heap->Items, &NewNode) ) {
        DEBUG_PRINTF("%s", "Error: Failed to add new item to Binary_Heap_t.");
        Binary_Heap_Node_Release(NewNode);
        return 1;
    }

    Iterator_Invalidate(&(Heap->Items->Iterator));

    if ( 0 != Binary_Heap_siftUp(Heap) ) {
        DEBUG_PRINTF("%s", "Error: Error occurred during Sift-Up process.");
        return 1;
    }

    DEBUG_PRINTF("%s", "Successfully added item to Binary_Heap_t.");
    return 0;
}

Binary_Heap_KeyValuePair_t Binary_Heap_Next(Binary_Heap_t *Heap) {

    Binary_Heap_KeyValuePair_t KeyValuePair = {NULL, NULL};
    Binary_Heap_Node_t *       Node         = NULL;

    if ( NULL == Heap ) {
        DEBUG_PRINTF("%s", "Error: NULL Heap* provided.");
        return KeyValuePair;
    }

    Node = Array_Next(Heap->Items);
    if ( NULL == Node ) {
        if ( NULL == Heap->Items->Iterator ) {
            DEBUG_PRINTF("%s", "Iteration has successfully finished for Binary_Heap_t.");
        } else {
            DEBUG_PRINTF("%s", "Error: Iteration has failed for Binary_Heap_t.");
        }
        return KeyValuePair;
    }

    KeyValuePair.Key   = Node->Key;
    KeyValuePair.Value = Node->Value;

    DEBUG_PRINTF("%s", "Successfully retrieved Next item from Binary_Heap_t.");
    return KeyValuePair;
}

int Binary_Heap_DoCallback(Binary_Heap_t *Heap, CallbackFunc_t *Callback) {

    Binary_Heap_KeyValuePair_t KeyValuePair = {NULL, NULL};
    int                        RetVal       = 0;

    if ( NULL == Heap ) {
        DEBUG_PRINTF("%s", "Error: NULL Heap* provided.");
        return -1;
    }

    if ( NULL == Callback ) {
        DEBUG_PRINTF("%s", "Note: NULL Callback* provided, nothing to do.");
        return 0;
    }

    BINARY_HEAP_FOREACH(Heap, KeyValuePair) {
        if ( 0 != Callback(&KeyValuePair) ) {
            DEBUG_PRINTF("%s", "Warning: Callback function returned non-zero.");
            RetVal += 1;
        }
    }

    Iterator_Invalidate(&(Heap->Items->Iterator));

    DEBUG_PRINTF("%s", "Finished performing Callback on Binary_Heap_t items.");
    return RetVal;
}

int Binary_Heap_DoCallbackArg(Binary_Heap_t *Heap, CallbackArgFunc_t *Callback, void *Args) {

    Binary_Heap_KeyValuePair_t KeyValuePair = {NULL, NULL};
    int                        RetVal       = 0;

    if ( NULL == Heap ) {
        DEBUG_PRINTF("%s", "Error: NULL Heap* provided.");
        return -1;
    }

    if ( NULL == Callback ) {
        DEBUG_PRINTF("%s", "Note: NULL Callback* provided, nothing to do.");
        return 0;
    }

    BINARY_HEAP_FOREACH(Heap, KeyValuePair) {
        if ( 0 != Callback(&KeyValuePair, Args) ) {
            DEBUG_PRINTF("%s", "Warning: Callback function returned non-zero.");
            RetVal += 1;
        }
    }

    Iterator_Invalidate(&(Heap->Items->Iterator));

    DEBUG_PRINTF("%s", "Finished performing Callback on Binary_Heap_t items.");
    return RetVal;
}

int Binary_Heap_Clear(Binary_Heap_t *Heap) {

    if ( NULL == Heap ) {
        DEBUG_PRINTF("%s", "Error: NULL Heap* provided.");
        return 1;
    }

    if ( 0 == Array_Clear(Heap->Items) ) {
        DEBUG_PRINTF("%s", "Successfully cleared all contents from Binary_Heap_t.");
        return 0;
    } else {
        DEBUG_PRINTF("%s", "Error: Failed to fully clear Binary_Heap_t.");
        return 1;
    }
}

void Binary_Heap_Release(Binary_Heap_t *Heap) {

    if ( NULL == Heap ) {
        DEBUG_PRINTF("%s", "Note: NULL Heap* provided, nothing to release.");
        return;
    }

    Array_Release(Heap->Items);

    ZERO_CONTAINER(Heap, Binary_Heap_t);
    free(Heap);

    DEBUG_PRINTF("%s", "Successfully released Binary_Heap_t.");
    return;
}

/* ++++++++++ Private Binary_Heap_t Functions ++++++++++ */

int Binary_Heap_siftUp(Binary_Heap_t *Heap) {

    Binary_Heap_Node_t *Current = NULL, *Parent = NULL;
    size_t              MinKeySize = 0, Index = 0;

    if ( Binary_Heap_IsEmpty(Heap) ) {
        DEBUG_PRINTF("%s", "Note: Sift-Up is not defined for the Root of a Binary_Heap_t.");
        return 0;
    }

    Index = Array_Length(Heap->Items) - 1;

    Current = Array_GetElement(Heap->Items, Index);
    if ( NULL == Current ) {
        DEBUG_PRINTF("%s", "Error: Failed to get \"Current\" Heap item.");
        return 1;
    }

    /*
        While the node at the current index has a "larger" value than it's parent,
        swap the two nodes.

        We can simplify this as walking "up" the heap, and checking if a given node
        compares larger than it's parent. This check takes advantage of how the PARENT_INDEX
        macro is defined. If Index == 0, this macro evaluates to 0 and the loop breaks.

        The other method of breaking out of the loop is the shortcut where the child is correctly
       smaller than it's parent. Since the heap property is always maintained at the end of every
       Push() or Pop() operation, once this is found we can exit.
    */
    while ( Index > PARENT_INDEX(Index) ) {

        Parent = Array_GetElement(Heap->Items, PARENT_INDEX(Index));
        if ( NULL == Parent ) {
            DEBUG_PRINTF("%s", "Error: Failed to get \"Parent\" Heap item.");
            return 1;
        }

        MinKeySize =
            ((Current->KeySize > Parent->KeySize) ? (Parent->KeySize) : (Current->KeySize));

        if ( Heap->KeyCompareFunc(Current->Key, Parent->Key, MinKeySize) > 0 ) {
            Array_swapItems(Heap->Items, Index, PARENT_INDEX(Index));
        } else {
            break;
        }

        Index = PARENT_INDEX(Index);
    }

    return 0;
}

int Binary_Heap_siftDown(Binary_Heap_t *Heap) {

    Binary_Heap_Node_t *Current = NULL, *LeftChild = NULL, *RightChild = NULL, *MaxChild = NULL;
    size_t              Index = 0, ArrayLength = 0, MinKeyLength = 0, ChildIndex = 0;

    ArrayLength = Array_Length(Heap->Items);

    if ( 0 == ArrayLength ) {
        DEBUG_PRINTF("%s", "Sift-Down operation not necessary for Binary_Heap_t with no elements.");
        return 0;
    }

    Current = Array_GetElement(Heap->Items, Index);
    if ( NULL == Current ) {
        DEBUG_PRINTF("%s", "Error: Failed to get Current.");
        return 1;
    }

    while ( Index <= ArrayLength ) {

        if ( LEFT_CHILD_INDEX(Index) < ArrayLength ) {
            LeftChild = Array_GetElement(Heap->Items, LEFT_CHILD_INDEX(Index));
            if ( NULL == LeftChild ) {
                DEBUG_PRINTF("%s", "Error: Failed to get Left Child.");
                return 1;
            }
        }

        if ( RIGHT_CHILD_INDEX(Index) < ArrayLength ) {
            RightChild = Array_GetElement(Heap->Items, RIGHT_CHILD_INDEX(Index));
            if ( NULL == RightChild ) {
                DEBUG_PRINTF("%s", "Error: Failed to get Right Child.");
                return 1;
            }
        }

        if ( (NULL != LeftChild) && (NULL != RightChild) ) {
            MinKeyLength = ((LeftChild->KeySize > RightChild->KeySize) ? (RightChild->KeySize)
                                                                       : (LeftChild->KeySize));
            if ( Heap->KeyCompareFunc(LeftChild->Key, RightChild->Key, MinKeyLength) >= 0 ) {
                MaxChild   = LeftChild;
                ChildIndex = LEFT_CHILD_INDEX(Index);
            } else {
                MaxChild   = RightChild;
                ChildIndex = RIGHT_CHILD_INDEX(Index);
            }
        } else if ( (NULL == LeftChild) && (NULL != RightChild) ) {
            MaxChild   = RightChild;
            ChildIndex = RIGHT_CHILD_INDEX(Index);
        } else if ( (NULL != LeftChild) && (NULL == RightChild) ) {
            MaxChild   = LeftChild;
            ChildIndex = LEFT_CHILD_INDEX(Index);
        } else {
            break;
        }

        MinKeyLength =
            ((MaxChild->KeySize > Current->KeySize) ? (Current->KeySize) : (MaxChild->KeySize));
        if ( Heap->KeyCompareFunc(Current->Key, MaxChild->Key, MinKeyLength) < 0 ) {
            Array_swapItems(Heap->Items, Index, ChildIndex);
        } else {
            break;
        }

        Index      = ChildIndex;
        LeftChild  = NULL;
        RightChild = NULL;
        MaxChild   = NULL;
    }

    return 0;
}

/* ---------- Private Binary_Heap_t Functions ---------- */
