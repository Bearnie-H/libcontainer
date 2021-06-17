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

#include <stdlib.h>

#define LIBCONTAINER_ENABLE_LIST

#include "../../include/libcontainer.h"

#include "../iterator/include/iterator.h"
#include "../logging/logging.h"
#include "include/list.h"

int Iterator_Initialize_List(List_t *List) {

    Iterator_t *Iterator = NULL;

    if (NULL == List) {
        DEBUG_PRINTF("%s", "Error: NULL List* provided.");
        return 1;
    }

    Iterator_Invalidate(&(List->Iterator));

    Iterator = (Iterator_t *)calloc(1, sizeof(Iterator_t));
    if (NULL == Iterator) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory to initialize Iterator.");
        return 1;
    }

    Iterator->ReleaseContext = NULL;
    Iterator->Type = Iterator_List;
    Iterator->Context = NULL;

    DEBUG_PRINTF("%s", "Successfully prepared List Iterator.");
    List->Iterator = Iterator;
    return 0;
}

void *List_Next(List_t *List) {

    void *Item = NULL;

    if (NULL == List) {
        DEBUG_PRINTF("%s", "Error: NULL List* provided.");
        return NULL;
    }

    if ((NULL == List->Iterator) || (List->Iterator->Type == Iterator_UNKNOWN)) {
        if (0 != Iterator_Initialize_List(List)) {
            DEBUG_PRINTF("%s", "Error: Failed to initialize List iterator.");
            return NULL;
        }
        List->Iterator->Context = List->Head;
    }

    if (NULL == List->Iterator->Context) {
        DEBUG_PRINTF("%s", "Note: Iteration has reached the end of the List_t.");
        Iterator_Invalidate(&(List->Iterator));
        return NULL;
    }

    Item = ((List_Node_t *)(List->Iterator->Context))->Contents.ContentRaw;

    List->Iterator->Context = ((List_Node_t *)(List->Iterator->Context))->Next;

    return Item;
}

void *List_Previous(List_t *List) {

    void *Item = NULL;

    if (NULL == List) {
        DEBUG_PRINTF("%s", "Error: NULL List* provided.");
        return NULL;
    }

    if ((NULL == List->Iterator) || (List->Iterator->Type == Iterator_UNKNOWN)) {
        if (0 != Iterator_Initialize_List(List)) {
            DEBUG_PRINTF("%s", "Error: Failed to initialize List iterator.");
            return NULL;
        }
        List->Iterator->Context = List->Tail;
    }

    if (NULL == List->Iterator->Context) {
        DEBUG_PRINTF("%s", "Note: Iteration has reached the end of the List_t.");
        Iterator_Invalidate(&(List->Iterator));
        return NULL;
    }

    Item = ((List_Node_t *)(List->Iterator->Context))->Contents.ContentRaw;

    List->Iterator->Context = ((List_Node_t *)(List->Iterator->Context))->Previous;

    return Item;
}
