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

#include "../../logging/logging.h"
#include "include/tree_rotations.h"

int Binary_Tree_balanceFactor(Binary_Tree_Node_t *Root) {

    ssize_t RightHeight = 0, LeftHeight = 0;

    if ( NULL == Root ) {
        return 0;
    }

    RightHeight = (ssize_t)Binary_Tree_Node_Height(Root->RightChild);
    LeftHeight  = (ssize_t)Binary_Tree_Node_Height(Root->LeftChild);

    return (int)(RightHeight - LeftHeight);
}

Binary_Tree_Node_t *Binary_Tree_rebalance(Binary_Tree_Node_t *Root) {

    int BalanceFactor = 0;

    if ( NULL == Root ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Cannot perform tree rotations on NULL Root*.");
#endif
        return NULL;
    }

    BalanceFactor = Binary_Tree_balanceFactor(Root);

    if ( 2 <= BalanceFactor ) {
        if ( 0 > Binary_Tree_balanceFactor(Root->RightChild) ) {
            Root = Binary_Tree_rotateDoubleLeft(Root);
        } else {
            Root = Binary_Tree_rotateLeft(Root);
        }
    } else if ( -2 >= BalanceFactor ) {
        if ( 0 < Binary_Tree_balanceFactor(Root->LeftChild) ) {
            Root = Binary_Tree_rotateDoubleRight(Root);
        } else {
            Root = Binary_Tree_rotateRight(Root);
        }
    }

    return Root;
}

Binary_Tree_Node_t *Binary_Tree_rotateLeft(Binary_Tree_Node_t *Root) {

    Binary_Tree_Node_t *Pivot = NULL;

    if ( NULL == Root ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Cannot perform tree rotations on NULL Root*.");
#endif
        return NULL;
    }

    if ( NULL == Root->RightChild ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Cannot perform Left rotation with NULL Right Child.");
#endif
        return Root;
    }

    Pivot            = Root->RightChild;
    Pivot->Parent    = Root->Parent;
    Root->Parent     = Pivot;
    Root->RightChild = Pivot->LeftChild;
    if ( NULL != Root->RightChild ) {
        Root->RightChild->Parent = Root;
    }
    Pivot->LeftChild = Root;

    return Pivot;
}

Binary_Tree_Node_t *Binary_Tree_rotateRight(Binary_Tree_Node_t *Root) {

    Binary_Tree_Node_t *Pivot = NULL;

    if ( NULL == Root ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Cannot perform tree rotations on NULL Root*.");
#endif
        return NULL;
    }

    if ( NULL == Root->LeftChild ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Cannot perform Left rotation with NULL Left Child.");
#endif
        return Root;
    }

    Pivot           = Root->LeftChild;
    Pivot->Parent   = Root->Parent;
    Root->Parent    = Pivot;
    Root->LeftChild = Pivot->RightChild;
    if ( NULL != Root->LeftChild ) {
        Root->LeftChild->Parent = Root;
    }
    Pivot->RightChild = Root;

    return Pivot;
}

Binary_Tree_Node_t *Binary_Tree_rotateDoubleLeft(Binary_Tree_Node_t *Root) {

    if ( NULL == Root ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Cannot perform tree rotations on NULL Root*.");
#endif
        return NULL;
    }

    Root->RightChild = Binary_Tree_rotateRight(Root->RightChild);
    Root             = Binary_Tree_rotateLeft(Root);

    return Root;
}

Binary_Tree_Node_t *Binary_Tree_rotateDoubleRight(Binary_Tree_Node_t *Root) {

    if ( NULL == Root ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Cannot perform tree rotations on NULL Root*.");
#endif
        return NULL;
    }

    Root->LeftChild = Binary_Tree_rotateLeft(Root->LeftChild);
    Root            = Binary_Tree_rotateRight(Root);

    return Root;
}
