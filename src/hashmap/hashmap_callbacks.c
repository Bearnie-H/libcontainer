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

    size_t BucketIndex = 0, EntryIndex = 0;
    List_t *Bucket = NULL;
    Hashmap_Entry_t *Entry = NULL;
    void *KeyValuePair[2] = {NULL, NULL};
    int RetVal = 0;

    if (NULL == Map) {
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
        return 1;
    }

    if (NULL == Callback) {
        DEBUG_PRINTF("%s", "Error: NULL Callback* provided, nothing to call.");
        return 0;
    }

    for (BucketIndex = 0; BucketIndex < Array_Length(Map->Buckets); BucketIndex++) {
        Bucket = (List_t *)Array_GetElement(Map->Buckets, BucketIndex);
        if (NULL == Bucket) {
            DEBUG_PRINTF("%s", "Error: Failed to get Bucket from Hashmap.");
            RetVal = 1;
            continue;
        }
        for (EntryIndex = 0; EntryIndex < List_Length(Bucket); EntryIndex++) {
            Entry = (Hashmap_Entry_t *)List_GetElement(Bucket, EntryIndex);
            if (NULL == Entry) {
                DEBUG_PRINTF("%s", "Error: Retrieved NULL Entry from Hashmap Bucket.");
                RetVal = 1;
                continue;
            }
            KeyValuePair[0] = Entry->Key;
            KeyValuePair[1] = Entry->Value.ValueRaw;
            if (0 != Callback(KeyValuePair)) {
                DEBUG_PRINTF("%s", "Error: Callback function returned non-zero.");
                RetVal = 1;
            }
        }
    }

    return RetVal;
}

int Hashmap_DoCallbackArg(Hashmap_t *Map, CallbackArgFunc_t *Callback, void *Args) {

    size_t BucketIndex = 0, EntryIndex = 0;
    List_t *Bucket = NULL;
    Hashmap_Entry_t *Entry = NULL;
    void *KeyValuePair[2] = {NULL, NULL};
    int RetVal = 0;

    if (NULL == Map) {
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
        return 1;
    }

    if (NULL == Callback) {
        DEBUG_PRINTF("%s", "Error: NULL Callback* provided, nothing to call.");
        return 0;
    }

    for (BucketIndex = 0; BucketIndex < Array_Length(Map->Buckets); BucketIndex++) {
        Bucket = (List_t *)Array_GetElement(Map->Buckets, BucketIndex);
        if (NULL == Bucket) {
            DEBUG_PRINTF("%s", "Error: Failed to get Bucket from Hashmap.");
            RetVal = 1;
            continue;
        }
        for (EntryIndex = 0; EntryIndex < List_Length(Bucket); EntryIndex++) {
            Entry = (Hashmap_Entry_t *)List_GetElement(Bucket, EntryIndex);
            if (NULL == Entry) {
                DEBUG_PRINTF("%s", "Error: Retrieved NULL Entry from Hashmap Bucket.");
                RetVal = 1;
                continue;
            }
            KeyValuePair[0] = Entry->Key;
            KeyValuePair[1] = Entry->Value.ValueRaw;
            if (0 != Callback(KeyValuePair, Args)) {
                DEBUG_PRINTF("%s", "Error: Callback function returned non-zero.");
                RetVal = 1;
            }
        }
    }

    return RetVal;
}
