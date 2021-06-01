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

#ifndef LIBCONTAINER_LIST_H
#define LIBCONTAINER_LIST_H

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

#include "list_node.h"

struct List_t {

    /*
        Head points to the first node within the list.
        NULL if the list is empty.
        Equal to Tail if the list has one element.
    */
    List_Node_t* Head;

    /*
        Tail points to the final node within the list.
        NULL is the list is empty.
        Equal to Head if the list has one element.
    */
    List_Node_t* Tail;

    /* Length holds the count of nodes within the list. */
    size_t Length;
};

/* Private Function Declarations. */

/*
    List_findNode

    This function is the basic linear search through a List_t to find the item at the
    specified index. Slightly more efficient for the ends of the List, but a linear search
    for any middle index.

    Inputs:
    List    -   Pointer to the List_T to search through.
    Index   -   The 0-indexed location of the List_Node_t to return.

    Outputs:
    List_Node_t*    -   Pointer to the desired Node of the List_t.

    Note:
    As this function is private, and all public functions perform
    bounds-checking on the Index value, this is guaranteed to return
    a Node within the List as long as the List_t itself is structured
    correctly. As such, no input validation is needed.
*/
List_Node_t* List_findNode(List_t* List, int Index);

/*
    List_removeNode

    This function will remove the given List_Node_t from the specified List_t.
    This is the generic removal operation, and will safely release the Node resources,
    as well as handle updating the necessary List fields.

    Inputs:
    List    -   Pointer to the List_t to remove the node from.
    Node    -   Pointer to the Node to be removed from the List.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int List_removeNode(List_t* List, List_Node_t* Node);

#if defined(TESTING) || defined(DEBUGGER)

#include "list_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
