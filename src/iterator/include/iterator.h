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

#ifndef LIBCONTAINER_ITERATOR_H
#define LIBCONTAINER_ITERATOR_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

#include "../../../include/libcontainer.h"

/*
    Iterator_Type_t

    This enumeration defines the "type" of iterator, i.e. which container it
    applies to. This is to ensure that the iterator created for one container
    is only used on the container it was initialized for.
*/
typedef enum Iterator_Type_t {
    Iterator_UNKNOWN,
    Iterator_Array,
    Iterator_List,
    Iterator_Hashmap,
    Iterator_BinaryTree,
    Iterator_Stack,
    Iterator_String,
    /* ... */
} Iterator_Type_t;

/*
    Iterator_t

    This type represents an ability to iterate over a Container in a well-defined
    manner, as described by the *_Next() function for the given container.
    Generally, these iterators are used to range over a container to access each items
    in a more explicit and hands-on manner than what is offered by the CallbackFunc_t
    and CallbackArgFunc_t interfaces.

    Furthermore, an Iterator_t is resumable on a container as long as there are no
    insert or delete operations performed between the pause and resumption.

*/
typedef struct Iterator_t {

    /*
        Context is a pointer to a container-specific value which contains
        the necessary information required to safely and consistently iterate
        over the given container.
    */
    void* Context;

    /*
        ReleaseContext is a function
    */
    ReleaseFunc_t* ReleaseContext;

    /*
        Type defines the specific container type this iterator is initialized for.
    */
    Iterator_Type_t Type;
} Iterator_t;

/*
    Iterator_Invalidate

    This function takes an Iterator_t, and puts it into an invalid state
    such that any future calls or uses will either error out, or
    trigger a re-initialization of the Iterator.

    Inputs:
    Iterator    -   Pointer to the iterator to invalidate.

    Outputs:
    None, the iterator is invalidated and no longer safe for use.
*/
void Iterator_Invalidate(Iterator_t** Iterator);

#ifdef __cplusplus
}
#endif

#endif
