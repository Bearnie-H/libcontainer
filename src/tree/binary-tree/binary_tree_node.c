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

#include "../../logging/logging.h"
#include "include/binary_tree_node.h"

Binary_Tree_Node_t *Binary_Tree_Node_Create(void *Key, size_t KeySize,
                                            ReleaseFunc_t *KeyReleaseFunc, void *Value,
                                            size_t ValueSize, ReleaseFunc_t *ValueReleaseFunc) {

    Binary_Tree_Node_t *Node          = NULL;
    uint8_t *           KeyContents   = NULL;
    uint8_t *           ValueContents = NULL;

    if ( NULL == KeyReleaseFunc ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: NULL KeyReleaseFunc* provided, defaulting to free().");
#endif
        KeyReleaseFunc = free;
    }

    if ( NULL == ValueReleaseFunc ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: NULL ValueReleaseFunc* provided, defaulting to free().");
#endif
        ValueReleaseFunc = free;
    }

    Node = (Binary_Tree_Node_t *)calloc(1, sizeof(Binary_Tree_Node_t));
    if ( NULL == Node ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Binary_Tree_Node_t.");
#endif
        return NULL;
    }

    if ( KeySize == 0 ) {
        KeyContents = (uint8_t *)Key;
    } else {
        KeyContents = (uint8_t *)calloc(KeySize, sizeof(uint8_t));
        if ( NULL == KeyContents ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Binary_Tree_Node_t Key.");
#endif
            free(Node);
            return NULL;
        }
        memcpy(KeyContents, Key, KeySize);
    }

    if ( (NULL == Value) || (ValueSize == 0) ) {
        ValueContents = (uint8_t *)Value;
    } else {
        ValueContents = (uint8_t *)calloc(ValueSize, sizeof(uint8_t));
        if ( NULL == ValueContents ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Binary_Tree_Node_t contents.");
#endif
            free(Node);
            free(KeyContents);
            return NULL;
        }
        memcpy(ValueContents, Value, ValueSize);
    }

    Node->KeyReleaseFunc = KeyReleaseFunc;
    Node->Key.KeyBytes   = KeyContents;
    Node->KeySize        = KeySize;

    Node->ValueReleaseFunc = ValueReleaseFunc;
    Node->Value.ValueBytes = ValueContents;
    Node->ValueSize        = ValueSize;

    Node->Parent     = NULL;
    Node->LeftChild  = NULL;
    Node->RightChild = NULL;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully created Binary_Tree_Node_t.");
#endif
    return Node;
}

size_t Binary_Tree_Node_Height(Binary_Tree_Node_t *Root) {

    size_t LeftHeight = 0, RightHeight = 0;

    if ( NULL == Root ) {
        return 0;
    }

    LeftHeight  = Binary_Tree_Node_Height(Root->LeftChild);
    RightHeight = Binary_Tree_Node_Height(Root->RightChild);

    return 1 + ((LeftHeight > RightHeight) ? (LeftHeight) : RightHeight);
}

int Binary_Tree_Node_UpdateKey(Binary_Tree_Node_t *Node, void *NewKey, size_t KeySize) {

    uint8_t *TempKey = NULL;

    if ( (NULL == NewKey) || (0 == KeySize) ) {
        TempKey = (uint8_t *)NewKey;
    } else {
        TempKey = (uint8_t *)calloc(KeySize, sizeof(uint8_t));
        if ( NULL == TempKey ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to allocate memory for new Key.");
#endif
            return 1;
        }
        memcpy(TempKey, NewKey, KeySize);
    }

    if ( (Node->Key.KeyRaw != NULL) && (NULL != Node->KeyReleaseFunc) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Binary_Tree_Node_t has non-NULL Key, releasing.");
#endif
        Node->KeyReleaseFunc(Node->Key.KeyRaw);
    }

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully updated Key of Binary_Tree_Node_t.");
#endif
    Node->Key.KeyBytes = TempKey;
    Node->KeySize      = KeySize;
    return 0;
}

int Binary_Tree_Node_UpdateValue(Binary_Tree_Node_t *Node, void *NewValue, size_t ValueSize) {

    uint8_t *TempValue = NULL;

    if ( (NULL == NewValue) || (0 == ValueSize) ) {
        TempValue = (uint8_t *)NewValue;
    } else {
        TempValue = (uint8_t *)calloc(ValueSize, sizeof(uint8_t));
        if ( NULL == TempValue ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to allocate memory for new Value.");
#endif
            return 1;
        }
        memcpy(TempValue, NewValue, ValueSize);
    }

    if ( (Node->Value.ValueRaw != NULL) && (NULL != Node->ValueReleaseFunc) ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Binary_Tree_Node_t has non-NULL value, releasing.");
#endif
        Node->ValueReleaseFunc(Node->Value.ValueRaw);
    }

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully updated value of Binary_Tree_Node_t.");
#endif
    Node->Value.ValueBytes = TempValue;
    Node->ValueSize        = ValueSize;
    return 0;
}

void Binary_Tree_Node_Release(Binary_Tree_Node_t *Node) {

    Binary_Tree_Node_t *Parent = NULL;

    if ( NULL == Node ) {
        return;
    }

    Binary_Tree_Node_Release(Node->LeftChild);
    Binary_Tree_Node_Release(Node->RightChild);

    if ( (NULL != Node->Key.KeyRaw) && (NULL != Node->KeyReleaseFunc) ) {
        Node->KeyReleaseFunc(Node->Key.KeyRaw);
    }

    if ( (NULL != Node->Value.ValueRaw) && (NULL != Node->ValueReleaseFunc) ) {
        Node->ValueReleaseFunc(Node->Value.ValueRaw);
    }

    Parent = Node->Parent;
    if ( NULL != Parent ) {
        if ( Node == Parent->LeftChild ) {
            Parent->LeftChild = NULL;
        } else if ( Node == Parent->RightChild ) {
            Parent->RightChild = NULL;
        } else {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Node* is not a child of its Parent!");
#endif
        }
    }

    ZERO_CONTAINER(Node, Binary_Tree_Node_t);
    free(Node);

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully release Binary_Tree_Node_t.");
#endif
    return;
}
