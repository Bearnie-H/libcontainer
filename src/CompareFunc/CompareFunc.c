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

#include "../../include/libcontainer.h"
#include "../logging/logging.h"
#include "include/CompareFunc.h"

int CompareFunc_Int(const void *A, const void *B, __attribute__((unused)) size_t Size) {
    return (*(const int *)A - *(const int *)B);
}

int CompareFunc_String(const void *A, const void *B, size_t Size) {

    size_t Size_A = 0, Size_B = 0;

    if ( 0 == Size ) {
        DEBUG_PRINTF(
            "%s",
            "Warning: Size of 0 provided, using strlen() to compute length of given strings.");
        Size_A = strlen(A);
        Size_B = strlen(B);
        Size   = (Size_A > Size_B) ? (Size_B) : (Size_A);
        DEBUG_PRINTF("Provided strings have lengths: strlen(A) = (%lu), strlen(B) = (%lu), Using "
                     "(%lu) as size for strncmp().",
                     (unsigned long)Size_A, (unsigned long)Size_B, (unsigned long)Size);
    }

    return strncmp(A, B, Size);
}
