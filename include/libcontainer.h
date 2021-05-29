/*
    libcontainer - A library for creating and working with generic containers in pure C.

    This library contains a range of generic, useful "container" implementations
    to help solve the problem of rebuilding common data structures, and to
    provide a consistent and memory-safe basis for building upon. These containers
    are designed to provide some of the benefits and power of the C++ standard library
    container templates, but in a pure C environment.
*/

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

#ifndef LIBCONTAINER_H
#define LIBCONTAINER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

/* ++++++++++ Exported Library Macros ++++++++++ */

/* ... */

/* ---------- Exported Library Macros ---------- */

/* ++++++++++ Exported Library Globals ++++++++++ */

/* ... */

/* ---------- Exported Library Globals ---------- */

/* ++++++++++ Exported Library Types ++++++++++ */

#ifdef LIBCONTAINER_ENABLE_ARRAY
/*
    Array_t

    An Array_t is a dynamically resizeable array (or vector),
    capable of holding arbitrary homogeneous elements. These
    elements may be either directly held by the array itself,
    or references where the array only holds a pointer to some
    existing element. See Array_Create() vs. Array_RefCreate()
    for an explanation of the difference.

    This struct is opaque to ensure all accesses are performed
    through the functions provided in this library to ensure
    safe access and operation.

    See the functions prefixed with "Array_" for the available operations
    on these arrays.
*/
typedef struct Array_t Array_t;

#endif

#ifdef LIBCONTAINER_ENABLE_LIST

/*
    List_t

    A List_t is a doubly-linked list, capable of holding arbitrary elements.
    The list does not need to be homogeneous, and using the list does not
    depend on this. This list can either own the memory to hold the items
    it contains, or simply hold pointers to memory owned by an existing
    object.

    This struct is opaque to ensure all accesses are performed
    through the functions provided in this library to ensure
    safe access and operation.

    See the functions prefixed with "List_" for the available operations
    on these lists.
*/
typedef struct List_t List_t;

#endif

/*
    ReleaseFunc_t

    This is the generic form of a "free()" function used by this library. If any
    of the containers used don't own the memory associated by the items they hold,
    this memory still needs to be released when the container is released.
    Each homogeneous container, or each element of a heterogeneous container
    therefore needs to contain a function pointer to the function which can be
    used to safely release its memory.

    Note:
    If NULL is given, the library will default to the free() function in libstd.h
    if the context implies this is a meaningful default.
*/
typedef void(*ReleaseFunc_t)(void*);

/* ... */

/* ---------- Exported Library Types ---------- */

/* ++++++++++ Exported Library Functions ++++++++++ */

#ifdef LIBCONTAINER_ENABLE_ARRAY
/* +++++ Array Functions +++++ */
/*
    Array_Create

    This function will create and initialize an array with a specified element size
    and at a capacity at least large enough to hold "StartingCapacity" worth of elements.
    If StartingCapacity is 0, a reasonable default value is used
    (See the LIBCONTAINER_ARRAY_DEFAULT_CAPACITY macro). Once created,
    the size of each element to be added to a given Array_t must be equal to what
    it was initialized to hold.

    Inputs:
    StartingCapacity    -   The desired initial capacity. The array will be initialized to
   hold at least this many elements.

    ElementSize         -   The size (in bytes) of a single element to be held.
   Good practice is to have this be of the form: sizeof(<type>).

    Outputs:
    Array_t*    -   A properly initialized Array_t*, ready to be operated on by the rest
   of this library on success, or NULL on failure.

    Note:
    Array_Create() will own the memory of the objects it holds. This is suitable for
    primitive types, as well as "small" structs acting as simple collections of data.
    For working with arrays of values which may hold their own memory or resources,
    use the Array_RefCreate() function to allow safe management of these resources.
*/
Array_t* Array_Create(size_t StartingCapacity, size_t ElementSize);

/*
    Array_RefCreate

    This function will create and initialize an array with a specified element size
    and at a capacity at least large enough to hold "StartingCapacity" worth of elements.
    If StartingCapacity is 0, a reasonable default value is used
    (See the LIBCONTAINER_ARRAY_DEFAULT_CAPACITY macro). Once created,
    the size of each element to be added to a given Array_t must be equal to what
    it was initialized to hold.

    Inputs:
    StartingCapacity    -   The desired initial capacity. The array will be initialized to
   hold at least this many elements.

    ElementSize         -   The size (in bytes) of a single element to be held.
   Good practice is to have this be of the form: sizeof(<type>).

    ReleaseFunc         -   The pointer to the function to use to release the resources
   associated with a given element on either removal or array destruction. If NULL is
   provided, this defaults to "free()" from libstd.h

    Outputs:
    Array_t*    -   A properly initialized Array_t*, ready to be operated on by the rest
   of this library on success, or NULL on failure.

    Note:
    An array created with Array_RefCreate() simply holds the *pointers* to the items
    it holds, not the items themselves. This is more suitable for larger, or more complex
    items. This will safely handle the deallocation of resources, even if nested,
    when an element is removed, or if the array is released as a whole. Each element
    must be able to be fully released by a single call of the given ReleaseFunc.
*/
Array_t* Array_RefCreate(size_t StartingCapacity, size_t ElementSize, ReleaseFunc_t ReleaseFunc);

