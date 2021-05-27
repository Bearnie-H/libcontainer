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

#include "../../../include/libcontainer.h"
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

    /*
        IsReference defines whether the nodes of the list own the resources they
        hold, or if they simple reference existing objects.
    */
    bool IsReference;

    /*
        IsHomogeneous defines whether the list contains nodes where the contents
        of each are of the same type (size, alignment, etc...), or if each node
        may contain different objects.
    */
    bool IsHomogeneous;

    /*
        ReleaseFunc can mean a number of different things based on the values of IsReference and
        IsHomogeneous. Generally, this refers to a function used to release the resources of
        the nodes within the list.

        IsHomogeneous && !IsReference   - Simplest case; Simply the free() function
            from stdlib.h, and used for the contents of each node within the list.

        IsHomogeneous && IsReference    - Slightly more complex; Refers to the function
            to call to release the resources of a given Node. Equal across all
            nodes and is therefore not required when adding a new node.

        !IsHomogeneous && !IsReference  - Simple case; Simply the free() function
            from stdlib.h, and used for the contents of each node within the list.
            The fact that the list is heterogeneous doesn't matter, as the list
            contains the resources for each element.

        !IsHomogeneous && IsReference   - Complex case; Each element must provide
            its own ReleaseFunc when it is added to the List in order to ensure each
            element is able to be safely released.
    */
    ReleaseFunc_t ReleaseFunc;
};

#if defined(TESTING) || defined(DEBUGGER)

#include "list_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
