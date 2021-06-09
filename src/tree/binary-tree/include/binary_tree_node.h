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

#ifndef LIBCONTAINER_BINARY_TREE_NODE_H
#define LIBCONTAINER_BINARY_TREE_NODE_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "../../../../include/libcontainer.h"

/*
    Binary_Tree_Node_t

    This represents a single node within a Binary Search Tree.
    This node has two children, as well as a key-value pair.
    The key must be a unique integer, but the value can be
    any arbitrary data. The values for all nodes within a tree
    must be homogeneous, but two trees may hold different types.
*/
typedef struct Binary_Tree_Node_t Binary_Tree_Node_t;

struct Binary_Tree_Node_t {

    /*
        A node maintains pointers to its parent to simplify the
        tree rotations needed for self-balancing.
    */
    Binary_Tree_Node_t* Parent;

    /*
        A node must also contain pointers to it's left and right sub-trees
        to be a binary tree. These may be NULL.
    */
    Binary_Tree_Node_t* LeftChild;
    Binary_Tree_Node_t* RightChild;

    /*
        The value associated with this node of the Tree.
        It is the responsibility of the user to ensure
        type matching and sizing.
    */
    uint8_t* Value;

    /*
        ReleaseFunc holds the function to call to release the resources
        associated with the Value pointer.
    */
    ReleaseFunc_t* ReleaseFunc;

    /*
        The number of bytes of memory owned by the Node for Value.
        If zero, this indicates the Value is a Reference-Type, and memory
        is owned by something else.
    */
    size_t ValueSize;

    /*
        The raw integer key value to associate with this entry. This
        is the field on which the total pre-order of the Tree is based.

        This may change in the future to allow arbitrary Key types, similar
        to what is done with the Hashmap_t container.
    */
    int Key;
};

/*
    Binary_Tree_Node_Create

    This function creates a new Binary_Tree_Node_t for inserting into a Binary_Tree_t.
    This associates a Key to a given Value, as well as injecting the ReleaseFunc to
    properly manage resources held by the Value.

    Inputs:
    Key             -   The Key to associate with this Node
    ValueSize       -   The size of the Value, as measured in bytes (typically a sizeof() result)
    Value           -   Pointer to the Value to assign to this Node
    ReleaseFunc     -   Pointer to the function to call to release the resources associated with this Node

    Outputs:
    Binary_Tree_Node_t*     -   Pointer to the created and initialized Node, or NULL on failure.
*/
Binary_Tree_Node_t* Binary_Tree_Node_Create(int Key, size_t ValueSize, const void* Value, ReleaseFunc_t* ReleaseFunc);

/*
    Binary_Tree_Node_Height

    This function returns the height of the (sub)tree below the given Node. The height
    is defined as the maximum linear chain of children before reaching a leaf.

    Inputs:
    Root    -   Pointer to the Node to return the height of.

    Outputs:
    size_t  -   The height of the Binary Tree below this Node. If NULL is provided, returns 0.
*/
size_t Binary_Tree_Node_Height(Binary_Tree_Node_t* Root);

/*
    Binary_Tree_Node_Update

    This function updates the Value held by a Node to a new Value.

    Inputs:
    Node        -   Pointer to the Node to update the value for
    NewValue    -   Pointer to the new Value to assign to this Node
    ValueSize   -   The size of the Value, as measured in bytes (typically a sizeof() result)

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Binary_Tree_Node_Update(Binary_Tree_Node_t* Node, const void* NewValue, size_t ValueSize);

/*
    Binary_Tree_Node_Release

    This function fully release the given Node, all held resources
    and the subtrees of the Node.

    Inputs:
    Node    -   Pointer to the Node to release.

    Outputs:
    None, the Node and all held resources are fully released.

    Note:
    This will also release the full sub-trees of the Node.
    If this is not desired, set the Child pointers to NULL before calling this.
*/
void Binary_Tree_Node_Release(Binary_Tree_Node_t* Node);

#if defined(TESTING) || defined(DEBUGGER)

#include "binary_tree_node_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
