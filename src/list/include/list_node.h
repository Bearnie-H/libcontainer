/*
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

#ifndef LIBCONTAINER_LIST_NODE_H
#define LIBCONTAINER_LIST_NODE_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#define LIBCONTAINER_ENABLE_LIST

#include "../../../include/libcontainer.h"

/*
    List_Node_t

    This represents one element within a larger Linked-List or derived structure.
    This implements the necessary functionality to build Doubly-Linked lists,
    as well as circularly linked lists. Furthermore, this allows for heterogeneous
    lists by ensuring each element knows how to release it's own resources,
    rather than performing it all from the encompassing List_t.
*/
typedef struct List_Node_t List_Node_t;

struct List_Node_t {

    /*
        ReleaseFunc defines the function to call (in combination with IsReference)
        to release the memory held by the contents of this node. This is held within
        the node itself, to allow for heterogeneous lists.
    */
    ReleaseFunc_t* ReleaseFunc;

    /*
        Pointers to the next and previous nodes of the list,
        to allow for doubly-linked traversal.
    */
    List_Node_t* Next;
    List_Node_t* Previous;

    /* Contents holds the actual data of this node. */
    union {
        uint8_t* ContentBytes;
        void* ContentRaw;
    } Contents;

    /* Size counts how many bytes are allocated to Contents. */
    size_t Size;
};

/*
    ListNode_Create

    This function will create a new List_Node_t which owns the memory associated with
    its contents, and whose contents are *Size* bytes large. This node is intended to
    be used to construct larger Linked-Lists, or other containers or data structures
    composed of such lists.

    Inputs:
    Contents    -   Pointer to the memory contents to transfer into this Node.
    Size        -   The size of the memory to transfer into this Node, in bytes.

    Outputs:
    List_Node_t*    -   Pointer to the fully constructed and initialized List_Node_t
                            on success, or NULL on failure.
*/
List_Node_t* ListNode_Create(const void* Contents, size_t Size);

/*
    ListNode_RefCreate

    This function will create a new List_Node_t which does *NOT* own the memory
    associated with its contents. This is valuable when the Node is simply to
    contain some existing data object which has a life-cycle more complex than
    a basic malloc()/free().

    Inputs:
    Contents        -   Pointer to the memory contents to transfer into this Node.
    ReleaseFunc     -   Pointer to the function to call to release the memory associated
                            with the Contents.

    Outputs:
    List_Node_t*    -   Pointer to the fully constructed and initialized List_Node_t
                            on success, or NULL on failure.

    Note:
    Passing NULL as the ReleaseFunc will default to using free() from the stdlib.h
*/
List_Node_t* ListNode_RefCreate(void* Contents, ReleaseFunc_t* ReleaseFunc);

/*
    ListNode_Release

    This function will fully release a List_Node_t as prepared by either ListNode_Create()
    or ListNode_RefCreate(). This will safely handle the proper release of the contents of
    the node depending on whether the node owns the memory or not. After being passed to this
    function, any List_Node_t is invalid for further use.

    This function also ensures the neighbouring List_Node_t values maintain correct ordering
    while this is being released.

    Inputs:
    Node    -   Pointer to the List_Node_t to release.

    Outputs:
    None, this call will fully release the Node and all held resources.
*/
void ListNode_Release(List_Node_t* Node);

/*
    ListNode_InsertAfter

    This function will place the requested List_Node_t immediately after the requested Node.
    If the Base node has an existing Next node, this will safely insert the requested Node
    between them. In addition, if Base is the end of a non-circular list, this will preserve
    the end-condition for the newly inserted Node.

    Inputs:
    Base        -   Pointer to the List_Node_t* used as a reference.
    ToInsert    -   Pointer to the List_Node_t* to be added immediately after Base.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int ListNode_InsertAfter(List_Node_t* Base, List_Node_t* ToInsert);

/*
    ListNode_InsertBefore

    This function will place the requested List_Node_t immediately before the requested Node.
    If the Base node has an existing Previous node, this will safely insert the requested Node
    between them. In addition, if Base is the beginning of a non-circular list, this will preserve
    the start-condition for the newly inserted Node.

    Inputs:
    Base        -   Pointer to the List_Node_t* used as a reference.
    ToInsert    -   Pointer to the List_Node_t* to be added immediately before Base.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int ListNode_InsertBefore(List_Node_t* Base, List_Node_t* ToInsert);

/*
    ListNode_Delete

    This function will remove a List_Node_t, but not release its contents. This is useful when
    an item is being transferred out of a List_t, and ownership is being transferred to something
    else.

    Inputs:
    Node    -   Pointer to the List_Node_t to delete.

    Outputs:
    None, the Node is removed and the neighbouring pointers are safely updated.
*/
void ListNode_Delete(List_Node_t* Node);

/*
    ListNode_UpdateValue

    This function will update the value held by the List_Node_t, releasing the existing
    contents and updating to the newly provided value. This is the common function regardless
    of whether the values are Reference-Type or not, and the semantics depends on the value of
    the ElementSize parameter.

    Inputs:
    Node            -   The List_Node_t to update the value of.
    Element         -   Pointer to the new value to update the Node to.
    ElementSize     -   The size (in bytes) of memory to allocate to hold the value.
    ReleaseFunc     -   The function to use to release the resources held by the object.
                            Only meaningful if ElementSize != 0.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.

    Note:
    If ElementSize is equal to 0, this indicates a Reference-Type value, and no allocations
    should be performed. This also means the given ReleaseFunc will be stored to release the
    contents as needed. Non-zero ElementSize indicates a non-reference value, and an allocation
    will take place, setting the ReleaseFunc to free() from stdlib.h
*/
int ListNode_UpdateValue(List_Node_t* Node, void* Element, size_t ElementSize, ReleaseFunc_t* ReleaseFunc);


#if defined(TESTING) || defined(DEBUGGER)

#include "list_node_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
