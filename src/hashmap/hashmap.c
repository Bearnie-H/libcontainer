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
#include <string.h>

#include "include/hashmap.h"

#include "../array/include/array.h"
#include "../list/include/list.h"
#include "../logging/logging.h"

Hashmap_t *Hashmap_Create(HashFunc_t *HashFunc, size_t KeySize, ReleaseFunc_t *KeyReleaseFunc) {

    Hashmap_t *Map         = NULL;
    List_t *   NewList     = NULL;
    size_t     BucketIndex = 0;

    if ( NULL == HashFunc ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: NULL HashFunc provided, defaulting to HashFunc_String.");
#endif
        HashFunc = HashFunc_String;
        KeySize  = 0;
    } else if ( HashFunc_Int == HashFunc ) {
        KeySize = sizeof(int);
    } else if ( HashFunc_Long == HashFunc ) {
        KeySize = sizeof(long);
    } else if ( HashFunc_Double == HashFunc ) {
        KeySize = sizeof(double);
    }

    if ( NULL == KeyReleaseFunc ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: NULL KeyReleaseFunc* provided, defaulting to free().");
#endif
        KeyReleaseFunc = (ReleaseFunc_t *)free;
    }

    Map = (Hashmap_t *)calloc(1, sizeof(Hashmap_t));
    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Hashmap.");
#endif
        return NULL;
    }

    Map->Buckets =
        Array_RefCreate(LIBCONTAINER_HASHMAP_DEFAULT_CAPACITY, (ReleaseFunc_t *)List_Release);
    if ( NULL == Map->Buckets ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to create Array_t of Buckets.");
#endif
        Hashmap_Release(Map);
        return NULL;
    }

    for ( BucketIndex = 0; BucketIndex < LIBCONTAINER_HASHMAP_DEFAULT_CAPACITY; BucketIndex++ ) {
        NewList = List_Create();
        if ( 0 != Array_Append(Map->Buckets, &NewList) ) {
#ifdef DEBUG
            DEBUG_PRINTF("Error: Failed to initialize Map bucket [ %lu ].",
                         (unsigned long)BucketIndex);
#endif
            Hashmap_Release(Map);
            return NULL;
        }
    }

    Map->HashFunc       = HashFunc;
    Map->KeyReleaseFunc = KeyReleaseFunc;
    Map->ItemCount      = 0;
    Map->KeySize        = KeySize;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully created and initialized Hashmap_t.");
#endif
    return Map;
}

int Hashmap_Insert(Hashmap_t *Map, void *Key, void *Value, size_t KeySize, size_t ValueSize,
                   ReleaseFunc_t *ValueReleaseFunc) {

    Hashmap_Entry_t *Entry     = NULL;
    unsigned int     HashValue = 0;

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
#endif
        return 1;
    }

    Iterator_Invalidate(&(Map->Iterator));

    if ( (NULL == Key) || (NULL == Value) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Key* or Value* provided.");
#endif
        return 1;
    }

    if ( NULL == ValueReleaseFunc ) {
        if ( 0 == ValueSize ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: NULL ValueReleaseFunc for Reference-type value.");
#endif
            return 1;
        } else {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Note: NULL ValueReleaseFunc provided, defaulting to free().");
#endif
            ValueReleaseFunc = free;
        }
    }

    if ( 0 != Map->KeySize ) {
        if ( (0 != KeySize) && (KeySize != Map->KeySize) ) {
#ifdef DEBUG
            DEBUG_PRINTF("Error: Unexpected KeySize. Expected (%lu), got (%lu).",
                         (unsigned long)Map->KeySize, (unsigned long)KeySize);
#endif
            return 1;
        }
        KeySize = Map->KeySize;
    }

    /* First, check if this key is already present in the Hashmap. */
    Hashmap_Remove(Map, Key, KeySize);

    Entry = Hashmap_Entry_Create(Key, Value, KeySize, ValueSize, HashValue, Map->KeyReleaseFunc,
                                 ValueReleaseFunc);
    if ( NULL == Entry ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to create new Hashmap_Entry_t to insert into Hashmap.");
#endif
        return 1;
    }

    return Hashmap_insertEntry(Map, Entry, true);
}

void *Hashmap_Retrieve(Hashmap_t *Map, const void *Key, size_t KeySize) {

    unsigned int HashValue = 0;
    List_t *     Bucket    = NULL;

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
#endif
        return NULL;
    }

    if ( (0 != Map->KeySize) && (0 == KeySize) ) {
        KeySize = Map->KeySize;
    }

    if ( NULL == Key ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Key* provided.");
#endif
        return NULL;
    }

    Bucket = Hashmap_getBucket(Map, Key, KeySize, &HashValue);
    if ( NULL == Bucket ) {
        return NULL;
    }

    return Hashmap_findInBucket(Bucket, Key, KeySize, HashValue);
}

