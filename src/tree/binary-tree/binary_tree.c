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
        DEBUG_PRINTF("%s", "Note: NULL KeyReleaseFunc* provided, defaulting to free().");
        KeyReleaseFunc = free;
    }

    if ( NULL == KeyCompareFunc ) {
        DEBUG_PRINTF("%s", "Note: NULL KeyCompareFunc provided, defaulting to memcmp().");
        KeyCompareFunc = memcmp;
    }

    if ( 0 == KeySize ) {
        DEBUG_PRINTF("%s",
                     "Note: 0 ValueSize provided, creating Binary_Tree_t of Reference-Types.");
    }

    Tree = (Binary_Tree_t *)calloc(1, sizeof(Binary_Tree_t));
    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Binary_Tree_t.");
        return NULL;
    }

    Tree->KeyReleaseFunc  = KeyReleaseFunc;
    Tree->KeyCompareFunc  = KeyCompareFunc;
    Tree->Root            = NULL;
    Tree->TreeSize        = 0;
    Tree->KeySize         = KeySize;
    Tree->DuplicatePolicy = Policy;

    DEBUG_PRINTF("%s", "Successfully created new Binary_Tree_t.");
    return Tree;
}

size_t Binary_Tree_Length(Binary_Tree_t *Tree) {

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Warning: NULL Tree* provided, unable to report length.");
        return 0;
    }

    return Tree->TreeSize;
}

int Binary_Tree_Insert(Binary_Tree_t *Tree, void *Key, size_t KeySize, void *Value,
                       size_t ValueSize, ReleaseFunc_t *ValueReleaseFunc) {

    Binary_Tree_Node_t *NewRoot = NULL, *NewNode = NULL;
    bool                IncrementSize = false;

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot insert new value.");
        return 1;
    }

    if ( NULL == Key ) {
        DEBUG_PRINTF("%s", "Error: NULL Key* Provided.");
        return 1;
    }

    if ( 0 == KeySize ) {
        if ( 0 != Tree->KeySize ) {
            DEBUG_PRINTF("Note: KeySize of 0 provided, using cached value from tree initialization "
                         "of [ %ld ].",
                         (unsigned long)Tree->KeySize);
            KeySize = Tree->KeySize;
        } else {
            DEBUG_PRINTF("%s", "Note: KeySize of 0 provided, treating as reference type.");
        }
    }

    if ( NULL != Value ) {
        if ( 0 == ValueSize ) {
            DEBUG_PRINTF("%s", "Note: ValueSize of 0 provided, treating as reference type.");
        }

        if ( NULL == ValueReleaseFunc ) {
            DEBUG_PRINTF("%s", "Note: NULL ValueReleaseFunc* provided, defaulting to free().");
            ValueReleaseFunc = free;
        }
    }

    Iterator_Invalidate(&(Tree->Iterator));

    if ( NULL == Value ) {
        DEBUG_PRINTF("%s", "Note: NULL Value* provided, just inserting Key.");
    }

    IncrementSize = !(Binary_Tree_KeyExists(Tree, Key, KeySize));

    if ( !IncrementSize ) {
        switch ( Tree->DuplicatePolicy ) {
            case Policy_Error:
                DEBUG_PRINTF("%s", "Error: Value with the given key already exists in the Tree!");
                return -1;
            default: break;
        }
    }

    /* Otherwise, this is a new Key, and must be added to the tree as a new Node. */
    NewNode = Binary_Tree_Node_Create(Key, KeySize, Tree->KeyReleaseFunc, Value, ValueSize,
                                      ValueReleaseFunc);
    if ( NULL == NewNode ) {
        DEBUG_PRINTF("%s", "Error: Failed to create new Binary_Tree_Node_t");
        return 1;
    }

    NewRoot = Binary_Tree_insertNode(Tree->Root, Tree->KeyCompareFunc, NewNode);
    if ( NULL == NewRoot ) {
        DEBUG_PRINTF("%s", "Error: Failed to insert new Binary_Tree_Node_t");
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
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot search for Key.");
        return false;
    }

    if ( (0 == KeySize) && (0 != Tree->KeySize) ) {
        DEBUG_PRINTF("Note: Using cached KeySize of [ %ld ].", (unsigned long)Tree->KeySize);
        KeySize = Tree->KeySize;
    }

    Node = Binary_Tree_find(Tree->Root, Key, KeySize, Tree->KeyCompareFunc);
    if ( NULL == Node ) {
        DEBUG_PRINTF("%s", "Error: Failed to find Key in Tree.");
        return false;
    }

    return true;
}

