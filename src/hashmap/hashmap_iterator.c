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

#include <stdbool.h>
#include <stdlib.h>

#include "../logging/logging.h"
#include "include/hashmap.h"

#include "../array/include/array.h"
#include "../list/include/list.h"

int Iterator_Initialize_Hashmap(Hashmap_t *Map, CompareFunc_t *CompareFunc) {

    Iterator_t *Iterator = NULL;

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
#endif
        return 1;
    }

    Iterator_Invalidate(&(Map->Iterator));

    Iterator = (Iterator_t *)calloc(1, sizeof(Iterator_t));
    if ( NULL == Iterator ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory to initialize Iterator.");
#endif
        return 1;
    }

    if ( NULL != CompareFunc ) {
        Iterator->Context = Hashmap_KeysSorted(Map, CompareFunc);
    } else {
        Iterator->Context = Hashmap_Keys(Map);
    }
    if ( NULL == Iterator->Context ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to create Iteration context.");
#endif
        free(Iterator);
        return 1;
    }

    Iterator->ReleaseContext = (ReleaseFunc_t *)Array_Release;
    Iterator->Type           = Iterator_Hashmap;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully prepared Hashmap Iterator.");
#endif
    Map->Iterator = Iterator;
    return 0;
}

Hashmap_KeyValuePair_t Hashmap_Next(Hashmap_t *Map) {

    Hashmap_KeyValuePair_t KeyValue = {NULL, NULL};
    void *                 Temp[2]  = {NULL, NULL};

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
#endif
        return KeyValue;
    }

    if ( (NULL == Map->Iterator) || (Map->Iterator->Type == Iterator_UNKNOWN) ) {
        if ( 0 != Iterator_Initialize_Hashmap(Map, NULL) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to initialize Iterator to begin Map iteration.");
#endif
            return KeyValue;
        }
    }

    Temp[0] = Array_Next((Array_t *)(Map->Iterator->Context));
    if ( NULL == Temp[0] ) {
        if ( NULL == ((Array_t *)(Map->Iterator->Context))->Iterator ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Note: Successfully iterated over Hashmap.");
#endif
            return KeyValue;
        }
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to retrieve next Key.");
#endif
        return KeyValue;
    }

    Temp[1] = Hashmap_Retrieve(Map, Temp[0], 0);
    if ( NULL == Temp[1] ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to retrieve Value for next Key.");
#endif
        return KeyValue;
    }

    KeyValue.Key   = Temp[0];
    KeyValue.Value = Temp[1];

    return KeyValue;
}

Hashmap_KeyValuePair_t Hashmap_SortedNext(Hashmap_t *Map, CompareFunc_t *CompareFunc) {

    Hashmap_KeyValuePair_t KeyValue = {NULL, NULL};
    void *                 Temp[2]  = {NULL, NULL};

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
#endif
        return KeyValue;
    }

    if ( (NULL == Map->Iterator) || (Map->Iterator->Type == Iterator_UNKNOWN) ) {
        if ( 0 != Iterator_Initialize_Hashmap(Map, CompareFunc) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to initialize Iterator to begin Map iteration.");
#endif
            return KeyValue;
        }
    }

    Temp[0] = Array_Next((Array_t *)(Map->Iterator->Context));
    if ( NULL == Temp[0] ) {
        if ( NULL == ((Array_t *)(Map->Iterator->Context))->Iterator ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Note: Successfully iterated over Hashmap.");
#endif
            return KeyValue;
        }
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to retrieve next Key.");
#endif
        return KeyValue;
    }

    Temp[1] = Hashmap_Retrieve(Map, Temp[0], 0);
    if ( NULL == Temp[1] ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to retrieve Value for next Key.");
#endif
        return KeyValue;
    }

    KeyValue.Key   = Temp[0];
    KeyValue.Value = Temp[1];

    return KeyValue;
}

