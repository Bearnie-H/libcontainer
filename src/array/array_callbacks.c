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
#include "include/array.h"

int Array_DoCallback(Array_t *Array, CallbackFunc_t *Callback) {

    size_t Index = 0;
    void *ElementValue = NULL;
    int RetValue = 0;

    if (NULL == Array) {
        DEBUG_PRINTF("%s", "Error: NULL Array* provided.");
        return 1;
    }

    if (NULL == Callback) {
        DEBUG_PRINTF("%s", "Error: NULL Callback* provided, nothing to call.");
        return 0;
    }

    for (Index = 0; Index < Array_Length(Array); Index++) {
        ElementValue = Array_GetElement(Array, Index);
        if (NULL == ElementValue) {
            DEBUG_PRINTF("Error: Failed to get Array element at index [ %d ].", (int)Index);
            RetValue = 1;
            continue;
        }
        if (0 != Callback(ElementValue)) {
            DEBUG_PRINTF("Note: Callback function failed for Index [ %d ].", (int)Index);
            RetValue = 1;
        }
    }

    return RetValue;
}

int Array_DoCallbackArg(Array_t *Array, CallbackArgFunc_t *Callback, void *Args) {

    size_t Index = 0;
    void *ElementValue = NULL;
    int RetValue = 0;

    if (NULL == Array) {
        DEBUG_PRINTF("%s", "Error: NULL Array* provided.");
        return 1;
    }

    if (NULL == Callback) {
        DEBUG_PRINTF("%s", "Error: NULL Callback* provided, nothing to call.");
        return 0;
    }

    for (Index = 0; Index < Array_Length(Array); Index++) {
        ElementValue = Array_GetElement(Array, Index);
        if (NULL == ElementValue) {
            DEBUG_PRINTF("Error: Failed to get Array element at index [ %d ].", (int)Index);
            RetValue = 1;
            continue;
        }
        if (0 != Callback(ElementValue, Args)) {
            DEBUG_PRINTF("Note: Callback function failed for Index [ %d ].", (int)Index);
            RetValue = 1;
        }
    }

    return RetValue;
}
