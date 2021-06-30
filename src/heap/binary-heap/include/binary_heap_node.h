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

#ifndef LIBCONTAINER_BINARY_HEAP_NODE_H
#define LIBCONTAINER_BINARY_HEAP_NODE_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#include "../../../../include/libcontainer.h"

/*
    Binary_Heap_Node_t

    This struct represents a single Item within a Binary_Heap_t.
    This contains all of the necessary values to store the Key-Value pair,
    track their memory usage, and release the resources when no longer needed.
*/
typedef struct Binary_Heap_Node_t Binary_Heap_Node_t;

struct Binary_Heap_Node_t {

    /*
        Key is a pointer to the Key part of the Key-Value pair held by this Node.
    */
    void* Key;
    /*
        Value is a pointer to the Value part of the Key-Value pair held by this Node.
    */
    void* Value;

    /*
        KeyReleaseFunc is a pointer to the function to call to release the resources held
        by the Key pointer.
    */
    ReleaseFunc_t* KeyReleaseFunc;
    /*
        ValueReleaseFunc is a pointer to the function to call to release the resources held
        by the Value pointer.
    */
    ReleaseFunc_t* ValueReleaseFunc;

    /*
        KeySize holds the number of bytes allocated for the Key pointer.
    */
    size_t KeySize;
    /*
        ValueSize holds the number of bytes allocated for the Value pointer.
    */
    size_t ValueSize;

};

/*
    Binary_Heap_Node_Create

    This function creates and initializes a new Binary_Heap_Node_t, ready
    for use within a Binary_Heap_t.

    Inputs:
    Key     -   Pointer to the Key field of the Node.
    Value   -   Pointer to the Value field of the Node.
    KeySize     -   Size to allocate for the Key field (in bytes).
    ValueSize   -   Size to allocate for the Value field (in bytes).
    KeyReleaseFunc      -   Pointer to the function to call to release the resources
                                held by the Key field of the Node.
    ValueReleaseFunc    -   Pointer to the function to call to release the resources
                                held by the Value field of the Node.

    Outputs:
    Binary_Heap_Node_t* -   Pointer to a created and initialized Binary_Heap_Node_t
                                on success, or NULL on failure.
*/
Binary_Heap_Node_t *Binary_Heap_Node_Create(void* Key, void* Value,
                                            size_t KeySize, size_t ValueSize,
                                            ReleaseFunc_t* KeyReleaseFunc,
                                            ReleaseFunc_t* ValueReleaseFunc);

/*
    Binary_Heap_Node_UpdateKey

    This function updates the Key field of a given Node, setting it to some
    new value.

    Inputs:
    Node        -   Pointer to the Binary_Heap_Node_t to operate on.
    NewKey      -   Pointer to the new Key to update the Node to.
    NewKeySize  -   Size of the new Key to update the Node to.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Binary_Heap_Node_UpdateKey(Binary_Heap_Node_t* Node, void* NewKey, size_t NewKeySize);

/*
    Binary_Heap_Node_UpdateValue

    This function updates the Value field of a given Node, setting it to some
    new value.

    Inputs:
    Node    -       Pointer to the Binary_Heap_Node_t to operate on.
    NewValue        -   Pointer to the new Value to update the Node to.
    NewValueSize    -   Size of the new Value to update the Node to.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Binary_Heap_Node_UpdateValue(Binary_Heap_Node_t* Node, void* NewValue, size_t NewValueSize);

/*
    Binary_Heap_Node_Release

    This function fully releases a given Binary_Heap_Node_t* and all held resources.

    Inputs:
    Node    -   Pointer to the Binary_Heap_Node_t to operate on.

    Outputs:
    None, the resources are released.

    Note:
    The Node* passed to this function is no longer valid for use after
    this function returns.
*/
void Binary_Heap_Node_Release(Binary_Heap_Node_t* Node);

#if defined(TESTING) || defined(DEBUGGER)

#include "binary_heap_node_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
