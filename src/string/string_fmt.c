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

#include <stdio.h>
#include <stdlib.h>

#include "../logging/logging.h"
#include "include/string.h"
#include "include/string_fmt.h"

__attribute__((__format__(printf, 1, 2))) String_t *String_Createf(const char *fmt, ...) {

    String_t *String = NULL;
    va_list   Args;

    if ( NULL == fmt ) {
        DEBUG_PRINTF("%s", "Error: NULL fmt* provided.");
        return NULL;
    }

    va_start(Args, fmt);

    String = String_VCreatef(fmt, Args);

    va_end(Args);
    return String;
}

__attribute__((__format__(printf, 1, 0))) String_t *String_VCreatef(const char *fmt,
                                                                    va_list     VarArgs) {

    String_t *String = NULL;
    ssize_t   Length = 0;

    Length = String_getFMTLength(fmt, VarArgs);
    if ( Length < 0 ) {
        DEBUG_PRINTF("%s", "Error: Failed to determine length of resulting formatted string.");
        return NULL;
    }

    String = String_Create(NULL, (size_t)(Length + 1));
    if ( NULL == String ) {
        DEBUG_PRINTF("%s", "Error: Failed to create String_t to write formatted string to.");
        return NULL;
    }

    if ( Length != vsnprintf(String_ToCString(String), (size_t)(Length + 1), fmt, VarArgs) ) {
        DEBUG_PRINTF("%s", "Error: Failed to write formatted string to String.");
        String_Release(String);
        return NULL;
    }
    String->Contents->Length = (size_t)Length;

    return String;
}

__attribute__((__format__(printf, 2, 3))) int String_SPrintf(String_t *String, const char *fmt,
                                                             ...) {

    va_list Args;
    int     RetVal = 0;

    va_start(Args, fmt);

    RetVal = String_VSPrintf(String, fmt, Args);

    va_end(Args);
    return RetVal;
}

__attribute__((__format__(printf, 2, 0))) int String_VSPrintf(String_t *String, const char *fmt,
                                                              va_list VarArgs) {

    ssize_t Length = 0;

    if ( NULL == String ) {
        DEBUG_PRINTF("%s", "Error: NULL String* provided.");
        return 1;
    }

    if ( NULL == fmt ) {
        DEBUG_PRINTF("%s", "Error: NULL fmt* provided.");
        return 1;
    }

    Length = String_getFMTLength(fmt, VarArgs);
    if ( Length < 0 ) {
        DEBUG_PRINTF("%s", "Error: Failed to determine length of resulting formatted string.");
        return 1;
    }

    if ( ((long)Length - (long)String_Length(String)) > 0 ) {
        if ( 0 != String_grow(String, (size_t)((long)Length - (long)String_Length(String))) ) {
            DEBUG_PRINTF("%s", "Error: Failed to grow String to accommodate formatted string.");
            return 1;
        }
    }

    if ( Length != vsnprintf(String_ToCString(String), (size_t)(Length + 1), fmt, VarArgs) ) {
        DEBUG_PRINTF("%s", "Error: Failed to write formatted string to String.");
        return 1;
    }
    String->Contents->Length = (size_t)Length;

    return 0;
}

__attribute__((__format__(printf, 2, 3))) int String_Appendf(String_t *String, const char *fmt,
                                                             ...) {

    va_list Args;
    int     RetVal = 0;

    if ( NULL == String ) {
        DEBUG_PRINTF("%s", "Error: NULL String* provided.");
        return 1;
    }

    if ( NULL == fmt ) {
        DEBUG_PRINTF("%s", "Error: NULL fmt* provided.");
        return 1;
    }

    va_start(Args, fmt);

    RetVal = String_VAppendf(String, fmt, Args);

    va_end(Args);
    return RetVal;
}

__attribute__((__format__(printf, 2, 0))) int String_VAppendf(String_t *String, const char *fmt,
                                                              va_list VarArgs) {

    ssize_t Length = 0;

    Length = String_getFMTLength(fmt, VarArgs);
    if ( Length < 0 ) {
        DEBUG_PRINTF("%s", "Error: Failed to determine length of resulting formatted string.");
        return 1;
    }

    if ( 0 != String_grow(String, (size_t)Length + 1) ) {
        DEBUG_PRINTF("%s", "Error: Failed to grow String to accommodate formatted string.");
        return 1;
    }

    if ( Length != vsnprintf(&(String_ToCString(String)[String_Length(String)]),
                             (size_t)(Length + 1), fmt, VarArgs) ) {
        DEBUG_PRINTF("%s", "Error: Failed to write formatted string to String.");
        return 1;
    }
    String->Contents->Length += (size_t)Length;

    return 0;
}

/* ++++++++++ Private Library Functions ++++++++++ */

__attribute__((__format__(printf, 1, 0))) ssize_t String_getFMTLength(const char *fmt,
                                                                      va_list     args) {

    va_list ArgsCopy;
    ssize_t Length = 0;

    va_copy(ArgsCopy, args);

    Length = (ssize_t)vsnprintf(NULL, 0, fmt, ArgsCopy);

    va_end(ArgsCopy);

    return Length;
}

/* ---------- Private Library Functions ---------- */
