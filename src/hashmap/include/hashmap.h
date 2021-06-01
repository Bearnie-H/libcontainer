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

#ifndef LIBCONTAINER_HASHMAP_H
#define LIBCONTAINER_HASHMAP_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#define LIBCONTAINER_ENABLE_HASHMAP
#define LIBCONTAINER_ENABLE_ARRAY
#define LIBCONTAINER_ENABLE_LIST

#include "../../../include/libcontainer.h"
#include "hashmap_entry.h"

#ifndef LIBCONTAINER_HASHMAP_LOAD_FACTOR
/*
    LIBCONTAINER_HASHMAP_LOAD_FACTOR

    This macro defines the maximum Load Factor ( # Items / # Buckets )
    allowed in a Hashmap before a Rehash must be performed.

    This is tunable during build-time be re-defining this macro
    with the desired floating point value.
*/
#define LIBCONTAINER_HASHMAP_LOAD_FACTOR 4
#endif

#ifndef LIBCONTAINER_HASHMAP_DEFAULT_CAPACITY
/*
    LIBCONTAINER_HASHMAP_DEFAULT_CAPACITY

    This macro defines the number of buckets to initialize the Hashmap with
    on creation. This should be enough to be reasonably efficient for small
    collections, but large enough to not need immediate rehashing and resizing.

    This is tunable during build-time be re-defining this macro
    with the desired integer value.
*/
#define LIBCONTAINER_HASHMAP_DEFAULT_CAPACITY 16
#endif

struct Hashmap_t {

    /*
        HashFunc is the specific hash function to apply to the given Keys to compute the hash
        for locating the corresponding value into the Array_t of List_t's. This translates
        the arbitrary Key into an integer representation.
    */
    HashFunc_t* HashFunc;

    /*
        Buckets is the Array of "buckets", holding the values which hash and reduce to the same
        index. Currently, this is implemented as an array of linked lists, but may change to a
        more efficient structure in the future.
    */
    Array_t* Buckets;

    /*
        ItemCount is the count of key-value pairs within the Hashmap, or the "length" of the Hashmap.
    */
    size_t ItemCount;

    /*
        KeySize allows caching of the key size when the Map is initialized with a Key type that
        is always the same size as measured in bytes. For something like an Integer or Double key,
        or even a struct-type that has no pointers, this can cache the size so future calls
        don't need to explicitly pass the key size.
    */
    size_t KeySize;

    /*
        KeyReleaseFunc defines the function to pass in to each of the entries of the Hashmap
        to allow them to release their memory when either the entire Hashmap is released,
        or when the entry is deleted. This must safely release ALL memory and resources
        held by the key.
    */
    ReleaseFunc_t* KeyReleaseFunc;
};

/* ++++++++++ Private Functions ++++++++++ */

/*
    Hashmap_rehash

    This function is the entry-point for performing a full re-hash and rearrangement
    of the entries within the Hashmap when the actual Load Factor exceeds the threshold.
    This will re-compute the locations to store each of the entries of the Hashmap,
    growing the Buckets array and reorganizing to improve the distribution of entries
    over the set of buckets available.

    Inputs:
    Map     -   Pointer to the Hashmap to perform the rehash on.

    Outputs:
    None, the map is rehashed and the entries are rearranged.
*/
void Hashmap_rehash(Hashmap_t* Map);

/*
    Hashmap_getBucket

    This function will return the *Bucket* for a given Key. If HashValue points to a non-zero value,
    this is assumed to be a previously cached and valid HashValue, and will skip recomputing it.

    Inputs:
    Map         -   Pointer to the Hashmap_t to operate on.
    Key         -   Pointer to the Key Value to use.
    KeySize     -   Size of the Key value, as measured in bytes.
    HashValue   -   Pointer to either the pre-computed Hash value, or an out
                        parameter to write the newly computed hash value.

    Outputs:
    List_t*     -   Pointer to the List_t Bucket associated with the given Key, or NULL on failure.
*/
List_t* Hashmap_getBucket(Hashmap_t* Map, const void* Key, size_t KeySize, unsigned int* HashValue);

/*
    Hashmap_findInBucket

    This function will search the given Bucket, and return the Value associated with
    the given Key, if it exists.

    Inputs:
    Bucket      -   Pointer to the Bucket corresponding to the Key.
    Key         -   Pointer to the Key Value to use.
    KeySize     -   Size of the Key value, as measured in bytes.
    HashValue   -   The HashValue associated with the Key, to help identify the
                        corresponding value.

    Outputs:
    void*   -   Pointer to the value associated with the given Key, or NULL if it is not in the Hashmap.
*/
void *Hashmap_findInBucket(const List_t *Bucket, const void *Key, size_t KeySize, unsigned int HashValue);

/*
    Hashmap_insertEntry

    This function will actually insert a Hashmap_Entry_t into the Hashmap.

    Inputs:
    Map             -   Pointer to the Hashmap to insert the entry into.
    Entry           -   Pointer to the Entry to insert.
    AttemptRehash   -   Should the Hashmap try to rehash after adding the entry?
                            False during a rehash.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Hashmap_insertEntry(Hashmap_t *Map, Hashmap_Entry_t *Entry, bool AttemptRehash);

/* ---------- Private Functions ---------- */

#if defined(TESTING) || defined(DEBUGGER)

#include "hashmap_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
