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

#include "../logging/logging.h"
#include "include/list.h"

int List_DoCallback(List_t *List, CallbackFunc_t *Callback) {

    void * NodeValue = NULL;
    int    RetValue  = 0;
    size_t Count     = 0;

    if ( NULL == List ) {
        DEBUG_PRINTF("%s", "Error: NULL List* provided.");
        return 1;
    }

    Iterator_Invalidate(&(List->Iterator));

    if ( NULL == Callback ) {
        DEBUG_PRINTF("%s", "Error: NULL Callback* provided, nothing to call.");
        return 0;
    }

    while ( NULL != (NodeValue = List_Next(List)) ) {
        if ( 0 != Callback(NodeValue) ) {
            DEBUG_PRINTF("%s", "Error: Callback function returned non-zero.");
            RetValue = 1;
        }
        Count += 1;
    }

    if ( Count != List_Length(List) ) {
        DEBUG_PRINTF("%s", "Error: Iteration length does not equal List length.");
        return 1;
    }

    return RetValue;
}

int List_DoCallbackArg(List_t *List, CallbackArgFunc_t *Callback, void *Args) {

    void * NodeValue = NULL;
    int    RetValue  = 0;
    size_t Count     = 0;

    if ( NULL == List ) {
        DEBUG_PRINTF("%s", "Error: NULL List* provided.");
        return 1;
    }

    Iterator_Invalidate(&(List->Iterator));

    if ( NULL == Callback ) {
        DEBUG_PRINTF("%s", "Error: NULL Callback* provided, nothing to call.");
        return 0;
    }

    while ( NULL != (NodeValue = List_Next(List)) ) {
        if ( 0 != Callback(NodeValue, Args) ) {
            DEBUG_PRINTF("%s", "Error: Callback function returned non-zero.");
            RetValue = 1;
        }
        Count += 1;
    }

    if ( Count != List_Length(List) ) {
        DEBUG_PRINTF("%s", "Error: Iteration length does not equal List length.");
        return 1;
    }

    return RetValue;
}
