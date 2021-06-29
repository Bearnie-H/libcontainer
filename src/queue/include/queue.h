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

#ifndef LIBCONTAINER_QUEUE_H
#define LIBCONTAINER_QUEUE_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#define LIBCONTAINER_ENABLE_QUEUE
#define LIBCONTAINER_ENABLE_LIST
#include "../../../include/libcontainer.h"

struct Queue_t {

    /*
        Items is the List_t containing the actual items within the Queue.
        This allows for O(1) Push() and Pop() operations.
    */
    List_t* Items;

    /*
        ReleaseFunc is the cached function pointer to the function to call to release
        the memory associated with the held items.
    */
    ReleaseFunc_t* ReleaseFunc;

    /*
        ItemSize is the cached size of each item held by the Queue.
    */
    size_t ItemSize;
};

#if defined(TESTING) || defined(DEBUGGER)

#include "queue_test.h"

#endif

#ifdef __cplusplus
}
#endif

#endif
