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

#include <stdint.h>
#include <stdlib.h>

#include "../../logging/logging.h"
#include "include/binary_heap_node.h"

Binary_Heap_Node_t *Binary_Heap_Node_Create(void *Key, void *Value, size_t KeySize,
                                            size_t ValueSize, ReleaseFunc_t *KeyReleaseFunc,
                                            ReleaseFunc_t *ValueReleaseFunc) {

    Binary_Heap_Node_t *Node        = NULL;
    void *              KeyContents = NULL, *ValueContents = NULL;

    if ( NULL == KeyReleaseFunc ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Warning: NULL KeyReleaseFunc* provided, defaulting to free().");
#endif
        KeyReleaseFunc = free;
    }

    if ( NULL == ValueReleaseFunc ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Warning: NULL ValueReleaseFunc* provided, defaulting to free().");
#endif
        ValueReleaseFunc = free;
    }

    Node = (Binary_Heap_Node_t *)calloc(1, sizeof(Binary_Heap_Node_t));
    if ( NULL == Node ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Binary_Heap_Node_t.");
#endif
        return NULL;
    }

    if ( 0 == KeySize ) {
        KeyContents = Key;
    } else {
        KeyContents = calloc(KeySize, sizeof(uint8_t));
        if ( NULL == KeyContents ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s",
                         "Error: Failed to allocate memory for Key within Binary_Heap_Node_t.");
#endif
            free(Node);
            return NULL;
        }
        memcpy(KeyContents, Key, KeySize);
    }

    if ( 0 == ValueSize ) {
        ValueContents = Value;
    } else {
        ValueContents = calloc(ValueSize, sizeof(uint8_t));
        if ( NULL == ValueContents ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s",
                         "Error: Failed to allocate memory for Value within Binary_Heap_Node_t.");
#endif
            free(Node);
            free(KeyContents);
            return NULL;
        }
        memcpy(ValueContents, Value, ValueSize);
    }

    Node->Key            = KeyContents;
    Node->KeySize        = KeySize;
    Node->KeyReleaseFunc = KeyReleaseFunc;

    Node->Value            = ValueContents;
    Node->ValueSize        = ValueSize;
    Node->ValueReleaseFunc = ValueReleaseFunc;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully created and initialized Binary_Heap_Node_t.");
#endif
    return Node;
}

int Binary_Heap_Node_UpdateKey(Binary_Heap_Node_t *Node, void *NewKey, size_t NewKeySize) {

    void *NewKeyContents = NULL;

    if ( NULL == Node ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Node* provided.");
#endif
        return 1;
    }

    if ( NULL == NewKey ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL NewKey* provided.");
#endif
        return 1;
    }

    if ( 0 == NewKeySize ) {
        NewKeyContents = NewKey;
    } else {
        NewKeyContents = calloc(NewKeySize, sizeof(uint8_t));
        if ( NULL == NewKeyContents ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to allocate memory to hold new Key.");
#endif
            return 1;
        }
        memcpy(NewKeyContents, NewKey, NewKeySize);
    }

    if ( (NULL != Node->Key) && (NULL != Node->KeyReleaseFunc) ) {
        Node->KeyReleaseFunc(Node->Key);
    }

    Node->Key     = NewKeyContents;
    Node->KeySize = NewKeySize;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully updated Binary_Heap_Node_t Key.");
#endif
    return 0;
}

int Binary_Heap_Node_UpdateValue(Binary_Heap_Node_t *Node, void *NewValue, size_t NewValueSize) {

    void *NewValueContents = NULL;

    if ( NULL == Node ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Node* provided.");
#endif
        return 1;
    }

    if ( NULL == NewValue ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL NewValue* provided.");
#endif
        return 1;
    }

    if ( 0 == NewValueSize ) {
        NewValueContents = NewValue;
    } else {
        NewValueContents = calloc(NewValueSize, sizeof(uint8_t));
        if ( NULL == NewValueContents ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to allocate memory to hold new Value.");
#endif
            return 1;
        }
        memcpy(NewValueContents, NewValue, NewValueSize);
    }

    if ( (NULL != Node->Value) && (NULL != Node->ValueReleaseFunc) ) {
        Node->ValueReleaseFunc(Node->Value);
    }

    Node->Value     = NewValueContents;
    Node->ValueSize = NewValueSize;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully updated Binary_Heap_Node_t Value.");
#endif
    return 0;
}

void Binary_Heap_Node_Release(Binary_Heap_Node_t *Node) {

    if ( NULL == Node ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: NULL Node* provided, nothing to release.");
#endif
        return;
    }

    if ( (NULL != Node->Key) && (NULL != Node->KeyReleaseFunc) ) {
        Node->KeyReleaseFunc(Node->Key);
    }

    if ( (NULL != Node->Value) && (NULL != Node->ValueReleaseFunc) ) {
        Node->ValueReleaseFunc(Node->Value);
    }

    ZERO_CONTAINER(Node, Binary_Heap_Node_t);
    free(Node);

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully released Binary_Heap_Node_t.");
#endif
    return;
}
