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

#define LIBCONTAINER_ENABLE_STRING

#include "../array/include/array.h"
#include "../logging/logging.h"
#include "include/string.h"

String_t *String_Create(char *Value, size_t Length) {

    String_t *String = NULL;

    String = (String_t *)calloc(1, sizeof(String_t));
    if (NULL == String) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for String_t.");
        return NULL;
    }

    String->Contents = Array_Create(Length, sizeof(char));
    if (NULL == String->Contents) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for String_t contents.");
        free(String);
        return NULL;
    }

    String->IsConst = false;

    if (NULL == Value) {
        return String;
    }

    /* A Non-NULL initializer string was provided, so set the starting string contents to this
     * value. */
    if (0 != Array_InsertN(String->Contents, Value, 0, Length)) {
        DEBUG_PRINTF("%s", "Error: Failed to initialize String_t with given value.");
        String_Release(String);
        return NULL;
    }

    return String;
}

String_t *String_CreateConst(char *Value) {

    String_t *String = NULL;

    String = (String_t *)calloc(1, sizeof(String_t));
    if (NULL == String) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for String_t.");
        return NULL;
    }

    String->Contents = Array_Create(0, sizeof(char));
    if (NULL == String->Contents) {
        DEBUG_PRINTF("%s", "Error: Failed to allocate memory for String_t contents.");
        free(String);
        return NULL;
    }

    free(String->Contents->Contents.ContentBytes);
    String->Contents->Contents.ContentBytes = (uint8_t *)Value;
    String->IsConst = true;

    return String;
}

size_t String_Length(String_t *String) {

    if (NULL == String) {
        DEBUG_PRINTF("%s", "Note: NULL String* provided, cannot check length.");
        return 0;
    }

    return Array_Length(String->Contents);
}

bool String_IsEmpty(String_t *String) { return (0 == String_Length(String)); }

char String_GetAtIndex(String_t *String, size_t Index) {

    char *Character = NULL;

    if (NULL == String) {
        DEBUG_PRINTF("%s", "Error: NULL String* provided.");
        return 0x00;
    }

    Character = (char *)Array_GetElement(String->Contents, Index);
    if (NULL == Character) {
        DEBUG_PRINTF("Error: Failed to retrieve item at index [ %d ] from String.", (int)Index);
        return 0x00;
    }

    return *Character;
}

char String_GetFront(String_t *String) { return String_GetAtIndex(String, 0); }

char String_GetBack(String_t *String) { return String_GetAtIndex(String, String_Length(String)); }

int String_Insert(String_t *String, char *ToInsert, size_t Index, size_t Length) {

    if (NULL == String) {
        DEBUG_PRINTF("%s", "Error: NULL String* provided.");
        return 1;
    }

    if (String->IsConst) {
        DEBUG_PRINTF("%s", "Error: Cannot modify Const String_t.");
        return 1;
    }

    return Array_InsertN(String->Contents, ToInsert, Index, Length);
}

int String_Prepend(String_t *String, char *ToInsert, size_t Length) {

    if (NULL == String) {
        DEBUG_PRINTF("%s", "Error: NULL String* provided.");
        return 1;
    }

    if (String->IsConst) {
        DEBUG_PRINTF("%s", "Error: Cannot modify Const String_t.");
        return 1;
    }

    return Array_InsertN(String->Contents, ToInsert, 0, Length);
}

int String_Append(String_t *String, char *ToInsert, size_t Length) {

    if (NULL == String) {
        DEBUG_PRINTF("%s", "Error: NULL String* provided.");
        return 1;
    }

    if (String->IsConst) {
        DEBUG_PRINTF("%s", "Error: Cannot modify Const String_t.");
        return 1;
    }

    return Array_InsertN(String->Contents, ToInsert, Array_Length(String->Contents), Length);
}

int String_Replace(String_t *String, char *ToInsert, size_t Index, size_t Length) {

    if (NULL == String) {
        DEBUG_PRINTF("%s", "Error: NULL String* provided.");
        return 1;
    }

    if (String->IsConst) {
        DEBUG_PRINTF("%s", "Error: Cannot modify Const String_t.");
        return 1;
    }

    return Array_ReplaceN(String->Contents, ToInsert, Index, Length);
}