void *Binary_Tree_Get(Binary_Tree_t *Tree, void *Key, size_t KeySize) {

    Binary_Tree_Node_t *Node = NULL;

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot search for Key.");
        return NULL;
    }

    if ( (0 == KeySize) && (0 != Tree->KeySize) ) {
        DEBUG_PRINTF("Note: Using cached KeySize of [ %ld ].", (unsigned long)Tree->KeySize);
        KeySize = Tree->KeySize;
    }

    Node = Binary_Tree_find(Tree->Root, Key, KeySize, Tree->KeyCompareFunc);
    if ( NULL == Node ) {
        DEBUG_PRINTF("%s", "Error: Failed to find Key in Tree.");
        return NULL;
    }

    DEBUG_PRINTF("%s", "Successfully returned requested item from Binary_Tree_t.");
    return Node->Value.ValueRaw;
}

Binary_Tree_KeyValuePair_t Binary_Tree_Pop(Binary_Tree_t *Tree, void *Key, size_t KeySize) {

    Binary_Tree_Node_t *       Node         = NULL;
    Binary_Tree_KeyValuePair_t KeyValuePair = {NULL, NULL};

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot pop Key.");
        return KeyValuePair;
    }

    if ( (0 == KeySize) && (0 != Tree->KeySize) ) {
        DEBUG_PRINTF("Note: Using cached KeySize of [ %ld ].", (unsigned long)Tree->KeySize);
        KeySize = Tree->KeySize;
    }

    Iterator_Invalidate(&(Tree->Iterator));

    Node = Binary_Tree_find(Tree->Root, Key, KeySize, Tree->KeyCompareFunc);
    if ( NULL == Node ) {
        DEBUG_PRINTF("%s", "Error: Failed to find Key in Tree.");
        return KeyValuePair;
    }

    KeyValuePair.Key       = Node->Key.KeyRaw;
    KeyValuePair.Value     = Node->Value.ValueRaw;
    Node->ValueReleaseFunc = NULL;
    Node->KeyReleaseFunc   = NULL;

    Tree->Root = Binary_Tree_removeNode(Tree->Root, Key, KeySize, Tree->KeyCompareFunc);

    DEBUG_PRINTF("%s", "Successfully removed requested item from Tree.");
    Tree->TreeSize -= 1;
    return KeyValuePair;
}

int Binary_Tree_Remove(Binary_Tree_t *Tree, void *Key, size_t KeySize) {

    bool DecrementSize = false;

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot remove Key.");
        return 1;
    }

    if ( (0 == KeySize) && (0 != Tree->KeySize) ) {
        DEBUG_PRINTF("Note: Using cached KeySize of [ %ld ].", (unsigned long)Tree->KeySize);
        KeySize = Tree->KeySize;
    }

    Iterator_Invalidate(&(Tree->Iterator));

    DecrementSize = Binary_Tree_KeyExists(Tree, Key, KeySize);

    Tree->Root = Binary_Tree_removeNode(Tree->Root, Key, KeySize, Tree->KeyCompareFunc);

    DEBUG_PRINTF("%s", "Successfully removed requested item from Tree.");
    Tree->TreeSize -= (1 && DecrementSize);
    return 0;
}