size_t Hashmap_Length(Hashmap_t *Map) {

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Warning: NULL Map* provided, no length to report.");
#endif
        return 0;
    }

    return Map->ItemCount;
}

bool Hashmap_KeyExists(Hashmap_t *Map, const void *Key, size_t KeySize) {
    return (NULL != Hashmap_Retrieve(Map, Key, KeySize));
}

int Hashmap_Remove(Hashmap_t *Map, const void *Key, size_t KeySize) {

    unsigned int     HashValue = 0;
    List_t *         Bucket    = NULL;
    List_Node_t *    Node      = NULL;
    Hashmap_Entry_t *Entry     = NULL;

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Warning: NULL Map* provided, not attempting to remove item.");
#endif
        return 0;
    }

    Iterator_Invalidate(&(Map->Iterator));

    if ( 0 == Hashmap_Length(Map) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: Hashmap has no elements, nothing to remove.");
#endif
        return 0;
    }

    if ( (0 != Map->KeySize) && (0 == KeySize) ) {
        KeySize = Map->KeySize;
    }

    Bucket = Hashmap_getBucket(Map, Key, KeySize, &HashValue);
    if ( NULL == Bucket ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to get Bucket for provided Key*.");
#endif
        return 1;
    }

    Node = Bucket->Head;
    while ( Node != NULL ) {
        Entry = (Hashmap_Entry_t *)Node->Contents.ContentRaw;
        if ( Entry->HashValue == HashValue ) {
            if ( 0 == memcmp(Key, Entry->Key, KeySize) ) {
                List_removeNode(Bucket, Node);
                Map->ItemCount -= 1;
#ifdef DEBUG
                DEBUG_PRINTF("%s", "Successfully removed item from Hashmap.");
#endif
                return 0;
            }
        }
        Node = Node->Next;
    }

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Error: Failed to find requested item in Hashmap.");
#endif
    return 1;
}

void *Hashmap_Pop(Hashmap_t *Map, const void *Key, size_t KeySize) {

    List_t *         Bucket    = NULL;
    List_Node_t *    Node      = NULL;
    Hashmap_Entry_t *Entry     = NULL;
    unsigned int     HashValue = 0;
    void *           Value     = NULL;

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
#endif
        return NULL;
    }

    Iterator_Invalidate(&(Map->Iterator));

    if ( NULL == Key ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Key* provided.");
#endif
        return NULL;
    }

    if ( (0 != Map->KeySize) && (0 == KeySize) ) {
        KeySize = Map->KeySize;
    }

    Bucket = Hashmap_getBucket(Map, Key, KeySize, &HashValue);
    if ( NULL == Bucket ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to find Bucket containing requested value.");
#endif
        return NULL;
    }

    Node = Bucket->Head;
    while ( NULL != Node ) {
        Entry = (Hashmap_Entry_t *)Node->Contents.ContentRaw;
        if ( Entry->HashValue == HashValue ) {
            if ( 0 == memcmp(Key, Entry->Key, Entry->KeySize) ) {
                Entry->ValueReleaseFunc = NULL;
                Value                   = Entry->Value.ValueRaw;
                List_removeNode(Bucket, Node);
                Map->ItemCount -= 1;
                return Value;
            }
        }
    }

    return NULL;
}

int Hashmap_Clear(Hashmap_t *Map) {

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "NULL Map* provided, nothing to clear.");
#endif
        return 0;
    }

    Iterator_Invalidate(&(Map->Iterator));

    if ( 0 != Array_DoCallback(Map->Buckets, (CallbackFunc_t *)List_Clear) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to clear all buckets within Hashmap.");
#endif
        return 1;
    }

    Map->ItemCount = 0;
#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully cleared Hashmap_t of all contents.");
#endif
    return 0;
}

void Hashmap_Release(Hashmap_t *Map) {

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Warning, NULL Map* provided, nothing to release.");
#endif
        return;
    }

    if ( NULL != Map->Buckets ) {
        Array_Release(Map->Buckets);
    }

    Iterator_Invalidate(&(Map->Iterator));

    ZERO_CONTAINER(Map, Hashmap_t);
    free(Map);

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully released all resources for Hashmap_t.");
#endif
    return;
}

/* ++++++++++ Private Functions ++++++++++ */