/*
    Array_Duplicate

    This function will create a shallow duplicate of the given Array_t. If the Array holds
   reference types, the returned array will simply point to the same underlying objects as the
   original.

    Inputs:
    Source  -   Pointer to the Array_t to duplicate.

    Outputs:
    Array_t*    -   The resulting newly allocated array, ready to be used by this library on success,
   or NULL on failure.

    Note:
    For reference-type arrays, which only hold pointers to the items they hold, this cannot
   know how to duplicate these sub-items. If a reference-type object is duplicated, both
   arrays will point to the same objects in memory and altering one will be visible in both.
   This is allowed, to simplify giving read-only access of an array to something else,
   but must be done with caution to ensure consistency of the underlying objects.
   Removal of objects or release of the array are safe to perform, only operations
   on the items themselves must be done with caution.
*/
Array_t* Array_Duplicate(Array_t* Source);

/*
    Array_Release

    This function will fully and safely release the resources held by an Array_t,
    including those held by its elements. This safely respects whether the array
    holds reference-type elements or is a shallow-copy of another array.
    Once passed to this function, the Array_t* is invalid for further use.

    Inputs:
    Array   -   Pointer to the Array_t to release.

    Outputs:
    None. This function always fully releases the resources associated with the array,
    under the assumption that the ReleaseFunc provided during initialization is "safe"
    and "correct".
*/
void Array_Release(Array_t* Array);

/*
    Array_Length

    This function returns the number of items actually within a given Array_t.

    Inputs:
    Array   -   The array to determine the length of.

    Outputs:
    ssize_t -   The length of the array (0 inclusive) on success, or -1 on failure.

    Note:
    This operation is O(1) in the length of the array.
*/
ssize_t Array_Length(Array_t* Array);

/*
    Array_Prepend

    This function will insert a single element at the beginning of the Array_t,
    maintaining ordering for all other elements.

    Inputs:
    Array       -   The array to add the element to.
    Element     -   Pointer to the element to add to the array.

    Outputs:
    int -   Returns 0 on success, non-zero on failure.

    Note:
    An implicit assumption of this function is that it is able to read ElementSize bytes
    from the Element pointer.
*/
int Array_Prepend(Array_t* Array, const void* Element);

/*
    Array_Append

    This function will insert a single element at the end of the Array_t.

    Inputs:
    Array       -   The array to add the element to.
    Element     -   Pointer to the element to add to the array.

    Outputs:
    int -   Returns 0 on success, non-zero on failure.

    Note:
    An implicit assumption of this function is that it is able to read ElementSize bytes
    from the Element pointer.
*/
int Array_Append(Array_t* Array, const void* Element);

/*
    Array_Insert

    This function will insert a single element at the given index of the Array_t.

    Inputs:
    Array       -   The array to add the element to.
    Element     -   Pointer to the element to add to the array.
    Index       -   The 0-based index for where in the array to place the new element.

    Outputs:
    int -   Returns 0 on success, non-zero on failure.

    Note:
    An implicit assumption of this function is that it is able to read ElementSize bytes
    from the Element pointer.
*/
int Array_Insert(Array_t* Array, const void* Element, int Index);

/*
    Array_InsertN

    This function will insert *Count* elements sequentially, starting at the given index of the Array_t.

    Inputs:
    Array       -   The array to add the element to.
    Element     -   Pointer to the element to add to the array.
    Index       -   The 0-based index for where in the array to place the new element.
    Count       -   The number of elements to add to the array.

    Outputs:
    int -   Returns 0 on success, non-zero on failure.

    Note:
    An implicit assumption of this function is that it is able to read (Count * ElementSize) bytes
    from the Element pointer.

    Additionally, this will place the elements into the array in the same order as in
    the Elements pointer.
*/
int Array_InsertN(Array_t* Array, const void* Elements, int Index, int Count);

/*
    Array_Remove

    This function will remove a single element at the given index from the Array_t.

    Inputs:
    Array       -   The array to remove the element from.
    Index       -   The 0-based index for which element to remove from the array.

    Outputs:
    int -   Returns 0 on success, non-zero on failure.
*/
int Array_Remove(Array_t* Array, int Index);

/*
    Array_RemoveN

    This function will remove *Count* elements, starting at the given index from the Array_t.

    Inputs:
    Array       -   The array to remove the element from.
    Index       -   The 0-based index for which element to remove from the array.
    Count       -   The number of elements to remove from the array.

    Outputs:
    int -   Returns 0 on success, non-zero on failure.
*/
int Array_RemoveN(Array_t* Array, int Index, int Count);