Hashmap_KeyValuePair_t Hashmap_Previous(Hashmap_t *Map) {

    Hashmap_KeyValuePair_t KeyValue = {NULL, NULL};
    void *                 Temp[2]  = {NULL, NULL};

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
#endif
        return KeyValue;
    }

    if ( (NULL == Map->Iterator) || (Map->Iterator->Type == Iterator_UNKNOWN) ) {
        if ( 0 != Iterator_Initialize_Hashmap(Map, NULL) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to initialize Iterator to begin Map iteration.");
#endif
            return KeyValue;
        }
    }

    Temp[0] = Array_Previous((Array_t *)(Map->Iterator->Context));
    if ( NULL == Temp[0] ) {
        if ( NULL == ((Array_t *)(Map->Iterator->Context))->Iterator ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Note: Successfully iterated over Hashmap.");
#endif
            return KeyValue;
        }
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to retrieve next Key.");
#endif
        return KeyValue;
    }

    Temp[1] = Hashmap_Retrieve(Map, Temp[0], 0);
    if ( NULL == Temp[1] ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to retrieve Value for next Key.");
#endif
        return KeyValue;
    }

    KeyValue.Key   = Temp[0];
    KeyValue.Value = Temp[1];

    return KeyValue;
}

Hashmap_KeyValuePair_t Hashmap_SortedPrevious(Hashmap_t *Map, CompareFunc_t *CompareFunc) {

    Hashmap_KeyValuePair_t KeyValue = {NULL, NULL};
    void *                 Temp[2]  = {NULL, NULL};

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
#endif
        return KeyValue;
    }

    if ( (NULL == Map->Iterator) || (Map->Iterator->Type == Iterator_UNKNOWN) ) {
        if ( 0 != Iterator_Initialize_Hashmap(Map, CompareFunc) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to initialize Iterator to begin Map iteration.");
#endif
            return KeyValue;
        }
    }

    Temp[0] = Array_Previous((Array_t *)(Map->Iterator->Context));
    if ( NULL == Temp[0] ) {
        if ( NULL == ((Array_t *)(Map->Iterator->Context))->Iterator ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Note: Successfully iterated over Hashmap.");
#endif
            return KeyValue;
        }
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to retrieve next Key.");
#endif
        return KeyValue;
    }

    Temp[1] = Hashmap_Retrieve(Map, Temp[0], 0);
    if ( NULL == Temp[1] ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to retrieve Value for next Key.");
#endif
        return KeyValue;
    }

    KeyValue.Key   = Temp[0];
    KeyValue.Value = Temp[1];

    return KeyValue;
}

Array_t *Hashmap_Keys(Hashmap_t *Map) {

    Array_t *        Keys         = NULL;
    List_t *         Bucket       = NULL;
    List_Node_t *    BucketItem   = NULL;
    size_t           BucketIndex  = 0;
    Hashmap_Entry_t *CurrentEntry = NULL;

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
#endif
        return NULL;
    }

    Keys = Array_RefCreate(0, NULL);
    if ( NULL == Keys ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to create Keys Array to return.");
#endif
        return NULL;
    }
    /* Explicitly set the ReleaseFunc to NULL, as this Array only holds pointers to objects owned by
     * the Hashmap. */
    Keys->ReleaseFunc = NULL;

    for ( BucketIndex = 0; BucketIndex < Array_Length(Map->Buckets); BucketIndex++ ) {
        Bucket = (List_t *)Array_GetElement(Map->Buckets, BucketIndex);
        if ( NULL == Bucket ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to get Bucket of items to build Keys Array.");
#endif
            Array_Release(Keys);
            return NULL;
        }

        for ( BucketItem = Bucket->Head; NULL != BucketItem; BucketItem = BucketItem->Next ) {
            CurrentEntry = (Hashmap_Entry_t *)BucketItem->Contents.ContentRaw;
            if ( 0 != Array_Append(Keys, &(CurrentEntry->Key)) ) {
#ifdef DEBUG
                DEBUG_PRINTF("%s", "Error: Failed to add Key value to Keys array to be returned.");
#endif
                Array_Release(Keys);
                return NULL;
            }
        }
    }

    return Keys;
}

Array_t *Hashmap_KeysSorted(Hashmap_t *Map, CompareFunc_t *CompareFunc) {

    Array_t *Keys = Hashmap_Keys(Map);

    if ( 0 != Array_Sort(Keys, CompareFunc) ) {
        Array_Release(Keys);
        return NULL;
    }

    return Keys;
}
