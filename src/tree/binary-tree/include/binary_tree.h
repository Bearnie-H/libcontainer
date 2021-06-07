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

#define LIBCONTAINER_ENABLE_BINARY_TREE
#include "../../../../include/libcontainer.h"
#include "binary_tree_node.h"
#include "tree_rotations.h"

/*
    Binary_Tree_Direction_t

    This ...

    <UNUSED - Kept in case I can figure out resumable traversals>
*/
typedef enum Binary_Tree_Direction_t {
    Direction_PreOrder,
    Direction_InOrder,
    Direction_PostOrder
} Binary_Tree_Direction_t;

struct Binary_Tree_t {

    /*
        A Binary Search Tree simply contains a pointer to the current root node.
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
    ReleaseFunc_t* ReleaseFunc;

    /*
        The total number of elements in the tree.
    */
    size_t TreeSize;

    /*
        ValueSize caches the size of the values held by this tree. If this is set to 0,
        the tree will only hold references to the items, and not own the resources itself.
    */
    size_t ValueSize;
};

/* ++++++++++ Private Functions ++++++++++ */

/*
    TODO: Function Documentation
*/
Binary_Tree_Node_t* Binary_Tree_find(Binary_Tree_Node_t* Root, int Key);

Binary_Tree_Node_t* Binary_Tree_findMinimum(Binary_Tree_Node_t* Root);

/*
    TODO: Function Documentation
*/
Binary_Tree_Node_t* Binary_Tree_insertNode(Binary_Tree_Node_t* Root, Binary_Tree_Node_t* Node);

/*
    TODO: Function Documentation
*/
int Binary_Tree_removeNode(Binary_Tree_t* Tree, Binary_Tree_Node_t* Node);

int Binary_Tree_doCallback(Binary_Tree_Node_t* Root, CallbackFunc_t* Callback);
int Binary_Tree_doCallbackArg(Binary_Tree_Node_t* Root, CallbackArgFunc_t* Callback, void* Args);


/* ---------- Private Functions ---------- */

#if defined(TESTING) || defined(DEBUGGER)

#include "binary_tree_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
