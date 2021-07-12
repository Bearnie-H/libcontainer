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

#define LIBCONTAINER_ENABLE_BINARY_TREE
#include "../../../include/libcontainer.h"

#include "../../iterator/include/iterator.h"
#include "../../logging/logging.h"
#include "include/binary_tree.h"
#include "include/binary_tree_iterator.h"

int Iterator_Initialize_Binary_Tree(Binary_Tree_t *Tree, Binary_Tree_Direction_t Direction,
                                    bool Forward) {

    Iterator_t *        Iterator = NULL;
    Binary_Tree_Node_t *Node     = NULL;

    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided.");
#endif
        return 1;
    }

    Iterator_Invalidate(&(Tree->Iterator));

    Iterator = (Iterator_t *)calloc(1, sizeof(Iterator_t));
    if ( NULL == Iterator ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory to initialize Iterator.");
#endif
        return 1;
    }

    Node = Tree->Root;
    switch ( Direction ) {
        case Direction_InOrder:
            if ( Forward ) {
                for ( ; NULL != Node->LeftChild; Node = Node->LeftChild ) { ; }
            } else {
                for ( ; NULL != Node->RightChild; Node = Node->RightChild ) { ; }
            }
            break;
        case Direction_PreOrder:
            if ( !Forward ) {
                for ( ; NULL != Node->RightChild; Node = Node->RightChild ) { ; }
            }
            break;
        case Direction_PostOrder:
            if ( Forward ) {
                Node = Binary_Tree_leftmostLeaf(Tree->Root);
                if ( NULL == Node ) {
#ifdef DEBUG
                    DEBUG_PRINTF(
                        "%s",
                        "Error: Failed to find Left-most leaf of Binary Tree to start Iterator.");
#endif
                    free(Iterator);
                    return 1;
                }
            }
            break;
        default:
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Unknown traversal direction specified.");
#endif
            return 1;
    }

    Iterator->Context        = Node;
    Iterator->ReleaseContext = NULL;
    Iterator->Type           = Iterator_BinaryTree;

#ifdef DEBUG
    DEBUG_PRINTF("%s", "Successfully prepared Binary Tree Iterator.");
#endif
    Tree->Iterator = Iterator;
    return 0;
}

Binary_Tree_KeyValuePair_t Binary_Tree_Next(Binary_Tree_t *         Tree,
                                            Binary_Tree_Direction_t Direction) {

    Binary_Tree_KeyValuePair_t KeyValuePair = {NULL, NULL};
    Binary_Tree_Node_t *       Current      = NULL;

    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided.");
#endif
        return KeyValuePair;
    }

    if ( (NULL == Tree->Iterator) || (Tree->Iterator->Type == Iterator_UNKNOWN) ) {
        if ( 0 != Iterator_Initialize_Binary_Tree(Tree, Direction, true) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to initialize Binary Tree Iterator.");
#endif
            return KeyValuePair;
        }
        Current = (Binary_Tree_Node_t *)Tree->Iterator->Context;

        KeyValuePair.Key   = Current->Key.KeyRaw;
        KeyValuePair.Value = Current->Value.ValueRaw;
        return KeyValuePair;
    }

    Current = (Binary_Tree_Node_t *)Tree->Iterator->Context;

    switch ( Direction ) {
        case Direction_InOrder: Current = Binary_Tree_inOrderNext(Current); break;
        case Direction_PreOrder: Current = Binary_Tree_preOrderNext(Current); break;
        case Direction_PostOrder: Current = Binary_Tree_postOrderNext(Current); break;
        default:
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Unknown Tree traversal direction specified.");
#endif
            return KeyValuePair;
    }

    if ( NULL == Current ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: Iteration has reached the end of the Tree.");
#endif
        Iterator_Invalidate(&(Tree->Iterator));
        return KeyValuePair;
    }

    Tree->Iterator->Context = Current;
    KeyValuePair.Key        = Current->Key.KeyRaw;
    KeyValuePair.Value      = Current->Value.ValueRaw;

    return KeyValuePair;
}

Binary_Tree_KeyValuePair_t Binary_Tree_Previous(Binary_Tree_t *         Tree,
                                                Binary_Tree_Direction_t Direction) {

    Binary_Tree_KeyValuePair_t KeyValuePair = {NULL, NULL};
    Binary_Tree_Node_t *       Current      = NULL;

    if ( NULL == Tree ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Tree* provided.");
#endif
        return KeyValuePair;
    }

    if ( (NULL == Tree->Iterator) || (Tree->Iterator->Type == Iterator_UNKNOWN) ) {
        if ( 0 != Iterator_Initialize_Binary_Tree(Tree, Direction, false) ) {
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Failed to initialize Binary Tree Iterator.");
#endif
            return KeyValuePair;
        }
        Current = (Binary_Tree_Node_t *)Tree->Iterator->Context;

        KeyValuePair.Key   = Current->Key.KeyRaw;
        KeyValuePair.Value = Current->Value.ValueRaw;
        return KeyValuePair;
    }

    Current = (Binary_Tree_Node_t *)Tree->Iterator->Context;

    switch ( Direction ) {
        case Direction_InOrder: Current = Binary_Tree_inOrderPrevious(Current); break;
        case Direction_PreOrder: Current = Binary_Tree_preOrderPrevious(Current); break;
        case Direction_PostOrder: Current = Binary_Tree_postOrderPrevious(Current); break;
        default:
#ifdef DEBUG
            DEBUG_PRINTF("%s", "Error: Unknown Tree traversal direction specified.");
#endif
            return KeyValuePair;
    }

    if ( NULL == Current ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Note: Iteration has reached the end of the Tree.");
#endif
        Iterator_Invalidate(&(Tree->Iterator));
        return KeyValuePair;
    }

    Tree->Iterator->Context = Current;
    KeyValuePair.Key        = Current->Key.KeyRaw;
    KeyValuePair.Value      = Current->Value.ValueRaw;

    return KeyValuePair;
}

