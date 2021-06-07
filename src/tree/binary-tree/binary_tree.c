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

Binary_Tree_t *Binary_Tree_Create(size_t ValueSize, ReleaseFunc_t *ReleaseFunc) {

    Binary_Tree_t *Tree = NULL;

    if (NULL == ReleaseFunc) {
        DEBUG_PRINTF("%s", "Note: NULL ReleaseFunc* provided, defaulting to free().");
        ReleaseFunc = free;
    }

    if (0 == ValueSize) {
        DEBUG_PRINTF("%s",
                     "Note: 0 ValueSize provided, creating Binary_Tree_t of Reference-Types.");
    }

    Tree = (Binary_Tree_t *)malloc(sizeof(Binary_Tree_t));
    if (NULL == Tree) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Binary_Tree_t.");
        return NULL;
    }

    Tree->ReleaseFunc = ReleaseFunc;
    Tree->Root = NULL;
    Tree->TreeSize = 0;
    Tree->ValueSize = ValueSize;

    DEBUG_PRINTF("%s", "Successfully created new Binary_Tree_t.");
    return Tree;
}

size_t Binary_Tree_Length(Binary_Tree_t *Tree) {

    if (NULL == Tree) {
        DEBUG_PRINTF("%s", "Warning: NULL Tree* provided, unable to report length.");
        return 0;
    }

    return Tree->TreeSize;
}

int Binary_Tree_Insert(Binary_Tree_t *Tree, int Key, const void *Value) {

    Binary_Tree_Node_t *NewRoot = NULL, *NewNode = NULL;

    if (NULL == Tree) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot insert new value.");
        return 1;
    }

    if (NULL == Value) {
        DEBUG_PRINTF("%s", "Note: NULL Value* provided, nothing to insert.");
        return 0;
    }

    /* Otherwise, this is a new Key, and must be added to the tree as a new Node. */
    NewNode = Binary_Tree_Node_Create(Key, Tree->ValueSize, Value, Tree->ReleaseFunc);
    if (NULL == NewNode) {
        DEBUG_PRINTF("Error: Failed to create new Binary_Tree_Node_t with Key [ %d ].", Key);
        return 1;
    }

    NewRoot = Binary_Tree_insertNode(Tree->Root, NewNode);
    if (NULL == NewRoot) {
        DEBUG_PRINTF("Error: Failed to insert new Binary_Tree_Node_t with Key [ %d ].", Key);
        Binary_Tree_Node_Release(NewNode);
        return 1;
    }

    Tree->Root = NewRoot;
    Tree->TreeSize += 1;
    return 0;
}

bool Binary_Tree_KeyExists(Binary_Tree_t *Tree, int Key) {
    return (NULL != Binary_Tree_Get(Tree, Key));
}

void *Binary_Tree_Get(Binary_Tree_t *Tree, int Key) {

    Binary_Tree_Node_t *Node = NULL;

    if (NULL == Tree) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot search for Key.");
        return NULL;
    }

    Node = Binary_Tree_find(Tree->Root, Key);
    if (NULL == Node) {
        DEBUG_PRINTF("%s", "Error: Failed to find Key in Tree.");
        return NULL;
    }

    DEBUG_PRINTF("Successfully returned Value with Key [ %d ] from Binary_Tree_t.", Key);
    return (void *)(Node->Value);
}

void *Binary_Tree_Pop(Binary_Tree_t *Tree, int Key) {

    Binary_Tree_Node_t *Node = NULL;
    void *NodeContents = NULL;

    if (NULL == Tree) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot pop Key.");
        return NULL;
    }

    Node = Binary_Tree_find(Tree->Root, Key);
    if (NULL == Node) {
        DEBUG_PRINTF("%s", "Error: Failed to find Key in Tree.");
        return NULL;
    }

    NodeContents = Node->Value;
    Node->Value = NULL;

    if (0 != Binary_Tree_removeNode(Tree, Node)) {
        DEBUG_PRINTF("Error: Failed to delete Node with Key [ %d ] from Binary Tree.", Key);
        Node->Value = NodeContents;
        return NULL;
    }

    Tree->TreeSize -= 1;
    Tree->Root = Binary_Tree_rebalance(Tree->Root);
    DEBUG_PRINTF("Successfully popped Value with Key [ %d ] from Binary Tree.", Key);
    return NodeContents;
}

int Binary_Tree_DoCallback(Binary_Tree_t *Tree, CallbackFunc_t *Callback) {

    if (NULL == Tree) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided.");
        return 1;
    }

    if (NULL == Tree->Root) {
        DEBUG_PRINTF("%s", "Note: Tree is empty, no items to call Callback with.");
        return 0;
    }

    if (NULL == Callback) {
        DEBUG_PRINTF("%s", "Note: NULL Callback* provided, nothing to call.");
        return 0;
    }

    return Binary_Tree_doCallback(Tree->Root, Callback);
}

