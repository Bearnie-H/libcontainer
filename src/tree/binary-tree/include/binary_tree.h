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
        ValueSize caches the size of the values held by this tree. If this is set to 0,
        the tree will only hold references to the items, and not own the resources itself.
    */
    size_t ValueSize;

    /*
        The count of how many nodes exist within the tree. This allows O(1)
        checks for the length of the tree.
    */
    size_t Size;
};

#if defined(TESTING) || defined(DEBUGGER)

#include "binary_tree_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