/*
    Array_GetElement

    This function will return a pointer to the element at Index within the Array.

    Inputs:
    Array   -   The array to retrieve the element from.
    Index   -   The 0-based index of the array to return the element at.

    Outputs:
    void*   -   For reference-type arrays, this is the pointer to the object itself at
    the specified address. For non-reference type arrays, this is a pointer within the
    array itself to the element requested. On failure, this returns NULL.
*/
void* Array_GetElement(Array_t* Array, int Index);

/*
    Array_SetElement

    This function will set the value of a the element at Index to the value in Element.

    Inputs:
    Array   -   Pointer to the array to operate on.
    Element -   Pointer to the value to set the element to.
    Index   -   The index of the element to update.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Array_SetElement(Array_t* Array, const void* Element, int Index);

/*
    Array_PopElement

    This function will return the requested element of the Array, removing
    it from the Array but not releasing the resources. Ownership of the given
    element is transferred to the caller.

    Inputs:
    Array   -   Pointer to the Array_t to pop the element from.
    Index   -   Index of the element within the array to pop.

    Outputs:
    void*   -   Untyped pointer to the requested element on success, or NULL on failure.

    Note:
    All resources owned by the element (if any) are transferred to the caller
    by using this function. Furthermore, correct casting of the return value is
    necessary for safe operation.
*/
void* Array_PopElement(Array_t* Array, int Index);
/* ----- Array Functions ----- */
#endif

#ifdef LIBCONTAINER_ENABLE_LIST
/* +++++ List Functions +++++ */
/*
    List_Create

    This function will create a new List_t, capable of holding arbitrary elements.
    This List_t can hold a heterogeneous collection of items, and can either own the
    resources held by the items, or simply reference objects which hold their own resources.
    For more information, see the List_Insert() and List_RefInsert() functions.

    Inputs:
    None, A default list is constructed and returned.

    Outputs:
    List_t* -   A pointer to the newly constructed List_t, ready to be used.
*/
List_t* List_Create(void);

/*
    List_Release

    This function will fully and safely release the List_t, as well as all
    of the contents of the list. For complete and safe release, the ReleaseFunc
    provided when adding items to the List_t must fully release the contents of
    the given item.

    Inputs:
    List    -   A pointer to the List_t to release.

    Outputs:
    None, the List_t is fully released and no further operations are valid on it.
*/
void List_Release(List_t* List);

/*
    List_Length

    This function returns the count of items in the list.

    Inputs:
    List    -   Pointer to the List_t to get the length of.

    Outputs:
    size_t  -   The count of items in the list.
*/
size_t List_Length(List_t* List);

/*
    List_Insert

    This function will insert a new item into the list at the desired location.
    This item is a non-reference type, and ownership of the item's resources are
    transferred to the List_t.

    Inputs:
    List            -   Pointer to the List_t to insert the item into.
    Element         -   Pointer to the item to add to the list.
    ElementSize     -   The size of the element to be added to the list, in bytes.
    Index           -   The 0-indexed location to add the item at in the List_t.

    Outputs:
    int -   Returns 0 on success, nonzero on failure.
*/
int List_Insert(List_t* List, const void* Element, size_t ElementSize, int Index);

/*
    List_RefInsert

    This function will insert a new item into the list at the desired location.
    This item is a reference type, and ownership of the item's resources are
    retained by the item, to be released by the given ReleaseFunc.

    Inputs:
    List            -   Pointer to the List_t to insert the item into.
    Element         -   Pointer to the item to add to the list.
    ReleaseFunc     -   The function to use to release the resources owned by the element if the item is to be disposed.
    Index           -   The 0-indexed location to add the item at in the List_t.

    Outputs:
    int -   Returns 0 on success, nonzero on failure.
*/
int List_RefInsert(List_t* List, const void* Element, ReleaseFunc_t ReleaseFunc, int Index);

/*
    List_Prepend

    This function is a shorthand for List_Insert(List, Element, ElementSize, 0).

    Inputs:
    List            -   Pointer to the List_t to insert the item into.
    Element         -   Pointer to the item to add to the list.
    ElementSize     -   The size of the element to be added to the list, in bytes.

    Outputs:
    int -   Returns 0 on success, nonzero on failure.
*/
int List_Prepend(List_t* List, const void* Element, size_t ElementSize);

/*
    List_RefPrepend

    This function is a shorthand for List_RefInsert(List, Element, ReleaseFunc, 0).

    Inputs:
    List            -   Pointer to the List_t to insert the item into.
    Element         -   Pointer to the item to add to the list.
    ReleaseFunc     -   The function to use to release the resources owned by the element if the item is to be disposed.
    Index           -   The 0-indexed location to add the item at in the List_t.

    Outputs:
    int -   Returns 0 on success, nonzero on failure.
*/
int List_RefPrepend(List_t* List, const void* Element, ReleaseFunc_t ReleaseFunc);