List_t *Hashmap_getBucket(Hashmap_t *Map, const void *Key, size_t KeySize,
                          unsigned int *HashValue) {

    size_t BucketIndex = 0;

    if ( 0 == *HashValue ) {
        *HashValue = Map->HashFunc(Key, KeySize);
    } else {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: Using cached HashValue instead of recomputing.");
#endif
    }

    BucketIndex = (size_t)(*HashValue % Array_Length(Map->Buckets));

    return (List_t *)Array_GetElement(Map->Buckets, BucketIndex);
}

void *Hashmap_findInBucket(List_t *Bucket, const void *Key, size_t KeySize,
                           unsigned int HashValue) {

    Hashmap_Entry_t *Entry = NULL;

    while ( NULL != (Entry = (Hashmap_Entry_t *)List_Next(Bucket)) ) {
        if ( Entry->HashValue == HashValue ) {
            if ( 0 == memcmp(Key, Entry->Key, KeySize) ) {
                Iterator_Invalidate(&(Bucket->Iterator));
#ifdef DEBUG
                DEBUG_PRINTF("%s", "Successfully found and returned requested item from Hashmap.");
#endif
                return Entry->Value.ValueRaw;
            }
        }
    }

    Iterator_Invalidate(&(Bucket->Iterator));
#ifdef DEBUG
    DEBUG_PRINTF("%s", "Error: Failed to find requested item in Hashmap.");
#endif
    return NULL;
}

int Hashmap_insertEntry(Hashmap_t *Map, Hashmap_Entry_t *Entry, bool AttemptRehash) {

    List_t *Bucket = NULL;

    if ( (NULL == Map) || (NULL == Entry) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Map* or Entry* provided.");
#endif
        return 1;
    }

    Bucket = Hashmap_getBucket(Map, (const void *)Entry->Key, Entry->KeySize, &Entry->HashValue);
    if ( NULL == Bucket ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to get Bucket for given Key*.");
#endif
        Hashmap_Entry_Release(Entry);
        return 1;
    }

    if ( 0 != List_RefPrepend(Bucket, Entry, (ReleaseFunc_t *)Hashmap_Entry_Release) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to add new Hashmap_Entry_t to Hashmap.");
#endif
        Hashmap_Entry_Release(Entry);
        return 1;
    }
    Map->ItemCount += 1;

    if ( AttemptRehash ) {
        Hashmap_rehash(Map);
    }

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully inserted new value into Hashmap_t.");
#endif
    return 0;
}

void Hashmap_rehash(Hashmap_t *Map) {

    double           LoadFactor = 0;
    List_t *         Bucket     = NULL;
    Hashmap_Entry_t *Entry      = NULL;
    size_t BucketIndex = 0, BucketLength = 0, OriginalBucketCount = 0, NewBucketCount = 0, i = 0;

    if ( NULL == Map ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Map* provided.");
#endif
        return;
    }

    OriginalBucketCount = Array_Length(Map->Buckets);

    LoadFactor = ((double)(Map->ItemCount) / (double)(OriginalBucketCount));
    if ( LoadFactor <= LIBCONTAINER_HASHMAP_LOAD_FACTOR ) {
        return;
    }

    if ( ARRAY_DOUBLING_THRESHOLD >= OriginalBucketCount ) {
        NewBucketCount = OriginalBucketCount;
    } else {
        NewBucketCount = ARRAY_DOUBLING_THRESHOLD;
    }

    if ( 0 != Array_Grow(Map->Buckets, NewBucketCount - 1) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Warning: Failed to grow internal Buckets array, aborting rehash.");
#endif
        return;
    }

    for ( i = 0; i < NewBucketCount; i++ ) {
        Bucket = List_Create();
        if ( 0 != Array_Append(Map->Buckets, &Bucket) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to add new Bucket to expand Hashmap_t.");
#endif
            return;
        }
    }

    for ( BucketIndex = 0; BucketIndex < OriginalBucketCount; BucketIndex++ ) {
        Bucket       = (List_t *)Array_GetElement(Map->Buckets, BucketIndex);
        BucketLength = List_Length(Bucket);

        for ( i = 0; i < BucketLength; i++ ) {
            Entry = (Hashmap_Entry_t *)List_PopBack(Bucket);
            if ( 0 != Hashmap_insertEntry(Map, Entry, false) ) {
#ifdef DEBUG
                DEBUG_PRINTF("%s",
                             "Error: Failed to rehash item for expanded Hashmap_t, dropping item.");
#endif
                Hashmap_Entry_Release(Entry);
            }
            Map->ItemCount -= 1;
        }
    }

    return;
}

/* ---------- Private Functions ---------- */
