/*
    Copyright (c) 2021 Bearnie-H

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
*/

#include <stdlib.h>
#include <string.h>

#include "../../include/libcontainer.h"
#include "../error/include/error.h"
#include "include/list.h"
#include "include/list_node.h"

List_t *List_Create(void) {

    List_t *List = NULL;

    /* Allocate the memory for the list. */
    List = (List_t *)malloc(sizeof(List_t));
    if (NULL == List) {
        DEBUG_PRINTF("%s", "Error, Failed to allocate memory for List_t.");
        return NULL;
    }

    /* Set the default starting values. */
    List->Head = NULL;
    List->Tail = NULL;
    List->Length = 0;
    List->IsHomogeneous = true;
    List->IsReference = false;
    List->ReleaseFunc = free;

    return List;
}

List_t *List_CreateHet(void) {

    List_t *List = NULL;

    /* Create the default List_t. */
    List = List_Create();
    if (NULL == List) {
        DEBUG_PRINTF("%s", "Error, Failed to create List_t.");
        return NULL;
    }

    /* Tailor the settings for a heterogeneous, non-reference list. */
    List->IsHomogeneous = false;

    return List;
}

List_t *List_RefCreate(ReleaseFunc_t ReleaseFunc) {

    List_t *List = NULL;

    /* Create the default List_t. */
    List = List_Create();
    if (NULL == List) {
        DEBUG_PRINTF("%s", "Error, Failed to create List_t.");
        return NULL;
    }

    /* Assert a meaningful ReleaseFunc. */
    if (NULL == ReleaseFunc) {
        DEBUG_PRINTF("%s", "NULL ReleaseFunc provided, defaulting to free().");
        ReleaseFunc = free;
    }

    /* Tailor the properties for a homogeneous, ref-type List. */
    List->IsReference = true;
    List->ReleaseFunc = ReleaseFunc;

    return List;
}

List_t *List_RefCreateHet(void) {

    List_t *List = NULL;

    /* Create the default List_t. */
    List = List_Create();
    if (NULL == List) {
        DEBUG_PRINTF("%s", "Error, Failed to create List_t.");
        return NULL;
    }

    /* Tailor the settings for a heterogeneous, non-reference list. */
    List->IsHomogeneous = false;
    List->IsReference = true;
    List->ReleaseFunc = NULL; /* Each element must provide it's own ReleaseFunc
                                 when being added to the List. */

    return List;
}

void List_Release(List_t *List) {

    List_Node_t *Current = NULL;
    List_Node_t *Next = NULL;

    if (NULL == List) {
        DEBUG_PRINTF("%s", "NULL List_t* provided, nothing to release.");
        return;
    }

    for (Current = List->Head; Current != NULL;) {
        Next = Current->Next;
        ListNode_Release(Current);
        Current = Next;
    }

    List->Head = NULL;
    List->Tail = NULL;
    List->ReleaseFunc = NULL;
    List->Length = 0;

    free(List);
    DEBUG_PRINTF("%s", "Successfully release List_t* and all held contents.");
    return;
}
