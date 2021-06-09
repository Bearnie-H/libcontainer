/*
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

#ifndef LIBCONTAINER_ERRORS_H
#define LIBCONTAINER_ERRORS_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as
   appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#if defined(LIBCONTAINER_ZERO_ON_RELEASE)
#include <string.h>

#define ZERO_CONTAINER(_name, _type) memset(_name, 0, sizeof(_type))

#else

#define ZERO_CONTAINER(_name, _type) ((void)0)

#endif

#if defined(DEBUG)
#include <stdio.h>

/*
    DEBUG_PRINTF

    This macro expands to allow printing helpful messages for debugging. This
   will only be included in DEBUG enabled builds.

    This is a macro to simplify printing out the file, line, and function the
   message comes from.
*/
#define DEBUG_PRINTF(_fmt, ...)                                                \
    fprintf(stderr, "(DEBUG) [%s:%d:%s()]: " _fmt "\n", __FILE__, __LINE__,    \
            __func__, __VA_ARGS__)

/*
    TEST_PRINTF

    Similar macro to DEBUG_PRINTF, but for testing messages.
*/
#define TEST_PRINTF(_fmt, ...)                                                 \
    fprintf(stderr, "(TESTING) [%s:%d:%s()]: " _fmt "\n", __FILE__, __LINE__,  \
            __func__, __VA_ARGS__)

#define TEST_NOT_IMPLEMENTED TEST_PRINTF("%s", "Test Failure - Test not yet implemented!"); return 1

#define TEST_SUCCESSFUL TEST_PRINTF("%s", "Test Successful."); return 0

#define TEST_FAILURE return 1

#else

#define DEBUG_PRINTF(_fmt, ...) ((void)0)
#define TEST_PRINTF(_fmt, ...) ((void)0)
#define TEST_NOT_IMPLEMENTED ((void)0)
#define TEST_SUCCESSFUL ((void)0)
#define TEST_FAILURE ((void)0)

#endif

#if defined(TESTING)

#undef DEBUG_PRINTF
#undef TEST_PRINTF
#undef TEST_NOT_IMPLEMENTED
#undef TEST_SUCCESSFUL
#undef TEST_FAILURE

#define DEBUG_PRINTF(_fmt, ...) ((void)0)

#define TEST_PRINTF(_fmt, ...)                                                 \
    fprintf(stderr, "(TESTING) [%s:%d:%s()]: " _fmt "\n", __FILE__, __LINE__,  \
            __func__, __VA_ARGS__)

#define TEST_NOT_IMPLEMENTED TEST_PRINTF("%s", "Test Failure - Test not yet implemented!"); return 1

#define TEST_SUCCESSFUL TEST_PRINTF("%s", "Test Successful."); return 0

#define TEST_FAILURE return 1

#endif

#ifdef __cplusplus
}
#endif

#endif