int Binary_Tree_DoCallbackArg(Binary_Tree_t *Tree, CallbackArgFunc_t *Callback, void *Args) {

    if (NULL == Tree) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided.");
        return 1;
    }

    if (NULL == Tree->Root) {
        DEBUG_PRINTF("%s", "Note: Tree is empty, no items to call Callback with.");
        return 0;
    }

    if (NULL == Callback) {
        DEBUG_PRINTF("%s", "Note: NULL Callback* provided, nothing to call.");
        return 0;
    }

    return Binary_Tree_doCallbackArg(Tree->Root, Callback, Args);
}

int Binary_Tree_Remove(Binary_Tree_t *Tree, int Key) {

    Binary_Tree_Node_t *Node = NULL;

    if (NULL == Tree) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot remove Key.");
        return 1;
    }

    Node = Binary_Tree_find(Tree->Root, Key);
    if (NULL == Node) {
        DEBUG_PRINTF("%s", "Error: Failed to find Key in Tree.");
        return 1;
    }

    if (0 != Binary_Tree_removeNode(Tree, Node)) {
        DEBUG_PRINTF("Error: Failed to remove Node with Key [ %d ] from Binary Tree.", Key);
        return 1;
    }

    Tree->TreeSize -= 1;
    Tree->Root = Binary_Tree_rebalance(Tree->Root);
    DEBUG_PRINTF("Successfully removed item with Key [ %d ] from Tree.", Key);
    return 1;
}

void Binary_Tree_Release(Binary_Tree_t *Tree) {

    if (NULL == Tree) {
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

Binary_Tree_Node_t *Binary_Tree_insertNode(Binary_Tree_Node_t *Root, Binary_Tree_Node_t *Node) {

    if (NULL == Root) {
        return Node;
    } else if (Root->Key > Node->Key) {
        Root->LeftChild = Binary_Tree_insertNode(Root->LeftChild, Node);
    } else if (Root->Key < Node->Key) {
        Root->RightChild = Binary_Tree_insertNode(Root->RightChild, Node);
    } else {
        Binary_Tree_Node_Update(Root, Node->Value, Node->ValueSize);
        Binary_Tree_Node_Release(Node);
    }

    return Binary_Tree_rebalance(Root);
}

Binary_Tree_Node_t *Binary_Tree_find(Binary_Tree_Node_t *Root, int Key) {

    if (NULL == Root) {
        DEBUG_PRINTF("%s", "Warning: NULL Node reached without finding Key.");
        return NULL;
    }

    if (Key == Root->Key) {
        DEBUG_PRINTF("%s", "Successfully found Key within Tree.");
        return Root;
    } else if (Key < Root->Key) {
        return Binary_Tree_find(Root->LeftChild, Key);
    } else {
        return Binary_Tree_find(Root->RightChild, Key);
    }
}

Binary_Tree_Node_t *Binary_Tree_findMinimum(Binary_Tree_Node_t *Root) {
    for (; NULL != Root->LeftChild; Root = Root->LeftChild) {
        ;
    };
    return Root;
}

int Binary_Tree_removeNode(Binary_Tree_t *Tree, Binary_Tree_Node_t *Node) {

    // Binary_Tree_Node_t *Child = NULL;

    if ((NULL == Tree) || (NULL == Node)) {
        DEBUG_PRINTF("%s", "NULL Tree* or Node* provided.");
        return 1;
    }

    /*
        If the node has no children, simply delete it.

        If the node has one child, simply replace the node with that child.

        If the node has two children:
            Find the minimum node in the right sub-tree.
            Replace Node with this minimum node.
            Delete the original minumum node, recursing if necessary
    */
    if ((NULL == Node->LeftChild) && (NULL == Node->RightChild)) {
        /* ... */
    } else if ((NULL != Node->LeftChild) && (NULL == Node->RightChild)) {
        /* ... */
    } else if ((NULL == Node->LeftChild) && (NULL != Node->RightChild)) {
        /* ... */
    } else {
        /* ... */
    }

    return 1;
}

int Binary_Tree_doCallback(Binary_Tree_Node_t *Root, CallbackFunc_t *Callback) {

    void **KeyValuePair[2] = {NULL, NULL};
    int RetVal = 0;

    if (NULL != Root) {

        KeyValuePair[0] = (void *)&(Root->Key);
        KeyValuePair[1] = (void *)(Root->Value);

        RetVal |= Binary_Tree_doCallback(Root->LeftChild, Callback);
        RetVal |= Callback(KeyValuePair);
        RetVal |= Binary_Tree_doCallback(Root->RightChild, Callback);
    }

    return RetVal;
}

int Binary_Tree_doCallbackArg(Binary_Tree_Node_t *Root, CallbackArgFunc_t *Callback, void *Args) {

    void **KeyValuePair[2] = {NULL, NULL};
    int RetVal = 0;

    if (NULL != Root) {

        KeyValuePair[0] = (void *)&(Root->Key);
        KeyValuePair[1] = (void *)(Root->Value);

        RetVal |= Binary_Tree_doCallbackArg(Root->LeftChild, Callback, Args);
        RetVal |= Callback(KeyValuePair, Args);
        RetVal |= Binary_Tree_doCallbackArg(Root->RightChild, Callback, Args);
    }

    return RetVal;
}

/* ---------- Private Functions ---------- */