int Binary_Tree_DoCallback(Binary_Tree_t *Tree, Binary_Tree_Direction_t Direction,
                           CallbackFunc_t *Callback) {

    Binary_Tree_KeyValuePair_t KeyValuePair = {0, NULL};
    size_t                     NodesVisited = 0;
    int                        RetVal       = 0;

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided.");
        return 1;
    }

    if ( NULL == Tree->Root ) {
        DEBUG_PRINTF("%s", "Note: Tree is empty, no items to call Callback with.");
        return 0;
    }

    if ( NULL == Callback ) {
        DEBUG_PRINTF("%s", "Note: NULL Callback* provided, nothing to call.");
        return 0;
    }

    switch ( Direction ) {
        case Direction_InOrder: break;
        case Direction_PreOrder: break;
        case Direction_PostOrder: break;
        default:
            DEBUG_PRINTF("Error: Unknown tree traversal direction [ %d ].", Direction);
            return 1;
    }

    NodesVisited = 0;
    BINARY_TREE_FOREACH(Tree, KeyValuePair, Direction) {
        NodesVisited++;
        if ( 0 != Callback(&(KeyValuePair)) ) {
            DEBUG_PRINTF("%s",
                         "Error: Callback function returned non-zero for item with Key [ %d ].");
            RetVal |= 1;
        }
    }

    Iterator_Invalidate(&(Tree->Iterator));

    if ( NodesVisited != Binary_Tree_Length(Tree) ) {
        DEBUG_PRINTF("Error: Callback function not called on as many elements as tree reports it "
                     "has. (%lu vs. %lu)",
                     (unsigned long)NodesVisited, (unsigned long)Binary_Tree_Length(Tree));
        return 1;
    }

    DEBUG_PRINTF("%s", "Finished performing Callback on Binary Tree.");
    return RetVal;
}

int Binary_Tree_DoCallbackArg(Binary_Tree_t *Tree, Binary_Tree_Direction_t Direction,
                              CallbackArgFunc_t *Callback, void *Args) {

    Binary_Tree_KeyValuePair_t KeyValuePair = {0, NULL};
    size_t                     NodesVisited = 0;
    int                        RetVal       = 0;

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided.");
        return 1;
    }

    if ( NULL == Tree->Root ) {
        DEBUG_PRINTF("%s", "Note: Tree is empty, no items to call Callback with.");
        return 0;
    }

    if ( NULL == Callback ) {
        DEBUG_PRINTF("%s", "Note: NULL Callback* provided, nothing to call.");
        return 0;
    }

    switch ( Direction ) {
        case Direction_InOrder: break;
        case Direction_PreOrder: break;
        case Direction_PostOrder: break;
        default:
            DEBUG_PRINTF("Error: Unknown tree traversal direction [ %d ].", Direction);
            return 1;
    }

    NodesVisited = 0;
    BINARY_TREE_FOREACH(Tree, KeyValuePair, Direction) {
        NodesVisited++;
        if ( 0 != Callback(&(KeyValuePair), Args) ) {
            DEBUG_PRINTF("%s",
                         "Error: Callback function returned non-zero for item with Key [ %d ].");
            RetVal |= 1;
        }
    }

    Iterator_Invalidate(&(Tree->Iterator));

    if ( NodesVisited != Binary_Tree_Length(Tree) ) {
        DEBUG_PRINTF("Error: Callback function not called on as many elements as tree reports it "
                     "has. (%lu vs. %lu)",
                     (unsigned long)NodesVisited, (unsigned long)Binary_Tree_Length(Tree));
        return 1;
    }

    DEBUG_PRINTF("%s", "Finished performing Callback on Binary Tree.");
    return RetVal;
}

int Binary_Tree_Clear(Binary_Tree_t *Tree) {

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "NULL Tree* provided, nothing to clear.");
        return 0;
    }

    Iterator_Invalidate(&(Tree->Iterator));

    Binary_Tree_Node_Release(Tree->Root);
    Tree->Root     = NULL;
    Tree->TreeSize = 0;

    DEBUG_PRINTF("%s", "Successfully released all items from Binary_Tree.");
    return 0;
}

void Binary_Tree_Release(Binary_Tree_t *Tree) {

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Warning: NULL Tree* provided, nothing to release.");
        return;
    }

    Binary_Tree_Node_Release(Tree->Root);

    ZERO_CONTAINER(Tree, Binary_Tree_t);
    free(Tree);

    DEBUG_PRINTF("%s", "Successfully released Binary_Tree_t and all held values.");
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
        DEBUG_PRINTF("%s", "Error: NULL Node* provided.");
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
            Root->RightChild = Binary_Tree_removeNode(Root->RightChild, Successor->Key.KeyRaw,
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
        DEBUG_PRINTF("%s", "Warning: NULL Node reached without finding Key.");
        return NULL;
    }

    MinKeySize    = (Root->KeySize > KeySize) ? KeySize : Root->KeySize;
    CompareResult = KeyCompareFunc(Root->Key.KeyRaw, Key, MinKeySize);

    if ( 0 == CompareResult ) {
        DEBUG_PRINTF("%s", "Successfully found Key within Tree.");
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
