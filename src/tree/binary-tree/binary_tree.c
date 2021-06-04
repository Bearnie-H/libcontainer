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

#include "include/binary_tree.h"
#include "../../logging/logging.h"

Binary_Tree_t* Binary_Tree_Create(size_t ValueSize, ReleaseFunc_t* ReleaseFunc) {

    Binary_Tree_t* Tree = NULL;

    if ( NULL == ReleaseFunc ) {
        DEBUG_PRINTF("%s", "Note: NULL ReleaseFunc* provided, defaulting to free().");
        ReleaseFunc = free;
    }

    if ( 0 == ValueSize ) {
        DEBUG_PRINTF("%s", "Note: 0 ValueSize provided, creating Binary_Tree_t of Reference-Types.");
    }

    Tree = (Binary_Tree_t*)malloc(sizeof(Binary_Tree_t));
    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Binary_Tree_t.");
        return NULL;
    }

    Tree->ReleaseFunc = ReleaseFunc;
    Tree->Root = NULL;
    Tree->Size = 0;
    Tree->ValueSize = ValueSize;

    DEBUG_PRINTF("%s", "Successfully created new Binary_Tree_t.");
    return Tree;
}

size_t Binary_Tree_Length(Binary_Tree_t* Tree) {

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Warning: NULL Tree* provided, unable to report length.");
        return 0;
    }

    return Tree->Size;
}

int Binary_Tree_Insert(Binary_Tree_t* Tree, int Key, const void* Value) {

    Binary_Tree_Node_t* NewNode = NULL;
    Binary_Tree_Node_t* Parent = NULL;

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot insert new value.");
        return 1;
    }

    if ( NULL == Value ) {
        DEBUG_PRINTF("%s", "Note: NULL Value* provided, nothing to insert.");
        return 0;
    }

    /* Case 1: The tree is empty. Any node must become the root. */
    if ( NULL == Tree->Root ) {
        DEBUG_PRINTF("Successfully inserted new Value with Key [ %d ] as the Root Node.", Key);
        Tree->Root = NewNode;
        Tree->Size += 1;
        return 0;
    }

    /*
        Otherwise, the node has to be inserted based on it's ordering so find the parent it
        must be added under.
    */
    Parent = Binary_Tree_Node_FindParent(Tree->Root, Key);
    if ( NULL == Parent ) {
        DEBUG_PRINTF("Error: Failed to find suitable parent for new Value with Key [ %d ].", Key);
        return 1;
    }

    /* If the parent we found already has a child with the Key we want to insert, update this existing Node. */
    if (( NULL != Parent->LeftChild ) && (Key == Parent->LeftChild->Key)) {
        DEBUG_PRINTF("%s", "Note: Item with Key [ %d ] already in Tree, overwriting with new Value.");
        return Binary_Tree_Node_Update(Parent->LeftChild, Value, Tree->ValueSize);
    } else if ((NULL != Parent->RightChild) && ( Key == Parent->RightChild->Key)) {
        DEBUG_PRINTF("%s", "Note: Item with Key [ %d ] already in Tree, overwriting with new Value.");
        return Binary_Tree_Node_Update(Parent->LeftChild, Value, Tree->ValueSize);
    }

    /* Finally, this is a new Key, and must be added to the tree as a new Node. */
    NewNode = Binary_Tree_Node_Create(Key, Tree->ValueSize, Value, Tree->ReleaseFunc);
    if ( NULL == NewNode ) {
        DEBUG_PRINTF("Error: Failed to create new Binary_Tree_Node_t with Key [ %d ].", Key);
        return 1;
    }

    /* Add this new Node to the tree. */
    if ( Key < Parent->Key ) {
        Parent->LeftChild = NewNode;
    } else {
        Parent->RightChild = NewNode;
    }

    /* Finally, re-colour the tree, ensuring it satisfies the necessary Red-Black criteria. */

    Tree->Size += 1;
    return 0;
}

