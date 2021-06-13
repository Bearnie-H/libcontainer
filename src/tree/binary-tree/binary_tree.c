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

    Tree = (Binary_Tree_t *)calloc(1, sizeof(Binary_Tree_t));
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

int Binary_Tree_Insert(Binary_Tree_t *Tree, int Key, void *Value) {

    Binary_Tree_Node_t *NewRoot = NULL, *NewNode = NULL;
    bool IncrementSize = false;

    if (NULL == Tree) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot insert new value.");
        return 1;
    }

    if (NULL == Value) {
        DEBUG_PRINTF("%s", "Note: NULL Value* provided, just inserting Key.");
    }

    /* Otherwise, this is a new Key, and must be added to the tree as a new Node. */
    NewNode = Binary_Tree_Node_Create(Key, Tree->ValueSize, Value, Tree->ReleaseFunc);
    if (NULL == NewNode) {
        DEBUG_PRINTF("Error: Failed to create new Binary_Tree_Node_t with Key [ %d ].", Key);
        return 1;
    }

    IncrementSize = !(Binary_Tree_KeyExists(Tree, Key));

    NewRoot = Binary_Tree_insertNode(Tree->Root, NewNode);
    if (NULL == NewRoot) {
        DEBUG_PRINTF("Error: Failed to insert new Binary_Tree_Node_t with Key [ %d ].", Key);
        Binary_Tree_Node_Release(NewNode);
        return 1;
    }

    Tree->Root = NewRoot;
    Tree->TreeSize += (1 && IncrementSize);
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

    NodeContents = (void *)Node->Value;
    Node->Value = NULL;

    Tree->Root = Binary_Tree_removeNode(Tree->Root, Key);

    DEBUG_PRINTF("Successfully removed item with Key [ %d ] from Tree.", Key);
    Tree->TreeSize -= 1;
    return NodeContents;
}

int Binary_Tree_Remove(Binary_Tree_t *Tree, int Key) {

    bool DecrementSize = false;

    if (NULL == Tree) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot remove Key.");
        return 1;
    }

    DecrementSize = Binary_Tree_KeyExists(Tree, Key);

    Tree->Root = Binary_Tree_removeNode(Tree->Root, Key);

    DEBUG_PRINTF("Successfully removed item with Key [ %d ] from Tree.", Key);
    Tree->TreeSize -= (1 && DecrementSize);
    return 0;
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

int Binary_Tree_Clear(Binary_Tree_t *Tree) {

    if (NULL == Tree) {
        DEBUG_PRINTF("%s", "NULL Tree* provided, nothing to clear.");
        return 0;
    }

    Binary_Tree_Node_Release(Tree->Root);
    Tree->Root = NULL;
    Tree->TreeSize = 0;

    DEBUG_PRINTF("%s", "Successfully released all items from Binary_Tree.");
    return 0;
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

Binary_Tree_Node_t *Binary_Tree_removeNode(Binary_Tree_Node_t *Root, int Key) {

    Binary_Tree_Node_t *Parent = NULL, *Child = NULL, *Successor = NULL;

    if (NULL == Root) {
        DEBUG_PRINTF("%s", "Error: NULL Node* provided.");
        return NULL;
    }

    if (Key < Root->Key) {
        Root->LeftChild = Binary_Tree_removeNode(Root->LeftChild, Key);
    } else if (Key > Root->Key) {
        Root->RightChild = Binary_Tree_removeNode(Root->RightChild, Key);
    } else {

        Parent = Root->Parent;
        if ((NULL != Root->LeftChild) && (NULL != Root->RightChild)) {
            /*
                The node has 2 children.

                Replace the node with it's in-order successor, then remove the in-order successor
                with a recursive call to this function.
            */
            Successor = Binary_Tree_findMinimum(Root->RightChild);
            Root->Key = Successor->Key;
            Root->ReleaseFunc = Successor->ReleaseFunc;
            Binary_Tree_Node_Update(Root, Successor->Value, Successor->ValueSize);
            Root->RightChild = Binary_Tree_removeNode(Root->RightChild, Successor->Key);
        } else if ((NULL != Root->LeftChild) && (NULL == Root->RightChild)) {
            /*
                The node has 1 child.

                Replace the node with its child. Update the child's parent to be Root's parent,
                and update the required Child pointer in the parent. Then release the Root.
            */
            Child = Root->LeftChild;
            Child->Parent = Root->Parent;
            if (NULL != Parent) {
                if (Root == Parent->LeftChild) {
                    Parent->LeftChild = Child;
                } else if (Root == Parent->RightChild) {
                    Parent->RightChild = Child;
                }
            }
            Root->LeftChild = NULL;
            Binary_Tree_Node_Release(Root);
            Root = Child;
        } else if ((NULL == Root->LeftChild) && (NULL != Root->RightChild)) {
            /*
                The node has 1 child.

                Replace the node with its child. Update the child's parent to be Root's parent,
                and update the required Child pointer in the parent. Then release the Root.
            */
            Child = Root->RightChild;
            Child->Parent = Root->Parent;
            if (NULL != Parent) {
                if (Root == Parent->LeftChild) {
                    Parent->LeftChild = Child;
                } else if (Root == Parent->RightChild) {
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

bool Binary_Tree_isAVLTree(Binary_Tree_Node_t *Root) {

    int BalanceFactor = 0;

    if (NULL == Root) {
        return true;
    }

    BalanceFactor = Binary_Tree_balanceFactor(Root);

    if ((BalanceFactor >= 2) || (BalanceFactor <= -2)) {
        return false;
    }

    return Binary_Tree_isAVLTree(Root->LeftChild) && Binary_Tree_isAVLTree(Root->RightChild);
}

/* ---------- Private Functions ---------- */
