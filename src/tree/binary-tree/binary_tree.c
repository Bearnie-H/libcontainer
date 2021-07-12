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
#include "include/binary_tree.h"
#include "include/tree_rotations.h"

Binary_Tree_t *Binary_Tree_Create(CompareFunc_t *KeyCompareFunc, size_t KeySize,
                                  ReleaseFunc_t *               KeyReleaseFunc,
                                  Binary_Tree_DuplicatePolicy_t Policy) {

    Binary_Tree_t *Tree = NULL;

    if ( NULL == KeyReleaseFunc ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: NULL KeyReleaseFunc* provided, defaulting to free().");
#endif
        KeyReleaseFunc = free;
    }

    if ( NULL == KeyCompareFunc ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: NULL KeyCompareFunc provided, defaulting to memcmp().");
#endif
        KeyCompareFunc = memcmp;
    }

    if ( 0 == KeySize ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s",
                     "Note: 0 ValueSize provided, creating Binary_Tree_t of Reference-Types.");
#endif
    }

    Tree = (Binary_Tree_t *)calloc(1, sizeof(Binary_Tree_t));
    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Binary_Tree_t.");
#endif
        return NULL;
    }

    Tree->KeyReleaseFunc  = KeyReleaseFunc;
    Tree->KeyCompareFunc  = KeyCompareFunc;
    Tree->Root            = NULL;
    Tree->TreeSize        = 0;
    Tree->KeySize         = KeySize;
    Tree->DuplicatePolicy = Policy;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully created new Binary_Tree_t.");
#endif
    return Tree;
}

size_t Binary_Tree_Length(Binary_Tree_t *Tree) {

    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Warning: NULL Tree* provided, unable to report length.");
#endif
        return 0;
    }

    return Tree->TreeSize;
}

int Binary_Tree_Insert(Binary_Tree_t *Tree, void *Key, size_t KeySize, void *Value,
                       size_t ValueSize, ReleaseFunc_t *ValueReleaseFunc) {

    Binary_Tree_Node_t *NewRoot = NULL, *NewNode = NULL;
    bool                IncrementSize = false;

    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot insert new value.");
#endif
        return 1;
    }

    if ( NULL == Key ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Key* Provided.");
#endif
        return 1;
    }

    if ( 0 == KeySize ) {
        if ( 0 != Tree->KeySize ) {
#ifdef DEBUG
            DEBUG_PRINTF("Note: KeySize of 0 provided, using cached value from tree initialization "
                         "of [ %lu ].",
                         (unsigned long)Tree->KeySize);
#endif
            KeySize = Tree->KeySize;
        } else {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Note: KeySize of 0 provided, treating as reference type.");
#endif
        }
    }

    if ( NULL != Value ) {
        if ( 0 == ValueSize ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Note: ValueSize of 0 provided, treating as reference type.");
#endif
        }

        if ( NULL == ValueReleaseFunc ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Note: NULL ValueReleaseFunc* provided, defaulting to free().");
#endif
            ValueReleaseFunc = free;
        }
    }

    Iterator_Invalidate(&(Tree->Iterator));

    if ( NULL == Value ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: NULL Value* provided, just inserting Key.");
#endif
    }

    IncrementSize = !(Binary_Tree_KeyExists(Tree, Key, KeySize));

    if ( !IncrementSize ) {
        switch ( Tree->DuplicatePolicy ) {
            case Policy_Error:
#ifdef DEBUG
                DEBUG_PRINTF("%s", "Error: Value with the given key already exists in the Tree!");
#endif
                return -1;
            default: break;
        }
    }

    /* Otherwise, this is a new Key, and must be added to the tree as a new Node. */
    NewNode = Binary_Tree_Node_Create(Key, KeySize, Tree->KeyReleaseFunc, Value, ValueSize,
                                      ValueReleaseFunc);
    if ( NULL == NewNode ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to create new Binary_Tree_Node_t");
#endif
        return 1;
    }

    NewRoot = Binary_Tree_insertNode(Tree->Root, Tree->KeyCompareFunc, NewNode);
    if ( NULL == NewRoot ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to insert new Binary_Tree_Node_t");
#endif
        Binary_Tree_Node_Release(NewNode);
        return 1;
    }

    Tree->Root = NewRoot;
    Tree->TreeSize += (1 && IncrementSize);
    return 0;
}