/* ++++++++++ Private Functions ++++++++++ */

Binary_Tree_Node_t *Binary_Tree_inOrderNext(Binary_Tree_Node_t *Current) {

    if ( NULL == Current ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Current* provided.");
#endif
        return NULL;
    }

    if ( NULL != Current->RightChild ) {
        Current = Current->RightChild;
        for ( ; NULL != Current->LeftChild; Current = Current->LeftChild ) { ; }
        return Current;
    }

    while ( NODE_IS_RIGHT_CHILD(Current) ) { Current = Current->Parent; }

    return Current->Parent;
}

Binary_Tree_Node_t *Binary_Tree_preOrderNext(Binary_Tree_Node_t *Current) {

    if ( NULL == Current ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Current* provided.");
#endif
        return NULL;
    }

    if ( NULL != Current->LeftChild ) {
        return Current->LeftChild;
    } else if ( NULL != Current->RightChild ) {
        return Current->RightChild;
    }

    while ( NODE_IS_RIGHT_CHILD(Current) ) { Current = Current->Parent; }

    if ( NULL == Current->Parent ) {
        return NULL;
    }

    return Current->Parent->RightChild;
}

Binary_Tree_Node_t *Binary_Tree_postOrderNext(Binary_Tree_Node_t *Current) {

    if ( NULL == Current ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Current* provided.");
#endif
        return NULL;
    }

    if ( NULL == Current->Parent ) {
        return NULL;
    }

    if ( NODE_IS_RIGHT_CHILD(Current) || (NULL == Current->Parent->RightChild) ) {
        return Current->Parent;
    }

    return Binary_Tree_leftmostLeaf(Current->Parent->RightChild);
}

Binary_Tree_Node_t *Binary_Tree_inOrderPrevious(Binary_Tree_Node_t *Current) {

    if ( NULL == Current ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Current* provided.");
#endif
        return NULL;
    }

    if ( NULL != Current->LeftChild ) {
        Current = Current->LeftChild;
        while ( NULL != Current->RightChild ) { Current = Current->RightChild; }
        return Current;
    }

    while ( !NODE_IS_RIGHT_CHILD(Current) ) {
        if ( NULL == Current->Parent ) {
            return NULL;
        }
        Current = Current->Parent;
    }

    return Current->Parent;
}

Binary_Tree_Node_t *Binary_Tree_preOrderPrevious(Binary_Tree_Node_t *Current) {

    if ( NULL == Current ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Current* provided.");
#endif
        return NULL;
    }

    if ( NULL == Current->Parent ) {
        return NULL;
    }

    if ( (!NODE_IS_LEFT_CHILD(Current)) && (NULL != Current->Parent->LeftChild) ) {
        Current = Current->Parent->LeftChild;
        for ( ; NULL != Current->RightChild; Current = Current->RightChild ) { ; }
        return Current;
    }

    return Current->Parent;
}

Binary_Tree_Node_t *Binary_Tree_postOrderPrevious(Binary_Tree_Node_t *Current) {

    if ( NULL == Current ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Current* provided.");
#endif
        return NULL;
    }

    if ( NULL != Current->RightChild ) {
        return Current->RightChild;
    } else if ( NULL != Current->LeftChild ) {
        return Current->LeftChild;
    }

    if ( !NODE_IS_LEFT_CHILD(Current) && (NULL != Current->Parent->LeftChild) ) {
        return Current->Parent->LeftChild;
    }

    while ( NULL != Current->Parent ) {
        if ( NODE_IS_RIGHT_CHILD(Current->Parent) && (NULL != Current->Parent->Parent) &&
             (NULL != Current->Parent->Parent->LeftChild) ) {
            return Current->Parent->Parent->LeftChild;
        }
        Current = Current->Parent;
    }

    return NULL;
}

Binary_Tree_Node_t *Binary_Tree_leftmostLeaf(Binary_Tree_Node_t *Root) {

    if ( NULL == Root ) {
#ifdef DEBUG
        DEBUG_PRINTF("%s", "Error: NULL Root* provided.");
#endif
        return NULL;
    }

    if ( NULL != Root->LeftChild ) {
        return Binary_Tree_leftmostLeaf(Root->LeftChild);
    } else if ( NULL != Root->RightChild ) {
        return Binary_Tree_leftmostLeaf(Root->RightChild);
    } else {

        return Root;
    }
}

/* ---------- Private Functions ---------- */
