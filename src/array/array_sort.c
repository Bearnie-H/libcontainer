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

#include "../logging/logging.h"
#include "include/array.h"

int Array_Sort(Array_t *Array, CompareFunc_t *CompareFunc) {

    if ( NULL == Array ) {
        DEBUG_PRINTF("%s", "Error: NULL Array* provided.");
        return 1;
    }

    Iterator_Invalidate(&(Array->Iterator));

    if ( NULL == CompareFunc ) {
        DEBUG_PRINTF("%s", "Error: NULL CompareFunc* provided.");
        return 1;
    }

    return Array_doQuickSort(Array, CompareFunc, 0, (ssize_t)Array_Length(Array) - 1);
}

/* ++++++++++ Private Functions ++++++++++ */

void Array_swapItems(Array_t *Array, size_t IndexA, size_t IndexB) {

    uint8_t *ItemA = NULL, *ItemB = NULL;
    uint8_t  TempByte  = 0;
    size_t   ByteIndex = 0;

    if ( IndexA == IndexB ) {
        return;
    }

    if ( 0 != Array->ElementSize ) {
        ItemA = &(Array->Contents.ContentBytes[Array->ElementSize * IndexA]);
        ItemB = &(Array->Contents.ContentBytes[Array->ElementSize * IndexB]);

        for ( ByteIndex = 0; ByteIndex < Array->ElementSize; ByteIndex++ ) {
            TempByte = *ItemA;
            *ItemA   = *ItemB;
            *ItemB   = TempByte;

            ItemA++, ItemB++;
        }
    } else {
        ItemA = Array->Contents.ContentRefs[IndexA];
        ItemB = Array->Contents.ContentRefs[IndexB];

        Array->Contents.ContentRefs[IndexA] = ItemB;
        Array->Contents.ContentRefs[IndexB] = ItemA;
    }

    return;
}

ssize_t Array_doPartition(Array_t *Array, CompareFunc_t *CompareFunc, ssize_t Lower,
                          ssize_t Upper) {

    void *  Pivot = NULL, *Test = NULL;
    ssize_t i = Lower, j = Lower;
    ssize_t p = ((ssize_t)rand() % (Upper - Lower + 1)) + Lower;

    Array_swapItems(Array, (size_t)p, (size_t)Upper);

    Pivot = Array_GetElement(Array, (size_t)Upper);
    if ( NULL == Pivot ) {
        DEBUG_PRINTF("%s", "Error: Failed to retrieve item during Sort().");
        return -1;
    }

    for ( j = Lower; j < Upper; j++ ) {
        Test = Array_GetElement(Array, (size_t)j);
        if ( NULL == Test ) {
            DEBUG_PRINTF("%s", "Error: Failed to retrieve item during Sort().");
            return -1;
        }

        if ( 0 >= CompareFunc(Test, Pivot, Array->ElementSize) ) {
            Array_swapItems(Array, (size_t)j, (size_t)i);
            i += 1;
        }
    }

    Array_swapItems(Array, (size_t)i, (size_t)Upper);
    return i;
}

int Array_doQuickSort(Array_t *Array, CompareFunc_t *CompareFunc, ssize_t Lower, ssize_t Upper) {

    ssize_t Pivot = 0;

    if ( Lower < Upper ) {

        Pivot = Array_doPartition(Array, CompareFunc, Lower, Upper);
        if ( 0 > Pivot ) {
            DEBUG_PRINTF("%s", "Error: Failed to perform Sort() on Array_t.");
            return 1;
        }

        if ( 0 != Array_doQuickSort(Array, CompareFunc, Lower, (Pivot - 1)) ) {
            DEBUG_PRINTF("%s", "Error: Failed to perform Sort() on Array_t.");
            return 1;
        }

        if ( 0 != Array_doQuickSort(Array, CompareFunc, (Pivot + 1), Upper) ) {
            DEBUG_PRINTF("%s", "Error: Failed to perform Sort() on Array_t.");
            return 1;
        }
    }

    return 0;
}

/* ---------- Private Functions ---------- */
