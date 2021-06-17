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

#include "../array/include/array.h"
#include "../list/include/list.h"

Array_t *Hashmap_Keys(Hashmap_t *Map) {

    Array_t *Keys = NULL;
    List_t *Bucket = NULL;
    List_Node_t *BucketItem = NULL;
    size_t BucketIndex = 0;
    Hashmap_Entry_t *CurrentEntry = NULL;

    if (NULL == Map) {
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
        return NULL;
    }

    Keys = Array_RefCreate(0, NULL);
    if (NULL == Keys) {
        DEBUG_PRINTF("%s", "Error: Failed to create Keys Array to return.");
        return NULL;
    }
    /* Explicitly set the ReleaseFunc to NULL, as this Array only holds pointers to objects owned by
     * the Hashmap. */
    Keys->ReleaseFunc = NULL;

    for (BucketIndex = 0; BucketIndex < Array_Length(Map->Buckets); BucketIndex++) {
        Bucket = (List_t *)Array_GetElement(Map->Buckets, BucketIndex);
        if (NULL == Bucket) {
            DEBUG_PRINTF("%s", "Error: Failed to get Bucket of items to build Keys Array.");
            Array_Release(Keys);
            return NULL;
        }

        for (BucketItem = Bucket->Head; NULL != BucketItem; BucketItem = BucketItem->Next) {
            CurrentEntry = (Hashmap_Entry_t *)BucketItem->Contents.ContentRaw;
            if (0 != Array_Append(Keys, &(CurrentEntry->Key))) {
                DEBUG_PRINTF("%s", "Error: Failed to add Key value to Keys array to be returned.");
                Array_Release(Keys);
                return NULL;
            }
        }
    }

    return Keys;
}

Array_t *Hashmap_KeysSorted(Hashmap_t *Map, CompareFunc_t *CompareFunc) {

    Array_t *Keys = Hashmap_Keys(Map);

    if (0 != Array_Sort(Keys, CompareFunc)) {
        Array_Release(Keys);
        return NULL;
    }

    return Keys;
}
