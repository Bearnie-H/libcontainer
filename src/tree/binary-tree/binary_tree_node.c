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

Binary_Tree_Node_t *Binary_Tree_Node_Create(int Key, size_t ValueSize, const void *Value,
                                            ReleaseFunc_t *ReleaseFunc) {

    Binary_Tree_Node_t *Node = NULL;
    uint8_t *Contents = NULL;

    Node = (Binary_Tree_Node_t *)calloc(1, sizeof(Binary_Tree_Node_t));
    if (NULL == Node) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Binary_Tree_Node_t.");
        return NULL;
    }

    if (ValueSize == 0) {
        Contents = (uint8_t *)Value;
    } else {
        Contents = (uint8_t *)calloc(ValueSize, sizeof(uint8_t));
        if (NULL == Contents) {
            DEBUG_PRINTF("%s", "Error: Failed to allocate memory for Binary_Tree_Node_t contents.");
            free(Node);
            return NULL;
        }
        memcpy(Contents, Value, ValueSize);
    }

    Node->Colour = Red;
    Node->Key = Key;

    Node->Parent = NULL;
    Node->LeftChild = NULL;
    Node->RightChild = NULL;

    Node->ReleaseFunc = ReleaseFunc;
    Node->Value = Contents;
    Node->ValueSize = ValueSize;

    DEBUG_PRINTF("%s", "Successfully created Binary_Tree_Node_t.");
    return Node;
}

Binary_Tree_Node_t *Binary_Tree_Node_FindParent(Binary_Tree_Node_t *Root, int Key) {

    if (NULL == Root) {
        DEBUG_PRINTF("%s", "Warning: NULL Node reached without finding Key.");
        return NULL;
    }

    if (Key == Root->Key) {
        DEBUG_PRINTF("%s", "Successfully found both Key and Parent.");
        return Root->Parent;
    } else if (Key < Root->Key) {
        if (NULL == Root->LeftChild) {
            DEBUG_PRINTF("%s", "Successfully found Parent, and Key does not exist.");
            return Root;
        } else {
            return Binary_Tree_Node_FindParent(Root->LeftChild, Key);
        }
    } else {
        if (NULL == Root->RightChild) {
            return Root;
        } else {
            return Binary_Tree_Node_FindParent(Root->RightChild, Key);
        }
    }
}

Binary_Tree_Node_t *Binary_Tree_Node_Find(Binary_Tree_Node_t *Root, int Key) {

    if (NULL == Root) {
        DEBUG_PRINTF("%s", "Warning: NULL Node reached without finding Key.");
        return NULL;
    }

    if (Key == Root->Key) {
        DEBUG_PRINTF("%s", "Successfully found Key within Tree.");
        return Root;
    } else if (Key < Root->Key) {
        return Binary_Tree_Node_Find(Root->LeftChild, Key);
    } else {
        return Binary_Tree_Node_Find(Root->RightChild, Key);
    }
}

int Binary_Tree_Node_Update(Binary_Tree_Node_t *Node, const void *NewValue, size_t ValueSize) {

    uint8_t *TempValue = NULL;

    if (0 == ValueSize) {
        TempValue = (uint8_t *)NewValue;
    } else {
        TempValue = (uint8_t *)calloc(ValueSize, sizeof(uint8_t));
        if (NULL == TempValue) {
            DEBUG_PRINTF("%s", "Error: Failed to allocate memory for new Value.");
            return 1;
        }
        memcpy(TempValue, NewValue, ValueSize);
    }

    if (Node->Value != NULL) {
        DEBUG_PRINTF("%s", "Binary_Tree_Node_t has non-NULL value, releasing.");
        Node->ReleaseFunc(Node->Value);
        Node->Value = NULL;
    }

    DEBUG_PRINTF("%s", "Successfully updated value of Binary_Tree_Node_t.");
    Node->Value = TempValue;
    return 0;
}

void Binary_Tree_Node_Release(Binary_Tree_Node_t *Node) {

    if (NULL == Node) {
        DEBUG_PRINTF("%s", "Note: NULL Node* provided, nothing to release.");
        return;
    }

    Binary_Tree_Node_Release(Node->LeftChild);
    Binary_Tree_Node_Release(Node->RightChild);

    if (NULL != Node->Value) {
        Node->ReleaseFunc(Node->Value);
    }

    ZERO_CONTAINER(Node, Binary_Tree_Node_t);
    free(Node);

    DEBUG_PRINTF("%s", "Successfully release Binary_Tree_Node_t.");
    return;
}
