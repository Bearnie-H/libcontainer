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

#ifndef LIBCONTAINER_STRING_FMT_H
#define LIBCONTAINER_STRING_FMT_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

/*
    String_Createf

    This function creates and initializes a new String_t value based on the
    results of the given format string and arguments. Essentially, this function
    performs "sprintf()", but ensures safe allocation of the output buffer,
    and wraps it in a String_t.

    Inputs:
    fmt     -   The raw format C-String to use to generate the initialization value for the String_t.
    VarArgs -   The (optional) arguments as defined by the format string.

    Outputs:
    String_t*   -   Pointer to a fully prepared and ready-to-use String_t, or NULL on failure.

    Note:
    This function is helpful for creating complex strings, or for simply accessing a
    memory-safe sprintf() API. See sprintf() for API details for fmt and the var-args.
*/
String_t* String_Createf(const char* fmt, ...);

/*
    String_VCreatef

    This function creates and initializes a new String_t value based on the
    results of the given format string and arguments. Essentially, this function
    performs "sprintf()", but ensures safe allocation of the output buffer,
    and wraps it in a String_t.

    Inputs:
    fmt     -   The raw format C-String to use to generate the initialization value for the String_t.
    VarArgs -   The (optional) arguments as defined by the format string.
    ...

    Outputs:
    String_t*   -   Pointer to a fully prepared and ready-to-use String_t, or NULL on failure.

    Note:
    This function is helpful for creating complex strings, or for simply accessing a
    memory-safe sprintf() API. See sprintf() for API details for fmt and the var-args.
*/
String_t* String_VCreatef(const char* fmt, va_list VarArgs, va_list VarArgsCopy);

/*
    String_SPrintf

    This function acts similarly to String_Createf(), but operates on an existing
    String_t. This ensures safe allocation and writes the resulting formatted
    string to the String_t, overwriting any existing contents.

    Inputs:
    String  -   Pointer to the String_t to write the resulting formatted string to.
    fmt     -   The raw format C-String to use to generate the initialization value for the String_t.
    VarArgs -   The (optional) arguments as defined by the format string.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.

    Note:
    This function is helpful for creating complex strings, or for simply accessing a
    memory-safe sprintf() API. See sprintf() for API details for fmt and the var-args.
*/
int String_SPrintf(String_t* String, const char* fmt, ...);

/*
    String_VSPrintf

    This function provides the String_SPrintf() function, but exposes the va_list
    for better embedding in other variadic functions.

    Inputs:
    String  -   Pointer to the String_t to write the resulting formatted string to.
    fmt     -   The raw format C-String to use to generate the initialization value for the String_t.
    VarArgs -   The (optional) arguments as defined by the format string.
    ...

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.

    Note:
    This function is helpful for creating complex strings, or for simply accessing a
    memory-safe sprintf() API. See sprintf() for API details for fmt and the var-args.
*/
int String_VSPrintf(String_t* String, const char* fmt, va_list VarArgs, va_list VarArgsCopy);

/*
    String_Appendf

    This function acts similarly to String_SPrintf(), but rather than overwriting the
    existing String_t contents, will simply append the formatted string to the end
    of the String_t. This is helpful for building complex strings in multiple stages.

    Inputs:
    String  -   Pointer to the String_t to append the resulting formatted string to.
    fmt     -   The raw format C-String to use to generate the initialization value for the String_t.
    VarArgs -   The (optional) arguments as defined by the format string.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.

    Note:
    This function is helpful for creating complex strings, or for simply accessing a
    memory-safe sprintf() API. See sprintf() for API details for fmt and the var-args.
*/
int String_Appendf(String_t* String, const char* fmt, ...);

/*
    String_VAppendf

    This function acts similarly to String_SPrintf(), but rather than overwriting the
    existing String_t contents, will simply append the formatted string to the end
    of the String_t. This is helpful for building complex strings in multiple stages.

    Inputs:
    String  -   Pointer to the String_t to append the resulting formatted string to.
    fmt     -   The raw format C-String to use to generate the initialization value for the String_t.
    VarArgs -   The (optional) arguments as defined by the format string.
    ...

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.

    Note:
    This function is helpful for creating complex strings, or for simply accessing a
    memory-safe sprintf() API. See sprintf() for API details for fmt and the var-args.
*/
int String_VAppendf(String_t* String, const char* fmt, va_list VarArgs, va_list VarArgsCopy);

ssize_t String_getFMTLength(const char* fmt, va_list args);

#if defined(TESTING) || defined(DEBUGGER)

#include "string_fmt_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