bool Binary_Tree_KeyExists(Binary_Tree_t* Tree, int Key) {
    return ( NULL != Binary_Tree_Get(Tree, Key));
}

void* Binary_Tree_Get(Binary_Tree_t* Tree, int Key) {

    Binary_Tree_Node_t* Node = NULL;

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot search for Key.");
        return NULL;
    }

    Node = Binary_Tree_Node_Find(Tree->Root, Key);
    if ( NULL == Node ) {
        DEBUG_PRINTF("%s", "Error: Failed to find Key in Tree.");
        return NULL;
    }

    return (void*)(Node->Value);
}

void* Binary_Tree_Pop(Binary_Tree_t* Tree, int Key) {

    Binary_Tree_Node_t* Node = NULL;
    void* NodeContents = NULL;

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot pop Key.");
        return NULL;
    }

    Node = Binary_Tree_Node_Find(Tree->Root, Key);
    if ( NULL == Node ) {
        DEBUG_PRINTF("%s", "Error: Failed to find Key in Tree.");
        return NULL;
    }

    NodeContents = Node->Value;
    Node->Value = NULL;

    /* To-Do: Figure out how best to remove the node... */

    Tree->Size -= 1;
    return NULL;
}

int* Binary_Tree_RootKey(Binary_Tree_t* Tree) {

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided.");
        return NULL;
    }

    if ( NULL == Tree->Root ) {
        DEBUG_PRINTF("%s", "Error: NULL Root, no key to return.");
        return NULL;
    }

    return &(Tree->Root->Key);
}

void* Binary_Tree_Next_PreOrder(Binary_Tree_t* Tree, int* CurrentKey) {

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, no Next item.");
        return NULL;
    }

    if ( NULL == CurrentKey ) {
        DEBUG_PRINTF("%s", "Error: NULL Key* provided, maybe try Binary_Tree_RootKey() first?");
        return NULL;
    }

    /* ... */

    return NULL;
}

void* Binary_Tree_Next_InOrder(Binary_Tree_t* Tree, int* CurrentKey) {

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, no Next item.");
        return NULL;
    }

    if ( NULL == CurrentKey ) {
        DEBUG_PRINTF("%s", "Error: NULL Key* provided, maybe try Binary_Tree_RootKey() first?");
        return NULL;
    }

    /* ... */

    return NULL;
}

void* Binary_Tree_Next_PostOrder(Binary_Tree_t* Tree, int* CurrentKey) {

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, no Next item.");
        return NULL;
    }

    if ( NULL == CurrentKey ) {
        DEBUG_PRINTF("%s", "Error: NULL Key* provided, maybe try Binary_Tree_RootKey() first?");
        return NULL;
    }

    /* ... */

    return NULL;
}

int Binary_Tree_Remove(Binary_Tree_t* Tree, int Key) {

    Binary_Tree_Node_t* Node = NULL;

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided, cannot remove Key.");
        return 1;
    }

    Node = Binary_Tree_Node_Find(Tree->Root, Key);
    if ( NULL == Node ) {
        DEBUG_PRINTF("%s", "Error: Failed to find Key in Tree.");
        return 1;
    }

    /* To-Do: Figure out how best to remove the node. */
    /* ... */

    Tree->Size -= 1;
    return 0;
}

void Binary_Tree_Release(Binary_Tree_t* Tree) {

    if ( NULL == Tree ) {
        DEBUG_PRINTF("%s", "Warning: NULL Tree* provided, nothing to release.");
        return;
    }

    Binary_Tree_Node_Release(Tree->Root);

    ZERO_CONTAINER(Tree, Binary_Tree_t);
    free(Tree);

    DEBUG_PRINTF("%s", "Successfully released Binary_Tree_t.");
    return;
}

/* ++++++++++ Private Functions ++++++++++ */

/* ... */

/* ---------- Private Functions ---------- */