/*
    List_Append

    This function is a shorthand for List_Insert(List, Element, ElementSize, List->Length).

    Inputs:
    List            -   Pointer to the List_t to insert the item into.
    Element         -   Pointer to the item to add to the list.
    ElementSize     -   The size of the element to be added to the list, in bytes.

    Outputs:
    int -   Returns 0 on success, nonzero on failure.
*/
int List_Append(List_t* List, const void* Element, size_t ElementSize);

/*
    List_RefAppend

    This function is a shorthand for List_RefInsert(List, Element, ReleaseFunc, List->Length).

    Inputs:
    List            -   Pointer to the List_t to insert the item into.
    Element         -   Pointer to the item to add to the list.
    ReleaseFunc     -   The function to use to release the resources owned by the element if the item is to be disposed.
    Index           -   The 0-indexed location to add the item at in the List_t.

    Outputs:
    int -   Returns 0 on success, nonzero on failure.
*/
int List_RefAppend(List_t* List, const void* Element, ReleaseFunc_t ReleaseFunc);

/*
    List_Remove

    This function will remove the item at the specified index from the List_t, using
    the ReleaseFunc it was created with for Reference-types, or free() otherwise.
    This will remove the item and shrink the list.

    Inputs:
    List    -   Pointer to the List_t to operate on.
    Index   -   The 0-indexed location of the item to remove from the list.

    Outputs:
    int -   Returns 0 on success, nonzero on failure.
*/
int List_Remove(List_t* List, int Index);

/*
    List_GetElement

    This function returns a pointer to the contents of the item at the specified
    index into the List_t. The element stays within the list, and this simply gives
    access to the data.

    Inputs:
    List    -   Pointer to the List_t to operate on.
    Index   -   The 0-indexed location in the List_t of the element to retrieve.

    Outputs:
    void*   -   Untyped pointer to the contents at the specified index on success, NULL on failure.

    Note
    It is the responsibility of the caller to ensure the return value is cast to the correct
    type.
*/
void* List_GetElement(List_t* List, int Index);

/*
    List_SetElement

    This function allows overwriting the non-reference type item at the specified index
    of the List_t, updating the contents to a new value. The existing contents are released
    when the new contents are written.

    Inputs:
    List            -   Pointer to the List_t to operate on.
    Element         -   Pointer to the contents to update the item in the list to.
    ElementSize     -   The size of the contents, measured in bytes.
    Index           -   The 0-indexed position in the List_t of the item to update.

    Outputs:
    int -   Returns 0 on success, nonzero on failure.
*/
int List_SetElement(List_t* List, const void* Element, size_t ElementSize, int Index);

/*
    List_RefSetElement

    This function allows overwriting the reference type item at the specified index
    of the List_t, updating the contents to a new value. The existing contents are released
    when the new contents are written.

    Inputs:
    List            -   Pointer to the List_t to operate on.
    Element         -   Pointer to the contents to update the item in the list to.
    ReleaseFunc     -   The function which will safely release the new item being added.
    Index           -   The 0-indexed position in the List_t of the item to update.

    Outputs:
    int -   Returns 0 on success, nonzero on failure.
*/
int List_RefSetElement(List_t* List, const void* Element, ReleaseFunc_t ReleaseFunc, int Index);

/*
    List_PopElement

    This function will return the contents of the element at the specified index in
    the List_t, removing it from the List_t and transferring ownership to the caller.
    This is similar to List_GetElement, but the item is additionally removed from the list.

    Inputs:
    List    -   Pointer to the List_t to operate on.
    Index   -   The 0-indexed location of the List_t of the element to pop.

    Outputs:
    void*   -   Returns an untyped pointer to the requested item contents on success, or NULL on failure.
*/
void* List_PopElement(List_t* List, int Index);

/*
    List_PopFront

    This function is a shorthand for List_PopElement(List, 0).

    Inputs:
    List    -   Pointer to the List_t to operate on.

    Outputs:
    void*   -   Returns an untyped pointer to the requested item contents on success, or NULL on failure.
*/
void* List_PopFront(List_t* List);

/*
    List_PopBack

    This function is a shorthand for List_PopElement(List, List->Length).

    Inputs:
    List    -   Pointer to the List_t to operate on.

    Outputs:
    void*   -   Returns an untyped pointer to the requested item contents on success, or NULL on failure.
*/
void* List_PopBack(List_t* List);

/* ... */
/* ----- List Functions ----- */
#endif

/* ... */

/* ---------- Exported Library Functions ---------- */

#ifdef __cplusplus
}
#endif

#endif