bool Binary_Tree_KeyExists(Binary_Tree_t *Tree, void *Key, size_t KeySize) {

    Binary_Tree_Node_t *Node = NULL;

    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot search for Key.");
#endif
        return false;
    }

    if ( (0 == KeySize) && (0 != Tree->KeySize) ) {
#ifdef DEBUG
        DEBUG_PRINTF("Note: Using cached KeySize of [ %lu ].", (unsigned long)Tree->KeySize);
#endif
        KeySize = Tree->KeySize;
    }

    Node = Binary_Tree_find(Tree->Root, Key, KeySize, Tree->KeyCompareFunc);
    if ( NULL == Node ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to find Key in Tree.");
#endif
        return false;
    }

    return true;
}

void *Binary_Tree_Get(Binary_Tree_t *Tree, void *Key, size_t KeySize) {

    Binary_Tree_Node_t *Node = NULL;

    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot search for Key.");
#endif
        return NULL;
    }

    if ( (0 == KeySize) && (0 != Tree->KeySize) ) {
#ifdef DEBUG
        DEBUG_PRINTF("Note: Using cached KeySize of [ %lu ].", (unsigned long)Tree->KeySize);
#endif
        KeySize = Tree->KeySize;
    }

    Node = Binary_Tree_find(Tree->Root, Key, KeySize, Tree->KeyCompareFunc);
    if ( NULL == Node ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to find Key in Tree.");
#endif
        return NULL;
    }

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully returned requested item from Binary_Tree_t.");
#endif
    return Node->Value.ValueRaw;
}

Binary_Tree_KeyValuePair_t Binary_Tree_Pop(Binary_Tree_t *Tree, void *Key, size_t KeySize) {

    Binary_Tree_Node_t *       Node         = NULL;
    Binary_Tree_KeyValuePair_t KeyValuePair = {NULL, NULL};

    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot pop Key.");
#endif
        return KeyValuePair;
    }

    if ( (0 == KeySize) && (0 != Tree->KeySize) ) {
#ifdef DEBUG
        DEBUG_PRINTF("Note: Using cached KeySize of [ %lu ].", (unsigned long)Tree->KeySize);
#endif
        KeySize = Tree->KeySize;
    }

    Iterator_Invalidate(&(Tree->Iterator));

    Node = Binary_Tree_find(Tree->Root, Key, KeySize, Tree->KeyCompareFunc);
    if ( NULL == Node ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to find Key in Tree.");
#endif
        return KeyValuePair;
    }

    KeyValuePair.Key       = Node->Key.KeyRaw;
    KeyValuePair.Value     = Node->Value.ValueRaw;
    Node->KeyReleaseFunc   = NULL;
    Node->ValueReleaseFunc = NULL;

    Tree->Root = Binary_Tree_removeNode(Tree->Root, Key, KeySize, Tree->KeyCompareFunc);

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully removed requested item from Tree.");
#endif
    Tree->TreeSize -= 1;
    return KeyValuePair;
}

int Binary_Tree_Remove(Binary_Tree_t *Tree, void *Key, size_t KeySize) {

    bool DecrementSize = false;

    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot remove Key.");
#endif
        return 1;
    }

    if ( (0 == KeySize) && (0 != Tree->KeySize) ) {
#ifdef DEBUG
        DEBUG_PRINTF("Note: Using cached KeySize of [ %lu ].", (unsigned long)Tree->KeySize);
#endif
        KeySize = Tree->KeySize;
    }

    Iterator_Invalidate(&(Tree->Iterator));

    DecrementSize = Binary_Tree_KeyExists(Tree, Key, KeySize);

    Tree->Root = Binary_Tree_removeNode(Tree->Root, Key, KeySize, Tree->KeyCompareFunc);

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully removed requested item from Tree.");
#endif
    Tree->TreeSize -= (1 && DecrementSize);
    return 0;
}

int Binary_Tree_DoCallback(Binary_Tree_t *Tree, Binary_Tree_Direction_t Direction,
                           CallbackFunc_t *Callback) {

    Binary_Tree_KeyValuePair_t KeyValuePair = {NULL, NULL};
    int                        RetVal       = 0;

    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided.");
#endif
        return -1;
    }

    if ( NULL == Tree->Root ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: Tree is empty, no items to call Callback with.");
#endif
        return 0;
    }

    if ( NULL == Callback ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: NULL Callback* provided, nothing to call.");
#endif
        return 0;
    }

    switch ( Direction ) {
        case Direction_InOrder: break;
        case Direction_PreOrder: break;
        case Direction_PostOrder: break;
        default:
#ifdef DEBUG
            DEBUG_PRINTF("Error: Unknown tree traversal direction [ %d ].", Direction);
#endif
            return -1;
    }

    BINARY_TREE_FOREACH(Tree, KeyValuePair, Direction) {
        if ( 0 != Callback(&(KeyValuePair)) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Warning: Callback function returned non-zero.");
#endif
            RetVal += 1;
        }
    }

    return RetVal;
}

