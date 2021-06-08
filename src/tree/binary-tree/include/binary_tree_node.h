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

    Binary_Tree_Node_t* Parent;
    Binary_Tree_Node_t* LeftChild;
    Binary_Tree_Node_t* RightChild;

    ReleaseFunc_t* ReleaseFunc;
    uint8_t* Value;
    size_t ValueSize;

    /*
        Is restricting this to an integer a reasonable compromise?
        For generic keys and generic values, the Hashmap container
        is a better choice?
    */
    int Key;
};

Binary_Tree_Node_t* Binary_Tree_Node_Create(int Key, size_t ValueSize, const void* Value, ReleaseFunc_t* ReleaseFunc);
size_t Binary_Tree_Node_Height(Binary_Tree_Node_t* Root);
int Binary_Tree_Node_Update(Binary_Tree_Node_t* Node, const void* NewValue, size_t ValueSize);
void Binary_Tree_Node_Release(Binary_Tree_Node_t* Node);

#if defined(TESTING) || defined(DEBUGGER)

#include "binary_tree_node_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