int String_Set(String_t *String, char *NewValue, size_t Length) {

    if (NULL == String) {
        DEBUG_PRINTF("%s", "Error: NULL String* provided.");
        return 1;
    }

    if (String->IsConst) {
        DEBUG_PRINTF("%s", "Error: Cannot modify Const String_t.");
        return 1;
    }

    if (0 != Array_Clear(String->Contents)) {
        DEBUG_PRINTF("%s", "Error: Failed to remove existing contents from String_t.");
        return 1;
    }

    if (NULL != NewValue) {
        return Array_InsertN(String->Contents, NewValue, 0, Length);
    }

    return 0;
}

char *String_ToCString(String_t *String) {

    if (NULL == String) {
        DEBUG_PRINTF("%s", "Error: NULL String* provided.");
        return NULL;
    }

    /* Const String_t values are read-only string literals that are always NUL-terminated. */
    if (String->IsConst) {
        return (char *)String->Contents->Contents.ContentBytes;
    }

    /* Grow the string to hold a NUL-terminator, if there isn't room. */
    if (Array_Capacity(String->Contents) == Array_Length(String->Contents)) {
        if (0 != Array_Grow(String->Contents, 1)) {
            DEBUG_PRINTF("%s", "Error: Failed to grow Array to accommodate NULL-terminator.");
            return NULL;
        }
    }

    /* Enforce there's a NUL-terminator at the end of the string. */
    String->Contents->Contents.ContentBytes[String->Contents->Length] = 0x00;

    return (char *)String->Contents->Contents.ContentBytes;
}

String_t *String_Copy(String_t *String) {

    String_t *Duplicate = NULL;

    if (NULL == String) {
        DEBUG_PRINTF("%s", "Error: NULL String* provided.");
        return NULL;
    }

    if (String->IsConst) {
        Duplicate = String_Create(String_ToCString(String), String_Length(String));
    } else {
        Duplicate = String_CreateConst(String_ToCString(String));
    }

    if (NULL == Duplicate) {
        DEBUG_PRINTF("%s", "Error: Failed to create new String_t copy.");
        return NULL;
    }

    return Duplicate;
}

String_t *String_Substring(String_t *String, size_t Index, size_t Length) {

    size_t StringLength = 0;

    if (NULL == String) {
        DEBUG_PRINTF("%s", "Error: NULL String* provided.");
        return NULL;
    }

    StringLength = String_Length(String);

    if (StringLength <= Index) {
        DEBUG_PRINTF("%s", "Error: Index is out of bounds.");
        return NULL;
    }

    if (Index + Length >= StringLength) {
        Length = StringLength - Index;
    }

    return String_Create(&(String_ToCString(String)[Index]), Length);
}

int String_Compare(String_t *A, String_t *B) {

    size_t MinLength = 0;

    if ((NULL == A) && (NULL == B)) {
        return 0;
    } else if ((NULL == A) && (NULL != B)) {
        return 1;
    } else if ((NULL != A) && (NULL == B)) {
        return -1;
    }

    MinLength = (String_Length(A) < String_Length(B)) ? (String_Length(A)) : (String_Length(B));

    return memcmp(String_ToCString(A), String_ToCString(B), MinLength);
}

int String_Clear(String_t *String) {

    if (NULL == String) {
        DEBUG_PRINTF("%s", "Error: NULL String* provided.");
        return 1;
    }

    if (String->IsConst) {
        DEBUG_PRINTF("%s", "Error: Cannot modify const String_t.");
        return 1;
    }

    if (0 == String_Length(String)) {
        return 0;
    }

    return Array_Clear(String->Contents);
}

void String_Release(String_t *String) {

    if (NULL == String) {
        DEBUG_PRINTF("%s", "Note: Provided String* is NULL, nothing to release.");
        return;
    }

    /* If the string is a const-initialized string, just point it to NULL and release like normal.
     */
    if (String->IsConst) {
        String->Contents->Contents.ContentBytes = NULL;
    }

    Array_Release(String->Contents);
    ZERO_CONTAINER(String, String_t);
    free(String);

    DEBUG_PRINTF("%s", "Successfully released String and held contents.");
    return;
}

/* ++++++++++ Private Functions ++++++++++ */

int String_grow(String_t *String, size_t AdditionalCapacity) {

    if (NULL == String) {
        DEBUG_PRINTF("%s", "Error: NULL String* provided.");
        return 1;
    }

    return Array_Grow(String->Contents, AdditionalCapacity);
}

/* ---------- Private Functions ---------- */