int Binary_Tree_DoCallbackArg(Binary_Tree_t *Tree, Binary_Tree_Direction_t Direction,
                              CallbackArgFunc_t *Callback, void *Args) {

    Binary_Tree_KeyValuePair_t KeyValuePair = {NULL, NULL};
    int                        RetVal       = 0;

    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided.");
#endif
        return -1;
    }

    if ( NULL == Tree->Root ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: Tree is empty, no items to call Callback with.");
#endif
        return 0;
    }

    if ( NULL == Callback ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: NULL Callback* provided, nothing to call.");
#endif
        return 0;
    }

    switch ( Direction ) {
        case Direction_InOrder: break;
        case Direction_PreOrder: break;
        case Direction_PostOrder: break;
        default:
#ifdef DEBUG
            DEBUG_PRINTF("Error: Unknown tree traversal direction [ %d ].", Direction);
#endif
            return -1;
    }

    BINARY_TREE_FOREACH(Tree, KeyValuePair, Direction) {
        if ( 0 != Callback(&(KeyValuePair), Args) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Warning: Callback function returned non-zero.");
#endif
            RetVal += 1;
        }
    }

    return RetVal;
}

int Binary_Tree_Clear(Binary_Tree_t *Tree) {

    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "NULL Tree* provided, nothing to clear.");
#endif
        return 0;
    }

    Iterator_Invalidate(&(Tree->Iterator));

    Binary_Tree_Node_Release(Tree->Root);
    Tree->Root     = NULL;
    Tree->TreeSize = 0;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully released all items from Binary_Tree.");
#endif
    return 0;
}

void Binary_Tree_Release(Binary_Tree_t *Tree) {

    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Warning: NULL Tree* provided, nothing to release.");
#endif
        return;
    }

    Binary_Tree_Node_Release(Tree->Root);

    ZERO_CONTAINER(Tree, Binary_Tree_t);
    free(Tree);

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully released Binary_Tree_t and all held values.");
#endif
    return;
}

/* ++++++++++ Private Functions ++++++++++ */

Binary_Tree_Node_t *Binary_Tree_insertNode(Binary_Tree_Node_t *Root, CompareFunc_t *KeyCompareFunc,
                                           Binary_Tree_Node_t *Node) {

    size_t MinKeySize    = 0;
    int    CompareResult = 0;

    if ( NULL == Root ) {
        return Node;
    }

    MinKeySize    = (Root->KeySize > Node->KeySize) ? Node->KeySize : Root->KeySize;
    CompareResult = KeyCompareFunc(Root->Key.KeyRaw, Node->Key.KeyRaw, MinKeySize);

    if ( CompareResult > 0 ) {
        Root->LeftChild         = Binary_Tree_insertNode(Root->LeftChild, KeyCompareFunc, Node);
        Root->LeftChild->Parent = Root;
    } else if ( CompareResult < 0 ) {
        Root->RightChild         = Binary_Tree_insertNode(Root->RightChild, KeyCompareFunc, Node);
        Root->RightChild->Parent = Root;
    } else {
        Binary_Tree_Node_UpdateValue(Root, Node->Value.ValueRaw, Node->ValueSize);
        Binary_Tree_Node_Release(Node);
    }

    return Binary_Tree_rebalance(Root);
}

