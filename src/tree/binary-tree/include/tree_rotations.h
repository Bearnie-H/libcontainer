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

#ifndef LIBCONTAINER_TREE_ROTATIONS_H
#define LIBCONTAINER_TREE_ROTATIONS_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#include "binary_tree.h"

/*
    Binary_Tree_balanceFactor

    This function computes the AVL balance factor for the given sub-tree.

    Inputs:
    Root    -   The node to compute the balance factor at.

    Outputs:
    int     -   The balance factor of the given Node.
*/
int Binary_Tree_balanceFactor(Binary_Tree_Node_t* Root);

/*
    Binary_Tree_rebalance

    This function performs the necessary rotations to rebalance the AVL tree
    at the given Root node. This computes the balance factor at the node,
    determines the necessary rotations, and then performs them.

    Inputs:
    TreeRoot    -   Pointer to the current root to perform the tree rotation on.

    Outputs:
    Binary_Tree_Node_t*     -   Pointer to the (possibly new) subtree root Node.
*/
Binary_Tree_Node_t* Binary_Tree_rebalance(Binary_Tree_Node_t* TreeRoot);

/*
    Binary_Tree_rotateLeft

    This function performs a Left rotation, based at the given Node.

    Inputs:
    TreeRoot    -   Pointer to the current root to perform the tree rotation on.

    Outputs:
    Binary_Tree_Node_t*     -   Pointer to the (possibly new) subtree root Node.
*/
Binary_Tree_Node_t* Binary_Tree_rotateLeft(Binary_Tree_Node_t* TreeRoot);

/*
    Binary_Tree_rotateRight

    This function performs a Right rotation, based at the given Node.

    Inputs:
    TreeRoot    -   Pointer to the current root to perform the tree rotation on.

    Outputs:
    Binary_Tree_Node_t*     -   Pointer to the (possibly new) subtree root Node.
*/
Binary_Tree_Node_t* Binary_Tree_rotateRight(Binary_Tree_Node_t* TreeRoot);

/*
    Binary_Tree_rotateDoubleLeft

    This function performs a Double-Left rotation, based at the given Node.

    Inputs:
    TreeRoot    -   Pointer to the current root to perform the tree rotation on.

    Outputs:
    Binary_Tree_Node_t*     -   Pointer to the (possibly new) subtree root Node.
*/
Binary_Tree_Node_t* Binary_Tree_rotateDoubleLeft(Binary_Tree_Node_t* TreeRoot);

/*
    Binary_Tree_rotateDoubleRight

    This function performs a Double-Right rotation, based at the given Node.

    Inputs:
    TreeRoot    -   Pointer to the current root to perform the tree rotation on.

    Outputs:
    Binary_Tree_Node_t*     -   Pointer to the (possibly new) subtree root Node.
*/
Binary_Tree_Node_t* Binary_Tree_rotateDoubleRight(Binary_Tree_Node_t* TreeRoot);

#if defined(TESTING) || defined(DEBUGGER)

#include "tree_rotations_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
