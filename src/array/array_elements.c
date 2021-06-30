/*
    Copyright (c) 2021 Bearnie-H

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
*/

#include <stdlib.h>
#include <string.h>

#include "../logging/logging.h"
#include "include/array.h"

int Array_Prepend(Array_t *Array, void *Element) {
    return Array_InsertN(Array, Element, 0, 1);
}

int Array_Append(Array_t *Array, void *Element) {
    return Array_InsertN(Array, Element, Array->Length, 1);
}

int Array_Insert(Array_t *Array, void *Element, size_t Index) {
    return Array_InsertN(Array, Element, Index, 1);
}

int Array_InsertN(Array_t *Array, void *Elements, size_t Index, size_t Count) {

    size_t i = 0;

    if ( (NULL == Array) || (NULL == Elements) ) {
        DEBUG_PRINTF("%s", "Error, NULL Array_t or Element provided.");
        return 1;
    }

    Iterator_Invalidate(&(Array->Iterator));

    if ( (Array->Length < Index) ) {
        DEBUG_PRINTF("Index (%d) is out of bounds.", (int)Index);
        return 1;
    }

    if ( 0 != Array_Grow(Array, Count) ) {
        DEBUG_PRINTF("%s", "Error, failed to ensure array has room to insert new element.");
        return 1;
    }

    if ( 0 == Array->ElementSize ) {
        memmove(&(Array->Contents.ContentRefs[Index + Count]),
                &(Array->Contents.ContentRefs[Index]), (Array->Length - Index) * sizeof(void *));

        for ( i = 0; i < Count; i++ ) {
            Array->Contents.ContentRefs[Index + i] = ((void **)Elements)[i];
        }

    } else {
        memmove(&(Array->Contents.ContentBytes[(Index + Count) * Array->ElementSize]),
                &(Array->Contents.ContentBytes[Index * Array->ElementSize]),
                (Array->Length - Index) * Array->ElementSize);

        memcpy(&(Array->Contents.ContentBytes[Index * Array->ElementSize]), Elements,
               Count * Array->ElementSize);
    }

    Array->Length += Count;

    DEBUG_PRINTF("Successfully inserted [ %d ] element(s) starting at index [ %d ].", (int)Count,
                 (int)Index);
    return 0;
}

int Array_Remove(Array_t *Array, size_t Index) {
    return Array_RemoveN(Array, Index, 1);
}

int Array_RemoveN(Array_t *Array, size_t Index, size_t Count) {

    size_t ReleaseIndex = 0;

    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "Error, NULL Array_t provided.");
        return 1;
    }

    Iterator_Invalidate(&(Array->Iterator));

    if ( (0 == Array->Length) || (Array->Length <= Index) ) {
        DEBUG_PRINTF("Index (%d) is out of bounds.", (int)Index);
        return 1;
    }

    /* If the count would go over the end of the array, truncate to stop at the
     * end instead. */
    if ( (Index + Count) > Array->Length ) {
        Count = Array->Length - Index;
    }

    /*
        If this array doesn't own the memory of the elements about to be
       deleted, call their release function before the memmove() to prevent
       leaks.
    */
    if ( 0 == Array->ElementSize ) {
        for ( ReleaseIndex = 0; ReleaseIndex < Count; ReleaseIndex++ ) {
            Array->ReleaseFunc(Array->Contents.ContentRefs[Index]);
        }

        memmove(&(Array->Contents.ContentRefs[Index]),
                &(Array->Contents.ContentRefs[Index + Count]),
                (Array->Length - Index - Count) * sizeof(void *));

    } else {
        memmove(&(Array->Contents.ContentBytes[Index * Array->ElementSize]),
                &(Array->Contents.ContentBytes[(Index + Count) * Array->ElementSize]),
                (Array->Length - Index - Count) * Array->ElementSize);
    }

    Array->Length -= Count;

    DEBUG_PRINTF("Successfully removed [ %d ] element(s) starting at index [ %d ].", (int)Count,
                 (int)Index);
    return 0;
}

int Array_Replace(Array_t *Array, void *Element, size_t Index) {
    return Array_ReplaceN(Array, Element, Index, 1);
}

int Array_ReplaceN(Array_t *Array, void *Elements, size_t Index, size_t Count) {

    size_t ElementSize = sizeof(void *), i = 0;

    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "Error: NULL Array* provided.");
        return 1;
    }

    if ( (Array->Length <= Index) ) {
        DEBUG_PRINTF("Error: Index (%d) is out of bounds.", (int)Index);
        return 1;
    }

    if ( 0 == Array->ElementSize ) {
        for ( i = 0; i < Count; i++ ) {
            Array->ReleaseFunc(Array->Contents.ContentRefs[Index + i]);
            Array->Contents.ContentRefs[Index + i] = ((void **)Elements)[i];
        }
    } else {
        ElementSize = Array->ElementSize;
        memcpy(&(Array->Contents.ContentBytes[Index * ElementSize]), Elements, Count * ElementSize);
    }

    return 0;
}

void *Array_GetElement(Array_t *Array, size_t Index) {

    void *Element = NULL;

    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "Error, NULL Array_t provided.");
        return NULL;
    }

    if ( (0 == Array->Length) || (Array->Length <= Index) ) {
        DEBUG_PRINTF("Error, requested index [ %d ] is out of bounds.", (int)Index);
        return NULL;
    }

    if ( 0 == Array->ElementSize ) {
        Element = Array->Contents.ContentRefs[Index];
    } else {
        Element = (void *)&(Array->Contents.ContentBytes[Index * Array->ElementSize]);
    }

    DEBUG_PRINTF("Successfully retrieved element at index [ %d ].", (int)Index);
    return Element;
}

int Array_SetElement(Array_t *Array, void *Element, size_t Index) {

    void *ExistingElement = NULL;

    ExistingElement = Array_GetElement(Array, Index);
    if ( NULL == ExistingElement ) {
        DEBUG_PRINTF("Error: Failed to get element at index [ %d ] to update value.", (int)Index);
        return 1;
    }

    if ( 0 == Array->ElementSize ) {
        Array->Contents.ContentRefs[Index] = Element;
    } else {
        memcpy(ExistingElement, Element, Array->ElementSize);
    }

    DEBUG_PRINTF("Successfully set element at index [ %d ].", (int)Index);
    return 0;
}

void *Array_PopElement(Array_t *Array, size_t Index) {

    void *ElementContents = NULL;

    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "Error: NULL Array* provided.");
        return NULL;
    }

    Iterator_Invalidate(&(Array->Iterator));

    /* Get the contents to be returned to the caller (also validate arguments).
     */
    ElementContents = Array_GetElement(Array, Index);
    if ( NULL == ElementContents ) {
        DEBUG_PRINTF("%s", "Error, Failed to GetElement during PopElement.");
        return NULL;
    }

    /* Resize the array, removing the element. */
    if ( 0 == Array->ElementSize ) {
        if ( Index < Array_Length(Array) - 1 ) {
            memmove(Array->Contents.ContentRefs[Index], Array->Contents.ContentRefs[Index + 1],
                    (Array->Length - Index - 1) * sizeof(void *));
        }

    } else {
        if ( Index < Array_Length(Array) - 1 ) {
            memmove(&(Array->Contents.ContentBytes[Index * Array->ElementSize]),
                    &(Array->Contents.ContentBytes[(Index + 1) * Array->ElementSize]),
                    (Array->Length - Index - 1) * Array->ElementSize);
        }
    }

    Array->Length -= 1;

    DEBUG_PRINTF("Successfully popped element at index [ %d ] from Array_t.", (int)Index);
    return ElementContents;
}
