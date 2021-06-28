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

#ifndef LIBCONTAINER_BINARY_TREE_H
#define LIBCONTAINER_BINARY_TREE_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define LIBCONTAINER_ENABLE_BINARY_TREE
#include "../../../../include/libcontainer.h"

#include "../../../iterator/include/iterator.h"

#include "binary_tree_node.h"
#include "tree_rotations.h"

struct Binary_Tree_t {

    /*
        A Binary_Tree_t simply contains a pointer to the current root node.
        This root may be NULL, in which case the tree is empty. If the root
        is non-null, it will contain a value, as well as optionally up to 2
        children. Each of these children are recursively equivalent to roots
        of their subtrees.
    */
    Binary_Tree_Node_t* Root;

    /*
        ReleaseFunc contains a cached function pointer to provide to each of the
        Nodes within the tree so they can release their contents on removal or
        tree destruction.
    */
    ReleaseFunc_t* ValueReleaseFunc;

    /*
        KeyReleaseFunc is a pointer to the function to call to safely release the
        resources associated with the Key when the Node is released.
    */
    ReleaseFunc_t* KeyReleaseFunc;

    /*
        KeyCompareFunc is the generalized comparison function to apply to determine
        the ordering of the Keys within the Tree.
    */
    CompareFunc_t* KeyCompareFunc;

    /*
        Iterator is the struct containing the necessary information and context
        to be able to single-step iterate over the Binary Tree in an arbitrary direction.
    */
    Iterator_t* Iterator;

    /*
        The total number of elements in the tree. Allows O(1) checks of the
        length or size of the tree.
    */
    size_t TreeSize;

    /*
        KeySize caches the size of the Keys held by this tree. If this is set to 0,
        the tree will only hold references to the items, and not own the resources itself.
    */
    size_t KeySize;

    /*
        DuplicatePolicy notes how the tree will handle duplicate Keys.
    */
    Binary_Tree_DuplicatePolicy_t DuplicatePolicy;
};

/* ++++++++++ Private Functions ++++++++++ */

/*
    Binary_Tree_find

    This function recursively searches a binary search tree for the given Key.

    Inputs:
    Root    -   Pointer to the current (sub)tree root node.
    Key     -   Key value to search for.

    Outputs:
    Binary_Tree_Node_t* -   Pointer to the node which has the desired Key, or NULL if it doesn't exist.
*/
Binary_Tree_Node_t* Binary_Tree_find(Binary_Tree_Node_t* Root, void* Key, size_t KeySize, CompareFunc_t* KeyCompareFunc);

/*
    Binary_Tree_findMinimum

    This function acts similarly to Binary_Tree_find(), but specifically searches for the
    item with the minimal Key in a given (sub)tree.

    Inputs:
    Root    -   Pointer to the current (sub)tree root node.

    Outputs:
    Binary_Tree_Node_t* -   Pointer to the node which has the desired Key.

    Note:
    This function will never return NULL as the arguments are pre-validated
    and the sub-tree is assured to exist before this is called. At worst,
    this will simply return the original pointer.
*/
Binary_Tree_Node_t* Binary_Tree_findMinimum(Binary_Tree_Node_t* Root);

/*
    Binary_Tree_insertNode

    This function inserts a given Node into a (sub)tree, recursively descending
    to ensure it is placed correctly for its Key.

    Inputs:
    Root    -   Pointer to the current (sub)tree root node.
    Node    -   Pointer to the Node to be added to the tree.

    Outputs:
    Binary_Tree_Node_t* -   Pointer to the (possibly updated) Root node of the
                                sub-tree the node was inserted into.

    Note:
    The binary tree implemented by this library is an AVL tree. As such,
    when inserting new items, this may involve some number of tree rotations.
    This can shift which node is the Root of a given sub-tree, which is why
    this returns a pointer to a Node.
*/
Binary_Tree_Node_t* Binary_Tree_insertNode(Binary_Tree_Node_t* Root, CompareFunc_t* KeyCompareFunc, Binary_Tree_Node_t* Node);

/*
    Binary_Tree_removeNode

    This function removes the Node associated with the given Key from the tree,
    recursively descending to find the necessary Node.

    Inputs:
    Root    -   Pointer to the current (sub)tree root node.
    Key     -   Key value of the Node to remove.

    Binary_Tree_Node_t* -   Pointer to the (possibly updated) Root node of the
                                sub-tree the node was inserted into.

    Note:
    The binary tree implemented by this library is an AVL tree. As such,
    when removing items, this may involve some number of tree rotations.
    This can shift which node is the Root of a given sub-tree, which is why
    this returns a pointer to a Node.
*/
Binary_Tree_Node_t* Binary_Tree_removeNode(Binary_Tree_Node_t *Root, void* Key, size_t KeySize, CompareFunc_t* KeyCompareFunc);

/*
    Binary_Tree_leftmostLeaf

    This function searches the subtree, finding the left-most leaf, which may or may not
    be the left-most node. This is a helpful behaviour for the single-step iteration
    functions.

    Inputs:
    Root    -   The root of the sub-tree to search through.

    Outputs:
    Binary_Tree_Node_t* -   A pointer to the resulting left-most leaf node, or NULL
                                on error or if it doesn't exist.
*/
Binary_Tree_Node_t* Binary_Tree_leftmostLeaf(Binary_Tree_Node_t *Root);

/*
    Binary_Tree_isAVLTree

    This function is a testing/validation function for asserting each and every
    Node within the Tree satisfies the AVL invariant (Balance Factor in { -1, 0, +1 }).

    Inputs:
    Root    -   Pointer to the (sub)tree to check the AVL invariant for.

    Outputs:
    bool    -   Returns true if the given tree satisfies the AVL invariant.
*/
bool Binary_Tree_isAVLTree(Binary_Tree_Node_t* Root);

/* ---------- Private Functions ---------- */

#if defined(TESTING) || defined(DEBUGGER)

#include "binary_tree_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
