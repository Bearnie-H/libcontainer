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

#ifndef LIBCONTAINER_HASHMAP_ENTRY_H
#define LIBCONTAINER_HASHMAP_ENTRY_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../../../include/libcontainer.h"

/*
    Hashmap_Entry_t

    This is the internal representation of a given entry within a Hashmap_t struct.
    This contains all of the necessary values to track both of the key-value pair, as well as
    safely handing the resources either may hold. In addition, this tracks the raw HashValue
    associated with the key, to simplify the Hashmap table expansion process.
*/
typedef struct Hashmap_Entry_t {

    /* Key holds the raw bytes of the key of this entry, regardless of the type. */
    uint8_t* Key;

    /* Value holds the raw bytes of the value of this entry, regardless of the type. */
    uint8_t* Value;

    /* HashValue holds the full raw hash value for the key, before it is reduced to the bucket index
    used by the encompassing Hashmap_t. */
    unsigned int HashValue;

    /* KeySize holds the size of the memory owned by the Key pointer.
        If this is 0, then the value is a reference and this struct doesn't
        own the memory. */
    size_t KeySize;

    /* ValueSize holds the size of the memory owned by the Value pointer.
        If this is 0, then the value is a reference and this struct doesn't
        own the memory. */
    size_t ValueSize;

    /* KeyReleaseFunc is a pointer to the function to call to safely
        release the memory/resources held by the Key pointer. */
    ReleaseFunc_t* KeyReleaseFunc;

    /* ValueReleaseFunc is a pointer to the function to call to safely
        release the memory/resources held by the Value pointer. */
    ReleaseFunc_t* ValueReleaseFunc;

} Hashmap_Entry_t;

/*
    Hashmap_Entry_Create

    This function will create and initialize a new Hashmap_Entry_t value, ready
    to be inserted into a Hashmap_t bucket.

    Inputs:
    Key                 -   Pointer to the Key to associate with this entry
    Value               -   Pointer to the Value to associate with this entry
    KeySize             -   The number of bytes in the Key.
    ValueSize           -   The number of bytes within the Value.
    HashValue           -   The raw Hash Value associated with his entry.
    KeyReleaseFunc      -   Pointer to the function to call to release the resources associated with the Key.
    ValueReleaseFunc    -   Pointer to the function to call to release the resources associated with the Value.

    Outputs:
    Hashmap_Entry_t*    -   Pointer to the newly constructed Hashmap_Entry_t value, or NULL on failure.
*/
Hashmap_Entry_t* Hashmap_Entry_Create(const void* Key, const void* Value, size_t KeySize, size_t ValueSize, unsigned int HashValue, ReleaseFunc_t* KeyReleaseFunc, ReleaseFunc_t* ValueReleaseFunc);

/*
    Hashmap_Entry_Update

    This function will update the Value of the given Hashmap_Entry_t, simplifying the
    overwrite/update process.

    Inputs:
    Entry                   -   Pointer to the Entry to update.
    NewValue                -   Pointer to the new Value to assign to the Entry.
    NewValueSize            -   The size of the new Value, in bytes.
    NewValueReleaseFunc     -   The function to use to release the new Value

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Hashmap_Entry_Update(Hashmap_Entry_t* Entry, const void* NewValue, size_t NewValueSize, ReleaseFunc_t* NewValueReleaseFunc);

/*
    Hashmap_Entry_Release

    This function will safely release the given Hashmap_Entry_t, including all held resources.

    Inputs:
    Entry   -   Pointer to the Entry to release.

    Outputs:
    None, the resources are released safely and this Entry is no longer accessible for use.
*/
void Hashmap_Entry_Release(Hashmap_Entry_t* Entry);

#if defined(TESTING) || defined(DEBUGGER)

#include "hashmap_entry_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