Binary_Tree_Node_t *Binary_Tree_removeNode(Binary_Tree_Node_t *Root, void *Key, size_t KeySize,
                                           CompareFunc_t *KeyCompareFunc) {

    Binary_Tree_Node_t *Parent = NULL, *Child = NULL, *Successor = NULL;
    size_t              MinKeySize;
    int                 CompareResult;

    if ( NULL == Root ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Node* provided.");
#endif
        return NULL;
    }

    MinKeySize    = (Root->KeySize > KeySize) ? KeySize : Root->KeySize;
    CompareResult = KeyCompareFunc(Root->Key.KeyRaw, Key, MinKeySize);

    if ( CompareResult > 0 ) {
        Root->LeftChild = Binary_Tree_removeNode(Root->LeftChild, Key, KeySize, KeyCompareFunc);
    } else if ( CompareResult < 0 ) {
        Root->RightChild = Binary_Tree_removeNode(Root->RightChild, Key, KeySize, KeyCompareFunc);
    } else {

        Parent = Root->Parent;
        if ( (NULL != Root->LeftChild) && (NULL != Root->RightChild) ) {
            /*
                The node has 2 children.

                Replace the node with it's in-order successor, then remove the in-order successor
                with a recursive call to this function.
            */
            Successor = Binary_Tree_findMinimum(Root->RightChild);
            Binary_Tree_Node_UpdateKey(Root, Successor->Key.KeyRaw, Successor->KeySize);
            Binary_Tree_Node_UpdateValue(Root, Successor->Value.ValueRaw, Successor->ValueSize);
            Root->KeyReleaseFunc   = Successor->KeyReleaseFunc;
            Root->ValueReleaseFunc = Successor->ValueReleaseFunc;
            Root->RightChild       = Binary_Tree_removeNode(Root->RightChild, Successor->Key.KeyRaw,
                                                      Successor->KeySize, KeyCompareFunc);

        } else if ( (NULL != Root->LeftChild) && (NULL == Root->RightChild) ) {
            /*
                The node has 1 child.

                Replace the node with its child. Update the child's parent to be Root's parent,
                and update the required Child pointer in the parent. Then release the Root.
            */
            Child         = Root->LeftChild;
            Child->Parent = Root->Parent;
            if ( NULL != Parent ) {
                if ( Root == Parent->LeftChild ) {
                    Parent->LeftChild = Child;
                } else if ( Root == Parent->RightChild ) {
                    Parent->RightChild = Child;
                }
            }
            Root->LeftChild = NULL;
            Root->Parent    = NULL;
            Binary_Tree_Node_Release(Root);
            Root = Child;

        } else if ( (NULL == Root->LeftChild) && (NULL != Root->RightChild) ) {
            /*
                The node has 1 child.

                Replace the node with its child. Update the child's parent to be Root's parent,
                and update the required Child pointer in the parent. Then release the Root.
            */
            Child         = Root->RightChild;
            Child->Parent = Root->Parent;
            if ( NULL != Parent ) {
                if ( Root == Parent->LeftChild ) {
                    Parent->LeftChild = Child;
                } else if ( Root == Parent->RightChild ) {
                    Parent->RightChild = Child;
                }
            }
            Root->RightChild = NULL;
            Root->Parent     = NULL;
            Binary_Tree_Node_Release(Root);
            Root = Child;

        } else {
            /*
                The node has 0 children.

                Simply delete the node and clear all references to it.
            */
            Binary_Tree_Node_Release(Root);
            Root = NULL;
        }
    }

    return Binary_Tree_rebalance(Root);
}

Binary_Tree_Node_t *Binary_Tree_find(Binary_Tree_Node_t *Root, void *Key, size_t KeySize,
                                     CompareFunc_t *KeyCompareFunc) {

    size_t MinKeySize    = 0;
    int    CompareResult = 0;

    if ( NULL == Root ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Warning: NULL Node reached without finding Key.");
#endif
        return NULL;
    }

    MinKeySize    = (Root->KeySize > KeySize) ? KeySize : Root->KeySize;
    CompareResult = KeyCompareFunc(Root->Key.KeyRaw, Key, MinKeySize);

    if ( 0 == CompareResult ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Successfully found Key within Tree.");
#endif
        return Root;
    } else if ( CompareResult > 0 ) {
        return Binary_Tree_find(Root->LeftChild, Key, KeySize, KeyCompareFunc);
    } else {
        return Binary_Tree_find(Root->RightChild, Key, KeySize, KeyCompareFunc);
    }
}

Binary_Tree_Node_t *Binary_Tree_findMinimum(Binary_Tree_Node_t *Root) {
    for ( ; NULL != Root->LeftChild; Root = Root->LeftChild ) { ; };
    return Root;
}

bool Binary_Tree_isAVLTree(Binary_Tree_Node_t *Root) {

    int BalanceFactor = 0;

    if ( NULL == Root ) {
        return true;
    }

    BalanceFactor = Binary_Tree_balanceFactor(Root);

    if ( (BalanceFactor >= 2) || (BalanceFactor <= -2) ) {
        return false;
    }

    return Binary_Tree_isAVLTree(Root->LeftChild) && Binary_Tree_isAVLTree(Root->RightChild);
}

/* ---------- Private Functions ---------- */
