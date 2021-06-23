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

#ifndef LIBCONTAINER_BINARY_TREE_ITERATOR_H
#define LIBCONTAINER_BINARY_TREE_ITERATOR_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#define NODE_IS_LEFT_CHILD(Node) ((NULL != (Node)->Parent) && ((Node) == (Node)->Parent->LeftChild))
#define NODE_IS_RIGHT_CHILD(Node) ((NULL != (Node)->Parent) && ((Node) == (Node)->Parent->RightChild))

#define LIBCONTAINER_ENABLE_BINARY_TREE
#include "../../../../include/libcontainer.h"

#include "binary_tree_node.h"

/*
    Iterator_Initialize_Binary_Tree

    This function initializes the Iterator_t of a Binary_Tree_t, preparing it for the
    requested direction.

    Inputs:
    Tree        -   Pointer to the Binary Tree to iterate over.
    Direction   -   The tree traversal direction to iterate with.
    Forward     -   Boolean toggling forward or reverse order iteration.

    Outputs:
    int     -   returns 0 on success, non-zero on failure.

    Note:
    The Iterator_t context field is simply a pointer to the most recent Binary_Tree_Node_t,
    suitable for passing into any of the other *_Next() or *_Previous() functions defined in this header.
*/
int Iterator_Initialize_Binary_Tree(Binary_Tree_t *Tree, Binary_Tree_Direction_t Direction, bool Forward);

/*
    Binary_Tree_inOrderNext

    This function starts from the given node and returns the In-Order successor,
    if it exists.

    Inputs:
    Current     -   Pointer to the most recent Node visited. This is the *starting*
                        node from which we find the corresponding "next" node.

    Outputs:
    Binary_Tree_Node_t* -   Pointer to the corresponding "next" node, or NULL
                                when iteration is finished.
*/
Binary_Tree_Node_t* Binary_Tree_inOrderNext(Binary_Tree_Node_t* Current);

/*
    Binary_Tree_preOrderNext

    This function starts from the given node and returns the Pre-Order successor,
    if it exists.

    Inputs:
    Current     -   Pointer to the most recent Node visited. This is the *starting*
                        node from which we find the corresponding "next" node.

    Outputs:
    Binary_Tree_Node_t* -   Pointer to the corresponding "next" node, or NULL
                                when iteration is finished.
*/
Binary_Tree_Node_t* Binary_Tree_preOrderNext(Binary_Tree_Node_t* Current);

/*
    Binary_Tree_postOrderNext

    This function starts from the given node and returns the Post-Order successor,
    if it exists.

    Inputs:
    Current     -   Pointer to the most recent Node visited. This is the *starting*
                        node from which we find the corresponding "next" node.

    Outputs:
    Binary_Tree_Node_t* -   Pointer to the corresponding "next" node, or NULL
                                when iteration is finished.
*/
Binary_Tree_Node_t* Binary_Tree_postOrderNext(Binary_Tree_Node_t* Current);

/*
    Binary_Tree_inOrderPrevious

    This function starts from the given node and returns the In-Order predecessor,
    if it exists.

    Inputs:
    Current     -   Pointer to the most recent Node visited. This is the *starting*
                        node from which we find the corresponding "previous" node.

    Outputs:
    Binary_Tree_Node_t* -   Pointer to the corresponding "previous" node, or NULL
                                when iteration is finished.
*/
Binary_Tree_Node_t* Binary_Tree_inOrderPrevious(Binary_Tree_Node_t* Current);

/*
    Binary_Tree_preOrderPrevious

    This function starts from the given node and returns the Pre-Order predecessor,
    if it exists.

    Inputs:
    Current     -   Pointer to the most recent Node visited. This is the *starting*
                        node from which we find the corresponding "previous" node.

    Outputs:
    Binary_Tree_Node_t* -   Pointer to the corresponding "previous" node, or NULL
                                when iteration is finished.
*/
Binary_Tree_Node_t* Binary_Tree_preOrderPrevious(Binary_Tree_Node_t* Current);

/*
    Binary_Tree_postOrderPrevious

    This function starts from the given node and returns the Post-Order predecessor,
    if it exists.

    Inputs:
    Current     -   Pointer to the most recent Node visited. This is the *starting*
                        node from which we find the corresponding "previous" node.

    Outputs:
    Binary_Tree_Node_t* -   Pointer to the corresponding "previous" node, or NULL
                                when iteration is finished.
*/
Binary_Tree_Node_t* Binary_Tree_postOrderPrevious(Binary_Tree_Node_t* Current);

#if defined(TESTING) || defined(DEBUGGER)

#include "binary_tree_iterator_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
