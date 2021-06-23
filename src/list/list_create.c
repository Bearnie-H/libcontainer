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

#include "../logging/logging.h"
#include "include/list.h"

List_t *List_Create(void) {

    List_t *List = NULL;

    /* Allocate the memory for the list. */
    List = (List_t *)calloc(1, sizeof(List_t));
    if ( NULL == List ) {
        DEBUG_PRINTF("%s", "Error, Failed to allocate memory for List_t.");
        return NULL;
    }

    /* Set the default starting values. */
    List->Head     = NULL;
    List->Tail     = NULL;
    List->Iterator = NULL;
    List->Length   = 0;

    return List;
}

int List_Clear(List_t *List) {

    if ( NULL == List ) {
        DEBUG_PRINTF("%s", "NULL List* provided, nothing to remove.");
        return 0;
    }

    while ( 0 != List_Length(List) ) {
        if ( 0 != List_Remove(List, 0) ) {
            DEBUG_PRINTF("%s", "Error: Failed to remove item during List_Clear().");
            return 1;
        }
    }

    DEBUG_PRINTF("%s", "Successfully removed all entries from List_t.");
    return 0;
}

void List_Release(List_t *List) {

    List_Node_t *Current = NULL;
    List_Node_t *Next    = NULL;

    if ( NULL == List ) {
        DEBUG_PRINTF("%s", "NULL List_t* provided, nothing to release.");
        return;
    }

    for ( Current = List->Head; Current != NULL; ) {
        Next = Current->Next;
        ListNode_Release(Current);
        Current = Next;
    }

    Iterator_Invalidate(&(List->Iterator));
    ZERO_CONTAINER(List, List_t);

    free(List);
    DEBUG_PRINTF("%s", "Successfully released List_t* and all held contents.");
    return;
}
