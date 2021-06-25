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
#include <string.h>

#include "../logging/logging.h"
#include "include/hashmap_entry.h"

Hashmap_Entry_t *Hashmap_Entry_Create(void *Key, void *Value, size_t KeySize, size_t ValueSize,
                                      unsigned int HashValue, ReleaseFunc_t *KeyReleaseFunc,
                                      ReleaseFunc_t *ValueReleaseFunc) {

    Hashmap_Entry_t *Entry          = NULL;
    bool             KeyIsReference = false, ValueIsReference = false;

    if ( (NULL == Key) || (NULL == Value) ) {
        DEBUG_PRINTF("%s", "Error: NULL Key* or Value* provided.");
        return NULL;
    }

    KeyIsReference   = (KeySize == 0);
    ValueIsReference = (ValueSize == 0);

    if ( NULL == KeyReleaseFunc ) {
        if ( KeyIsReference ) {
            DEBUG_PRINTF("%s", "Error: NULL KeyReleaseFunc* provided for Reference-Type.");
            return NULL;
        } else {
            DEBUG_PRINTF("%s", "Note: NULL KeyReleaseFunc provided for non Reference-Type, "
                               "defaulting to free().");
            KeyReleaseFunc = free;
        }
    }

    if ( NULL == ValueReleaseFunc ) {
        if ( ValueIsReference ) {
            DEBUG_PRINTF("%s", "Error: NULL ValueReleaseFunc* provided for Reference-Type.");
            return NULL;
        } else {
            DEBUG_PRINTF("%s", "Note: NULL ValueReleaseFunc provided for non Reference-Type, "
                               "defaulting to free().");
            ValueReleaseFunc = free;
        }
    }

    Entry = (Hashmap_Entry_t *)calloc(1, sizeof(Hashmap_Entry_t));
    if ( NULL == Entry ) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Hashmap_Entry_t.");
        return NULL;
    }

    Entry->KeyReleaseFunc   = KeyReleaseFunc;
    Entry->ValueReleaseFunc = ValueReleaseFunc;

    if ( !KeyIsReference ) {
        Entry->Key = (uint8_t *)calloc(KeySize, sizeof(uint8_t));
        if ( NULL == Entry->Key ) {
            DEBUG_PRINTF("%s", "Error: Failed to allocate memory to hold Key.");
            free(Entry);
            return NULL;
        }
        memcpy(Entry->Key, Key, KeySize * sizeof(uint8_t));
        Entry->KeySize = KeySize;
    } else {
        Entry->Key     = (uint8_t *)Key;
        Entry->KeySize = 0;
    }

    if ( !ValueIsReference ) {
        Entry->Value.ValueRaw = calloc(ValueSize, sizeof(uint8_t));
        if ( NULL == Entry->Value.ValueRaw ) {
            DEBUG_PRINTF("%s", "Error: Failed to allocate memory to hold Value.");
            free(Entry);
            return NULL;
        }
        memcpy(Entry->Value.ValueBytes, Value, (ValueSize * sizeof(uint8_t)));
        Entry->ValueSize = ValueSize;
    } else {
        Entry->Value.ValueRaw = Value;
        Entry->ValueSize      = 0;
    }

    Entry->HashValue = HashValue;

    DEBUG_PRINTF("%s", "Successfully created Hashmap_Entry_t.");
    return Entry;
}

int Hashmap_Entry_Update(Hashmap_Entry_t *Entry, void *NewValue, size_t NewValueSize,
                         ReleaseFunc_t *NewValueReleaseFunc) {

    bool  NewValueIsReference = false;
    void *NewValueContents    = NULL;

    NewValueIsReference = (NewValueSize == 0);

    if ( NULL == Entry ) {
        DEBUG_PRINTF("%s", "Error: NULL Entry* provided.");
        return 1;
    }

    if ( NULL == NewValue ) {
        DEBUG_PRINTF("%s", "Error: NULL NewValue* provided.");
        return 1;
    }

    if ( NULL == NewValueReleaseFunc ) {
        if ( NewValueIsReference ) {
            DEBUG_PRINTF("%s", "Error: NULL NewValueIsReference* provided for Reference-Type.");
            return 1;
        } else {
            DEBUG_PRINTF("%s", "Note: NULL NewValueIsReference provided for non Reference-Type, "
                               "defaulting to free().");
            NewValueReleaseFunc = free;
        }
    }

    if ( !NewValueIsReference ) {
        NewValueContents = calloc(NewValueSize, sizeof(uint8_t));
        if ( NULL == NewValueContents ) {
            DEBUG_PRINTF("%s", "Error: Failed to allocate memory to hold new Value.");
            return 1;
        }
        memcpy(NewValueContents, NewValue, NewValueSize);
    } else {
        NewValueContents = (void *)NewValue;
    }

    Entry->ValueReleaseFunc(Entry->Value.ValueRaw);

    Entry->Value.ValueRaw   = NewValueContents;
    Entry->ValueSize        = NewValueSize;
    Entry->ValueReleaseFunc = NewValueReleaseFunc;

    DEBUG_PRINTF("%s", "Successfully updated Value of Hashmap_Entry_t.");
    return 0;
}

void Hashmap_Entry_Release(Hashmap_Entry_t *Entry) {

    if ( NULL == Entry ) {
        DEBUG_PRINTF("%s", "Note: NULL Entry* provided, nothing to release.");
        return;
    }

    /* If the pointer isn't NULL, AND this Entry owns the memory, release it. */
    if ( (NULL != Entry->Value.ValueRaw) && (NULL != Entry->ValueReleaseFunc) ) {
        Entry->ValueReleaseFunc(Entry->Value.ValueRaw);
    }

    /* If the pointer isn't NULL, AND this Entry owns the memory, release it. */
    if ( (NULL != Entry->Key) && (NULL != Entry->KeyReleaseFunc) ) {
        Entry->KeyReleaseFunc(Entry->Key);
    }

    ZERO_CONTAINER(Entry, Hashmap_Entry_t);

    free(Entry);
    return;
}
