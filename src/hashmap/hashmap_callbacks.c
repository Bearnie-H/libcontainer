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
#include "include/hashmap.h"

int Hashmap_DoCallback(Hashmap_t *Map, CallbackFunc_t *Callback) {

    Hashmap_KeyValuePair_t KeyValue;
    int                    RetVal = 0;

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
#endif
        return -1;
    }

    Iterator_Invalidate(&(Map->Iterator));

    if ( NULL == Callback ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Callback* provided, nothing to call.");
#endif
        return 0;
    }

    HASHMAP_FOREACH(Map, KeyValue) {
        if ( 0 != Callback(&(KeyValue)) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Warning: Callback function returned non-zero.");
#endif
            RetVal += 1;
        }
    }

    return RetVal;
}

int Hashmap_DoCallbackArg(Hashmap_t *Map, CallbackArgFunc_t *Callback, void *Args) {

    Hashmap_KeyValuePair_t KeyValue;
    int                    RetVal = 0;

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
#endif
        return -1;
    }

    Iterator_Invalidate(&(Map->Iterator));

    if ( NULL == Callback ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Callback* provided, nothing to call.");
#endif
        return 0;
    }

    HASHMAP_FOREACH(Map, KeyValue) {
        if ( 0 != Callback(&(KeyValue), Args) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Warning: Callback function returned non-zero.");
#endif
            RetVal += 1;
        }
    }

    return RetVal;
}
