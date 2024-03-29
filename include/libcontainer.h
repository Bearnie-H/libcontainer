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

/*
    libcontainer - A library for creating and working with generic containers in pure C.

    This library contains a range of generic, useful "container" implementations
    to help solve the problem of rebuilding common data structures, and to
    provide a consistent and memory-safe basis for building upon. These containers
    are designed to provide some of the benefits and power of the C++ standard library
    container templates, but in a pure C environment.
*/

#ifndef LIBCONTAINER_H
#define LIBCONTAINER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <stdarg.h>
#include <stdbool.h>

/* ++++++++++ Cross-Container Macro Enabling ++++++++++ */
/*
    For some of the containers, the API they expose requires the inclusion of
    at least some other containers. This section at the beginning of this file
    ensures that these API requirements are satisfied, without the user
    needing to explicitly enable everything.
*/

#ifdef LIBCONTAINER_ENABLE_ALL
#define LIBCONTAINER_ENABLE_ARRAY
#define LIBCONTAINER_ENABLE_LIST
#define LIBCONTAINER_ENABLE_HASHMAP
#define LIBCONTAINER_ENABLE_BINARY_TREE
#define LIBCONTAINER_ENABLE_SET
#define LIBCONTAINER_ENABLE_STACK
#define LIBCONTAINER_ENABLE_QUEUE
#define LIBCONTAINER_ENABLE_STRING
#define LIBCONTAINER_ENABLE_BINARY_HEAP
#define LIBCONTAINER_ENABLE_PRIORITY_QUEUE
#endif

/*
    The Hashmap_t container requires the Array_t container for the
    Hashmap_Keys() and Hashmap_Values() functions.
*/
#ifdef LIBCONTAINER_ENABLE_HASHMAP
#define LIBCONTAINER_ENABLE_ARRAY
#endif

/* ---------- Cross-Container Macro Enabling ---------- */

/* ++++++++++ Exported Library Macros ++++++++++ */


#ifdef LIBCONTAINER_ENABLE_ARRAY

/*
    ARRAY_FOREACH

    This macro expands to a simple for-each loop over an Array_t container.
    This allows for simpler use of the iteration interface.

    Inputs:
    Array   -   The array to iterate over.
    Value   -   The local variable to hold each next item from the Array.
*/
#define ARRAY_FOREACH(Array, Value) for ((Value) = Array_Next(Array); NULL != (Value); (Value) = Array_Next(Array))
#endif

#ifdef LIBCONTAINER_ENABLE_LIST

/*
    LIST_FOREACH

    This macro expands to a simple for-each loop over a List_t container.
    This allows for simpler use of the iteration interface.

    Inputs:
    List    -   The List to iterate over.
    Value   -   The local variable to hold each next item from the List.
*/
#define LIST_FOREACH(List, Value) for ((Value) = List_Next(List); (NULL != (Value)); (Value) = List_Next(List))
#endif

#ifdef LIBCONTAINER_ENABLE_HASHMAP

/*
    HASHMAP_FOREACH

    This macro expands to a simple for-each loop over a Hashmap_t container.
    This allows for simpler use of the iteration interface.

    Inputs:
    Hashmap         -   The Hashmap to iterate over.
    KeyValuePair    -   The local variable to hold each next item from the Hashmap.
*/
#define HASHMAP_FOREACH(Hashmap, KeyValuePair) for ((KeyValuePair) = Hashmap_Next(Hashmap); ((NULL != (KeyValuePair).Key) && (NULL !=(KeyValuePair).Value)); (KeyValuePair) = Hashmap_Next(Hashmap))

#define HASHMAP_SORTED_FOREACH(Hashmap, KeyValuePair, CompareFunc) \
            for \
            ((KeyValuePair) = Hashmap_SortedNext((Hashmap), (CompareFunc));\
            ((NULL != (KeyValuePair).Key) && (NULL != (KeyValuePair).Value));\
            (KeyValuePair) = Hashmap_SortedNext((Hashmap), NULL))

#endif

#ifdef LIBCONTAINER_ENABLE_BINARY_TREE

/*
    BINARY_TREE_FOREACH

    This macro expands to a simple for-each loop over a List_t container.
    This allows for simpler use of the iteration interface.

    Inputs:
    Tree        -   The Binary Tree to iterate over.
    KeyValuePair-   The local variable to hold each next item from the List.
    Direction   -   The forward iteration direction to traverse the tree in.
*/
#define BINARY_TREE_FOREACH(List, KeyValuePair, Direction) for ((KeyValuePair) = Binary_Tree_Next((Tree), (Direction)); (NULL != (KeyValuePair).Key); (KeyValuePair) = Binary_Tree_Next((Tree), (Direction)))
#endif

#ifdef LIBCONTAINER_ENABLE_SET

/*
    SET_FOREACH

    This macro defines the idiomatic method to iterate over a Set_t container.
    This will initialize iteration, and return each "next" value to the body of the
    for loop. At the end of this FOREACH loop, the Value will be NULL, and the iterator
    will be reset.

    Inputs:
    Set     -   Pointer to the Set_t to iterate over.
    Value   -   Pointer with a type matching that of the items within the Set_t.
*/
#define SET_FOREACH(Set, Value) for ((Value) = Set_Next(Set); (NULL != (Value)); (Value) = Set_Next(Set))
#endif

#ifdef LIBCONTAINER_ENABLE_QUEUE

/*
    QUEUE_FOREACH

    This macro defines the idiomatic method to iterate over a Queue_t container.
    This will initialize iteration, and return each "next" value to the body of the
    for loop. At the end of this FOREACH loop, the Value will be NULL, and the iterator
    will be reset.

    Inputs:
    Queue   -   Pointer to the Queue_t to iterate over.
    Value   -   Pointer with a type matching that of the items within the Queue_t.
*/
#define QUEUE_FOREACH(Queue, Value) for ((Value) = Queue_Next(Queue); (NULL != Value); (Value) = Queue_Next(Queue))
#endif

#ifdef LIBCONTAINER_ENABLE_BINARY_HEAP

/*
    BINARY_HEAP_FOREACH

    This macro defines the idiomatic method to iterate over a Binary_Heap_t container.
    This will initialize iteration, and return each "next" value to the body of the
    for loop. At the end of this FOREACH loop, the Value will be NULL, and the iterator
    will be reset.

    Inputs:
    Queue   -   Pointer to the Binary_Heap_t to iterate over.
    Value   -   Binary_Heap_KeyValuePair_t type.
*/
#define BINARY_HEAP_FOREACH(Heap, KeyValuePair) for ((KeyValuePair) = Binary_Heap_Next(Heap); (NULL != (KeyValuePair).Key); (KeyValuePair) = Binary_Heap_Next(Heap))
#endif

#ifdef LIBCONTAINER_ENABLE_PRIORITY_QUEUE

/*
    PRIORITY_QUEUE_FOREACH

    This macro defines the idiomatic method to iterate over a Priority_Queue_t container.
    This will initialize iteration, and return each "next" value to the body of the
    for loop. At the end of this FOREACH loop, the Value will be NULL, and the iterator
    will be reset.

    Inputs:
    Queue   -   Pointer to the Binary_Heap_t to iterate over.
    Value   -   Priority_Queue_Item_t type.
*/
#define PRIORITY_QUEUE_FOREACH(Queue, Item) for ((Item) = Priority_Queue_Next(Queue); (NULL != (Item).Value); (Item) = Priority_Queue_Next(Queue))
#endif

/* ---------- Exported Library Macros ---------- */

/* ++++++++++ Exported Library Globals ++++++++++ */

/* ---------- Exported Library Globals ---------- */

/* ++++++++++ Exported Library Types ++++++++++ */

#ifdef LIBCONTAINER_ENABLE_ARRAY
/* ++++++++++ Public Array_t Typedefs ++++++++++ */

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

/* ---------- Public Array_t Typedefs ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_LIST
/* ++++++++++ Public List_t Typedefs ++++++++++ */

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

/* ---------- Public List_t Typedefs ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_HASHMAP
/* ++++++++++ Public Hashmap_t Typedefs ++++++++++ */

/*
    Hashmap_t

    A Hashmap_t is a generic key-value map. A given Hashmap_t must use homogeneous
    keys, but can contain heterogeneous values associated with them. See the documentation
    about, and examples of, the HashFunc_t typedef for more detail about the Hash function
    used in this container.

    This struct is opaque to ensure all accesses are performed
    through the functions provided in this library to ensure
    safe access and operation.

    See the functions prefixed with "Hashmap_" for the available operations
    on Hashmaps.
*/
typedef struct Hashmap_t Hashmap_t;

/*
    HashFunc_t

    This represents the class of functions the Hashmap_t can use to compute
    the necessary internal hash value to map a key to a value within the Hashmap.

    This takes in a pointer to the key, as well as an *optional* size_t value.
    If the key is something like a string, array, or other non-constant sized
    item, this size value allows safe access to the key through the pointer.
    This size_t value should correspond to the number of bytes (specifically
    the number of *sizeof(char)*) which the item is composed of. If the key is
    a constant sized type, like int or double, this parameter can be ignored.

    By default, this library provides useable HashFunc_t implementations for the
    following key types:

        -   int
        -   long
        -   double
        -   string (char*) ( optionally NULL terminated, but not necessary if size_t is provided. )

    To extend the Hashmap_t to new keys, simply provide a HashFunc_t implementation for
    the desired type, as well as a meaningful ReleaseFunc_t (if necessary), and use those
    during Hashmap_t construction.

    Inputs:
    const void* Key -   Untyped pointer to the Key value, to be read to compute the Hash value.
    size_t KeySize  -   The number of bytes available to be read from Key, if it's not a direct cast.

    Outputs:
    unsigned int    -   The resulting Hash Value, prior to being reduced by the Hashmap_t to
                            the specific index value required to locate an item.
*/
typedef unsigned int (HashFunc_t)(const void*, size_t);

/*
    Hashmap_KeyValuePair_t

    This struct represents a general Key-Value pair from a Hashmap value entry.
    It is the responsibility of the caller to ensure safe typing of the returned values.

    This type is generally only used with the Iterator-family functions, such
    as Hashmap_Next() or Hashmap_Previous() or their sorted cousins. Additionally,
    this is the "Value" passed in during the Callback() API. See the documentation
    for those particular functions for more information about how this struct is used.
*/
typedef struct Hashmap_KeyValuePair_t {

    /*
        Pointer to the Key value of a Hashmap Entry.
    */
    void* Key;

    /*
        Pointer to the Value of a Hashmap Entry.
    */
    void* Value;
} Hashmap_KeyValuePair_t;

/* ---------- Public Hashmap_t Typedefs ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_BINARY_TREE
/* +++++++++ Public Binary_Tree_t Typedefs +++++++++ */

/*
    Binary_Tree_t

    A Binary_Tree_t is a self-balancing binary search tree structure. This
    can hold arbitrary homogeneous elements. This can either own the memory
    associated with the items it contains, or simply contain references
    to resources held by the items themselves.

    Duplicate items are not allowed, and insertion of a duplicate item
    will overwrite the existing contents. If this is problematic, always
    check if the key exists before performing an insert operation.
    Note, This API may change in the future to allow choosing a policy for
    handling duplicate items.

    This struct is opaque to ensure all accesses are performed
    through the functions provided in this library to ensure
    safe access and operation.

    See the functions prefixed with "Binary_Tree_" for the available operations
    on this container.
*/
typedef struct Binary_Tree_t Binary_Tree_t;

/*
    Binary_Tree_Direction_t

    This enum defines the different directions in which a Binary_Tree_t can be
    traversed. One of these directions is required for the single-step iteration
    over the Tree as provided by the "next" interface.
*/
typedef enum Binary_Tree_Direction_t {
    Direction_InOrder,
    Direction_PreOrder,
    Direction_PostOrder
} Binary_Tree_Direction_t;

/*
    Binary_Tree_DuplicatePolicy_t

    This enum defines the behaviour a binary tree will take when
    trying to insert a Key which already exists in the tree.
*/
typedef enum Binary_Tree_DuplicatePolicy_t {

    /*
        Policy_Overwrite - Overwrite the existing item with the newly added value,
            releasing the existing item.
    */
    Policy_Overwrite,

    /*
        Policy_Error    -   Return with an error code if the entry already exists
            in the Tree.
    */
    Policy_Error

} Binary_Tree_DuplicatePolicy_t;

/*
    Binary_Tree_KeyValuePair_t

    This type represents the basic container to simplify working with
    the Key-Value data-types used in the Binary_Tree_t container.

    Note that the pointers here are a reference to the actual contents
    within the tree. Modifications made here are reflected in future
    operations on the Tree.
*/
typedef struct Binary_Tree_KeyValuePair_t {

    /*
        Pointer to key associated with an item within the Binary Tree.
    */
    void* Key;

    /*
        Pointer to the actual item Value within the Binary Tree.
    */
    void* Value;

} Binary_Tree_KeyValuePair_t;

/* --------- Public Binary_Tree_t Typedefs --------- */
#endif

#ifdef LIBCONTAINER_ENABLE_BINARY_HEAP
/* ++++++++++ Public Binary_Heap_t Typedefs ++++++++++ */

typedef struct Binary_Heap_KeyValuePair_t {

    /*
        Key is a pointer to the Key associated with a Binary Heap Key-Value pair.
        This is a pointer to the item within the Binary_Heap_t itself, so modifications
        to this will be reflected in the tree itself.
    */
    void* Key;

    /*
        Value is a pointer to the Value associated with a Binary Heap Key-Value pair.
        This is a pointer to the item within the Binary_Heap_t itself, so modifications
        to this will be reflected in the tree itself.
    */
    void* Value;

} Binary_Heap_KeyValuePair_t;

/*
    Binary_Heap_t

    This container implements a generic Binary Heap. Whether this is a
    Min-Heap or a Max-Heap depends on the CompareFunc_t it is initialized with.

    Duplicate Keys and Values are allowed, and can be of arbitrary type.
    Within a single Binary_Heap_t, the "type" of all Keys must be homogeneous,
    and all Values must also be homogeneous. As is expected from a Heap,
    this provides an iterator, but this iterator is unordered.

    This struct is opaque to ensure all accesses are performed
    through the functions provided in this library to ensure
    safe access and operation.

    See the functions prefixed with "Binary_Heap_" for the available operations
    on these lists.
*/
typedef struct Binary_Heap_t Binary_Heap_t;

/* ---------- Public Binary_Heap_t Typedefs ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_SET
/* ++++++++++ Public Set_t Typedefs ++++++++++ */

/*
    Set_t

    A Set_t is a generic container which contains unique items.
    This acts similarly to the C++ std::ordered_set, and provides
    an API for adding, removing, and iterating over the collection
    of ordered, unique items.

    All items within a single Set_t must be homogeneous in type,
    but not necessarily in size. For example, a Set_t can contain
    char* strings of differing size, or some constant-size type
    like a struct or integral type.

    This struct is opaque to ensure all accesses are performed
    through the functions provided in this library to ensure
    safe access and operation.

    See the functions prefixed with "Set_" for the available operations
    on this container.
*/
typedef struct Set_t Set_t;

/* ---------- Public Set_t Typedefs ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_STACK
/* ++++++++++ Public Stack_t Typedefs ++++++++++ */

/*
    Stack_t

    This container provides a Last-In First-Out (LIFO) interface for arbitrary,
    homogeneous items. This container can either own the memory associated with
    the contents it holds, or it can simply hold references to items which manage
    their own resources.

    This interface provides the standard Push/Peek/Pop expected for a Stack,
    as well as the additional DoCallback() and DoCallbackArg() interface
    provided by other containers in this library for performing an action
    with each of the items in the container.

    This struct is opaque to ensure all accesses are performed
    through the functions provided in this library to ensure
    safe access and operation.

    See the functions prefixed with "Stack_" for the available operations
    on this container.
*/
typedef struct Stack_t Stack_t;

/* ---------- Public Stack_t Typedefs ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_QUEUE
/* ++++++++++ Public Queue_t Typedefs ++++++++++ */

/*
    Queue_t

    This container provides a First-In First-Out (FIFO) interface for arbitrary,
    homogeneous items. This container can either own the memory associated with
    the contents it holds, or it can simply hold references to items which manage
    their own resources.

    This interface provides the standard Push/Peek/Pop expected for a Queue,
    as well as the additional DoCallback() and DoCallbackArg() interface
    provided by other containers in this library for performing an action
    with each of the items in the container.

    This struct is opaque to ensure all accesses are performed
    through the functions provided in this library to ensure
    safe access and operation.

    See the functions prefixed with "Queue_" for the available operations
    on this container.
*/
typedef struct Queue_t Queue_t;

/* ---------- Public Queue_t Typedefs ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_PRIORITY_QUEUE
/* ++++++++++ Public Priority_Queue_t Typedefs ++++++++++ */

/*
    Priority_Queue_Item_t

    This type is a simple record type holding the priority and value from
    an item pushed into a Priority_Queue_t. This is used to allow returning
    an item and it's priority back from the queue.
*/
typedef struct Priority_Queue_Item_t {

    /*
        Priority holds the priority value which the Value was registered
        into the Queue with.
    */
    int Priority;

    /*
        Value is the raw value as was registered into the Queue.
    */
    void* Value;

} Priority_Queue_Item_t;

/*
    Priority_Queue_t

    This container provides a Priority-Ordered interface for arbitrary,
    homogeneous items. This container can either own the memory associated with
    the contents it holds, or it can simply hold references to items which manage
    their own resources.

    This interface provides the standard Push/Peek/Pop expected for a Queue,
    as well as the additional DoCallback() and DoCallbackArg() interface
    provided by other containers in this library for performing an action
    with each of the items in the container.

    This is a distinct container from the standard Queue. That container
    provides strict FIFO ordering, while this container will potentially
    re-order items to ensure the highest (or lowest if specified) priority
    item is returned first.

    This struct is opaque to ensure all accesses are performed
    through the functions provided in this library to ensure
    safe access and operation.

    See the functions prefixed with "Priority_Queue_" for the available operations
    on this container.
*/
typedef struct Priority_Queue_t Priority_Queue_t;

/* ---------- Public Priority_Queue_t Typedefs ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_STRING
/* ++++++++++ Public String_t Typedefs ++++++++++ */

/*
    String_t

    This container provides a richer String-type than the default NULL-terminated
    strings of standard C. This provides an interface for building, manipulating,
    reading, and writing Strings in a memory-safe manner.

    This struct is opaque to ensure all accesses are performed
    through the functions provided in this library to ensure
    safe access and operation.

    See the functions prefixed with "String_" for the available operations
    on this container.
*/
typedef struct String_t String_t;

/* ---------- Public String_t Typedefs ---------- */
#endif

/* ++++++++++ General Public Library Typedefs ++++++++++ */

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
typedef void(ReleaseFunc_t)(void*);

/*
    CallbackFunc_t

    This is the generic form of a callback function used by this library.
    Callbacks within this library are meant as something which can operate
    on the value in some item of any of the containers. This specific form
    of Callback is generally useful for printing, diagnostics, or other
    operations which only need the value itself. For more complex callbacks,
    see the CallbackArgFunc_t type.

    Inputs:
    Value   -   Untyped pointer to the "value" of the item in the container.
                    See the specific *_DoCallback() function for a container
                    for specifics about what exactly this points to.

    Outputs:
    int -   Must return 0 on success, non-zero on failure.

    Note:
    The library will assert that Value is non-NULL.
*/
typedef int(CallbackFunc_t)(void*);

/*
    CallbackArgFunc_t

    This is the generic form of a callback function which also takes
    "external" arguments in this library. These external arguments
    allow passing additional data each of the values in a given container,
    providing more flexibility than what is provided by CallbackFunc_t.
    These additional arguments must be formatted analogous to how they
    would be in calls to something like pthread_create().

    Inputs:
    Value   -   Untyped pointer to the value of the item in the container.
                    See the specific *_DoCallbackArg() function for a container
                    for specifics about what exactly this points to.
    Args    -   Untyped pointer to some memory to pass in addition to the
                    item value.
    Outputs:
    int -   Must return 0 on success, non-zero on failure.

    Note:
    The library will assert that Value is non-NULL.
    Args may or may not be NULL; this is left entirely up to the caller.
*/
typedef int(CallbackArgFunc_t)(void*, void*);

/*
    CompareFunc_t

    This is the generic form of a comparison function used by this library.
    This is equivalent to the function pointer type required by the qsort()
    function in stdlib.h, only explicitly typed for documentation and argument
    type-matching purposes.

    Inputs:
    void*   -   Pointer to the first item to be compared.
    void*   -   Pointer to the second item to be compared.
    size_t  -   Size of the items to compare

    Outputs:
    int     -   Must return negative if First < Second,
                Must return 0 if First == Second
                Must return positive if First > Second
                (For "ascending" order. Reverse this to define a
                "descending" order.)

    Note:
    The library will assert that NULL pointers are NEVER passed to this
    function type. If, however, the "Value" held by a container is a
    pointer-to-pointer type, this inner pointer MAY be NULL, and you must
    check for this. See the documentation for memcmp() for an example
    function which satisfies these requirements.

    The size of the values is not provided, as this is known by the container,
    and should be known by the author of such CompareFunc_t implementations.
*/
typedef int(CompareFunc_t)(const void*, const void*, size_t);

/* ---------- General Public Library Typedefs ---------- */

/* ---------- Exported Library Types ---------- */

/* ++++++++++ Exported Library Functions ++++++++++ */

/* +++++ General Library Version Functions +++++ */

/*
    Print_Libcontainer_Library_Version

    This function prints the library version documentation string to STDOUT.
    This prints out the version information, in MAJOR.MINOR.PATCH format,
    as well as the date and time at which the library was compiled.

    Inputs:
    None

    Outputs:
    None, the information is printed to stdout.

    Note:
    The internal version information used by this function is set during the library
    build process. This function is primarily intended as a human-readable method
    to understand when the library was made, and what version it's on.
*/
void Print_Libcontainer_Library_Version(void);

/*
    Libcontainer_Library_Version

    This function returns the Version-Code of the compiled library.
    This Version-Code is on AA.BB.CC format, where:
        AA  -   Major Version
        BB  -   Minor Version
        CC  -   Patch Version

    Inputs:
    None

    Outputs:
    long    -   AABBCC Version Code of the compiled library.

    Note:
    The internal version information used by this function is set during the library
    build process.
*/
long Libcontainer_Library_Version(void);

/*
    Libcontainer_Build_Time

    This function returns as a long the YYYYMMDD date at which the
    library was compiled.

    Inputs:
    None

    Outputs:
    long    -   YYYYMMDD datestamp at which the library was compiled.

    Note:
    The internal time information used by this function is set during the library
    build process.
*/
long Libcontainer_Build_Time(void);

/* ----- General Library Version Functions ----- */

/* ++++++++++ Default CompareFunc Implementations ++++++++++ */

/*
    CompareFunc_Int

    This function is a suitable comparison function for "int" types where this library
    uses a CompareFunc_t. This function is guaranteed to work in the expected ordering
    for all containers and uses which rely on a CompareFunc_t.

    Inputs:
    A       -   Pointer to the first item to compare.
    B       -   Pointer to the second item to compare.
    Size    -   (unused) The size of each item, in bytes.

    Outputs:
    int     -   Returns postive if A > B, 0 if A == B, and negative if A < B.

    Note:
    A suitable comparison function must follow this return interpretation to be useful.
    See the implementations for these functions, as well as the documentation for strncmp()
    and memcmp() for additional information about these kinds of comparison functions.
*/
CompareFunc_t CompareFunc_Int_Ascending;

/*
    CompareFunc_Int_Descending

    This function is equivalent to CompareFunc_Int_Ascending, except that
    it implies the reverse ordering.

    Inputs:
    A       -   Pointer to the first item to compare.
    B       -   Pointer to the second item to compare.
    Size    -   (unused) The size of each item, in bytes.

    Outputs:
    int     -   Returns negative if A > B, 0 if A == B, and positive if A < B.

    Note:
    A suitable comparison function must follow this return interpretation to be useful.
    See the implementations for these functions, as well as the documentation for strncmp()
    and memcmp() for additional information about these kinds of comparison functions.
*/
CompareFunc_t CompareFunc_Int_Descending;

/*
    CompareFunc_String

    This function is a suitable comparison function for "char*" types where this library
    uses a CompareFunc_t. This function is guaranteed to work in the expected ordering
    for all containers and uses which rely on a CompareFunc_t. If the Size parameter
    is non-zero, this can be used for non NUL-terminated strings. If a Size of 0 is
    given, this will use strlen() to compute the size of the strings, and this may not
    be valid for non NUL-terminated strings.

    Inputs:
    A       -   Pointer to the first item to compare.
    B       -   Pointer to the second item to compare.
    Size    -   The size of the strings, or at least the smaller string.

    Outputs:
    int     -   Returns postive if A > B, 0 if A == B, and negative if A < B.

    Note:
    A suitable comparison function must follow this return interpretation to be useful.
    See the implementations for these functions, as well as the documentation for strncmp()
    and memcmp() for additional information about these kinds of comparison functions.
*/
CompareFunc_t CompareFunc_String_Ascending;

/*
    CompareFunc_String_Descending

    This function is equivalent to CompareFunc_String_Ascending, except that
    it implies the reverse ordering.

    Inputs:
    A       -   Pointer to the first item to compare.
    B       -   Pointer to the second item to compare.
    Size    -   The size of the strings, or at least the smaller string.

    Outputs:
    int     -   Returns negative if A > B, 0 if A == B, and positive if A < B.

    Note:
    A suitable comparison function must follow this return interpretation to be useful.
    See the implementations for these functions, as well as the documentation for strncmp()
    and memcmp() for additional information about these kinds of comparison functions.
*/
CompareFunc_t CompareFunc_String_Descending;

/* ---------- Default CompareFunc Implementations ---------- */

#ifdef LIBCONTAINER_ENABLE_ARRAY
/* ++++++++++ Public Array_t Functions ++++++++++ */

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
Array_t* Array_RefCreate(size_t StartingCapacity, ReleaseFunc_t* ReleaseFunc);

/*
    Array_Clear

    This function will remove all the items from the Array, without
    releasing the full array.

    Inputs:
    Array   -   Pointer to the Array_t to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Array_Clear(Array_t* Array);

/*
    Array_Length

    This function returns the number of items actually within a given Array_t.

    Inputs:
    Array   -   The array to determine the length of.

    Outputs:
    size_t -   The length of the array (0 inclusive) on success, 0 on failure.

    Note:
    This operation is O(1) in the length of the array.
*/
size_t Array_Length(Array_t* Array);

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
int Array_Prepend(Array_t* Array, void* Element);

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
int Array_Append(Array_t* Array, void* Element);

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
int Array_Insert(Array_t* Array, void* Element, size_t Index);

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
int Array_InsertN(Array_t* Array, void* Elements, size_t Index, size_t Count);

/*
    Array_Remove

    This function will remove a single element at the given index from the Array_t.

    Inputs:
    Array       -   The array to remove the element from.
    Index       -   The 0-based index for which element to remove from the array.

    Outputs:
    int -   Returns 0 on success, non-zero on failure.
*/
int Array_Remove(Array_t* Array, size_t Index);

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
int Array_RemoveN(Array_t* Array, size_t Index, size_t Count);

/*
    Array_Replace

    This function replaces the contents at the given index with the new value,
    safely releasing the existing contents.

    Inputs:
    Array   -   Pointer to the Array_t to operate on.
    Element -   Pointer to the new contents to insert into the Array.
    Index   -   The index to start the replacement.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Array_Replace(Array_t* Array, void* Element, size_t Index);

/*
    Array_ReplaceN

    This function replaces the contents starting at the given index with the new value,
    safely releasing the existing contents.

    Inputs:
    Array       -   Pointer to the Array_t to operate on.
    Elements    -   Pointer to the new contents to insert into the Array.
    Index       -   The index to start the replacement.
    Count       -   The number of items pointed to by "Elements"

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Array_ReplaceN(Array_t* Array, void* Elements, size_t Index, size_t Count);

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
void* Array_GetElement(Array_t* Array, size_t Index);

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
int Array_SetElement(Array_t* Array, void* Element, size_t Index);

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
void* Array_PopElement(Array_t* Array, size_t Index);

/*
    Array_Next

    This function allows iteration over an Array_t in a single-step, resumable
    manner. This provides forward iteration over the array, returning each
    successive element to the caller.

    Inputs:
    Array   -   Pointer to the Array to operate on.

    Outputs:
    void*   -   Pointer to the "next" element of the Array.

    Note:
    Note that iteration will re-start if and when any Insert(), Remove(),
    Sort(), or Pop() operations are performed on the Array. Any other operation can
    be done without interrupting and restarting iteration.
*/
void* Array_Next(Array_t *Array);

/*
    Array_Previous

    This function allows iteration over an Array_t in a single-step, resumable
    manner. This provides reverse iteration over the array, returning each
    successive element to the caller.

    Inputs:
    Array   -   Pointer to the Array to operate on.

    Outputs:
    void*   -   Pointer to the "previous" element of the Array.

    Note:
    Note that iteration will re-start if and when any Insert(), Remove(),
    Sort(), or Pop() operations are performed on the Array. Any other operation can
    be done without interrupting and restarting iteration.
*/
void* Array_Previous(Array_t *Array);

/*
    Array_DoCallback

    This function will perform the given callback function on each of the elements of
    the array in sequence. This is helpful for things like printing or otherwise manipulating
    the array value.

    Inputs:
    Array       -   Pointer to the array to operate on.
    Callback    -   Pointer to the Callback function to call for each item of the array.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Array_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" pointer provided to the Callback is a pointer to each element
    of the array.
*/
int Array_DoCallback(Array_t* Array, CallbackFunc_t* Callback);

/*
    Array_DoCallbackArg

    This function will perform the given callback function on each of the elements
    of the array in sequence. This extends the Array_DoCallback function by allowing
    additional parameters to be passed in to this callback function to allow for
    richer and more complex behaviour.

    Inputs:
    Array       -   Pointer to the array to operate on.
    Callback    -   Pointer to the Callback function to call for each item of the array.
    Args        -   Pointer to some additional argument(s), values, resources, etc. to pass
                        in to the Callback function along with the item value.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Array_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" pointer provided to the Callback is a pointer to each element
    of the array.
*/
int Array_DoCallbackArg(Array_t* Array, CallbackArgFunc_t* Callback, void* Args);

/*
    Array_Sort

    This function sorts the array using the provided comparison function,
    ensuring that the array contents are in the desired sorted order following
    successful execution of this function.

    Inputs:
    Array       -   Pointer to the Array_t to operate on.
    CompareFunc -   Pointer to the function to use to compare two elements of the array.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.

    Note:
    See the documentation for CompareFunc_t for more information on this function.
    This must provide a total pre-order over the type contained in the array, and
    if a stable sort is desired, a stable comparison function must be provided.
*/
int Array_Sort(Array_t* Array, CompareFunc_t* CompareFunc);

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

/* ---------- Public Array_t Functions ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_LIST
/* ++++++++++ Public List_t Functions ++++++++++ */

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
    List_Clear

    This function will release all entries from the List, without
    releasing the list itself.

    Inputs:
    List    -   Pointer to the List_t to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int List_Clear(List_t* List);

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
int List_Insert(List_t* List, const void* Element, size_t ElementSize, size_t Index);

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
int List_RefInsert(List_t* List, void* Element, ReleaseFunc_t* ReleaseFunc, size_t Index);

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
int List_RefPrepend(List_t* List, void* Element, ReleaseFunc_t* ReleaseFunc);

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
int List_RefAppend(List_t* List, void* Element, ReleaseFunc_t* ReleaseFunc);

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
int List_Remove(List_t* List, size_t Index);

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
void* List_GetElement(List_t* List, size_t Index);

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
int List_SetElement(List_t* List, void* Element, size_t ElementSize, size_t Index);

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
int List_RefSetElement(List_t* List, void* Element, ReleaseFunc_t* ReleaseFunc, size_t Index);

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
void* List_PopElement(List_t* List, size_t Index);

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

/*
    List_Next

    This function iterates over the List in the <> direction,
    returning each new item in turn. This allows for single-step
    and resumable iteration over the container, in a more granular and
    explicit manner than what is provided by the Callback() interface.

    Inputs:
    List    -   Pointer to the List to iterate over.

    Outputs:
    void*   -   Pointer to the corresponding item from the List.
                    NULL on error, and when iteration reaches the end
                    of the List.

    Note:
    Iteration over the List will reset after any Insert(), Remove(), or Pop()
    operation on the List, as these operations can change the ordering of items
    within the container.
*/
void* List_Next(List_t* List);

/*
    List_Previous

    This function iterates over the List in the <> direction,
    returning each new item in turn. This allows for single-step
    and resumable iteration over the container, in a more granular and
    explicit manner than what is provided by the Callback() interface.

    Inputs:
    List    -   Pointer to the List to iterate over.

    Outputs:
    void*   -   Pointer to the corresponding item from the List.
                    NULL on error, and when iteration reaches the end
                    of the List.

    Note:
    Iteration over the List will reset after any Insert(), Remove(), or Pop()
    operation on the List, as these operations can change the ordering of items
    within the container.
*/
void* List_Previous(List_t* List);

/*
    List_DoCallback

    This function will perform the given callback function on each of the elements of
    the list in sequence. This is helpful for things like printing or otherwise manipulating
    the list value.

    Inputs:
    List        -   Pointer to the list to operate on.
    Callback    -   Pointer to the Callback function to call for each item of the list.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the List_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" pointer provided to the Callback points to the Value of the items
    in the list.
*/
int List_DoCallback(List_t* List, CallbackFunc_t* Callback);

/*
    List_DoCallbackArg

    This function will perform the given callback function on each of the elements
    of the list in sequence. This extends the List_DoCallback function by allowing
    additional parameters to be passed in to this callback function to allow for
    richer and more complex behaviour.

    Inputs:
    List        -   Pointer to the list to operate on.
    Callback    -   Pointer to the Callback function to call for each item of the list.
    Args        -   Pointer to some additional argument(s), values, resources, etc. to pass
                        in to the Callback function along with the item value.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the List_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" pointer provided to the Callback points to the Value of the items
    in the list.
*/
int List_DoCallbackArg(List_t* List, CallbackArgFunc_t* Callback, void* Args);

/* ---------- Public List_t Functions ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_HASHMAP
/* ++++++++++ Public Default HashFunc_t Functions ++++++++++ */

/*
    HashFunc_Int

    This function computes the necessary hash value for an *int* type key
    for use with the Hashmap_t container.

    Inputs:
    Key     -   Pointer to the key value to compute the hash value of.
    KeySize -   Size of the key (in bytes). Optional as Int has a known constant size.

    Outputs:
    unsigned int    -   The hashed value of the key, to be used internally by the Hashmap_t to store the
                            corresponding value.

    Note:
    As this function is part of the built-in set provided by this library, and the key is a
    constant size, the KeySize value can be always ignored for a Hashmap using this function
    as its HashFunc.
*/
HashFunc_t HashFunc_Int;

/*
    HashFunc_Long

    This function computes the necessary hash value for an *long* type key
    for use with the Hashmap_t container.

    Inputs:
    Key     -   Pointer to the key value to compute the hash value of.
    KeySize -   Size of the key (in bytes). Optional as Int has a known constant size.

    Outputs:
    unsigned int    -   The hashed value of the key, to be used internally by the Hashmap_t to store the
                            corresponding value.

    Note:
    As this function is part of the built-in set provided by this library, and the key is a
    constant size, the KeySize value can be always ignored for a Hashmap using this function
    as its HashFunc.
*/
HashFunc_t HashFunc_Long;

/*
    HashFunc_Double

    This function computes the necessary hash value for a *double* type key
    for use with the Hashmap_t container.

    Inputs:
    Key     -   Pointer to the key value to compute the hash value of.
    KeySize -   Size of the key (in bytes). Optional as Double has a known constant size.

    Outputs:
    unsigned int    -   The hashed value of the key, to be used internally by the Hashmap_t to store the
                            corresponding value.

    Note:
    As this function is part of the built-in set provided by this library, and the key is a
    constant size, the KeySize value can be always ignored for a Hashmap using this function
    as its HashFunc.
*/
HashFunc_t HashFunc_Double;

/*
    HashFunc_String

    This function computes the necessary hash value for a *string* (char*) type key
    for use with the Hashmap_t container.

    Inputs:
    Key     -   Pointer to the key value to compute the hash value of.
    KeySize -   Optional, and changes the interpretation of the Key if present.
                    If 0, use strlen() to compute the length of the given string.
                    If non-zero, use KeySize as the length of the given string.

    Outputs:
    unsigned int    -   The hashed value of the key, to be used internally by the Hashmap_t to store the
                            corresponding value.

    Note:
    This is the default HashFunc if NULL is given when creating a Hashmap_t.
*/
HashFunc_t HashFunc_String;

/* ---------- Public Default HashFunc_t Functions ---------- */

/* ++++++++++ Public Hashmap_t Functions ++++++++++ */

/*
    Hashmap_Create

    This function will create and prepare a Hashmap_t for use. As this takes in a generic HashFunc
    and ReleaseFunc for the Keys, this can be used with *arbitrary* key types, but only one key
    type per Hashmap_t.

    Inputs:
    HashFunc        -   Pointer to the function to use to convert the given Key type to a Hash value
                            for locating the corresponding item into the Hashmap. See the documentation
                            for the HashFunc_t type for more information.
    KeySize         -   The size (in bytes) of the Key type for this Hashmap_t.
                            If the type is [ string ], this can be 0. Generally, this is a sizeof() based
                            argument.
    KeyReleaseFunc  -   Pointer to the function to use to release any resources held by the Key types
                            for when an entry is released.

    Outputs:
    Hashmap_t*  -   A fully prepared and ready-to-use Hashmap_t on success, or NULL on failure.
*/
Hashmap_t* Hashmap_Create(HashFunc_t* HashFunc, size_t KeySize, ReleaseFunc_t* KeyReleaseFunc);

/*
    Hashmap_Length

    This function will return the "length" of the Hashmap_t, i.e. the number of items it contains.

    Inputs:
    Map     -   Pointer to the Hashmap to query the length of.

    Outputs:
    size_t  -   The count of items contained within the Hashmap_t.
*/
size_t Hashmap_Length(Hashmap_t* Map);

/*
    Hashmap_KeyExists

    This function will check the Hashmap for the given Key, checking if it exists or not.
    This is valuable check before insertions, as Hashmap_Insert() overwrites earlier contents.

    Inputs:
    Map     -   Pointer to the Hashmap_t to operate on.
    Key     -   Pointer to the Key Value to use.
    KeySize -   Size of the Key value, as measured in bytes.

    Outputs:
    bool    -   Returns true if the key exists within the Hashmap, false if it does not exist.
*/
bool Hashmap_KeyExists(Hashmap_t* Map, const void* Key, size_t KeySize);

/*
    Hashmap_Insert

    This function will add a key-value pair to the Hashmap, releasing and overwriting
    the contents if the given Key already exists within the Map.

    Inputs:
    Map                 -   Pointer to the Hashmap_t to operate on.
    Key                 -   Pointer to the Key Value to use.
    Value               -   Pointer to the Value associated with the Key to add.
    KeySize             -   Size of the Key value, as measured in bytes.
    ValueSize           -   Size of the Value, as measured in bytes.
    ValueReleaseFunc    -   Pointer to the function to call to release the resources associated
                                with the Value.

    Outputs:
    int     -   Returns 0 on success, nonzero on failure.

    Note:
    Unlike several other functions here where KeySize is an optional parameter, it is absolutely
    required here. This value must be equal to the number of bytes required to generate a copy of
    the Key, typically in the form of a sizeof() operator. For string keys, strlen() is more
    commonly used.
*/
int Hashmap_Insert(Hashmap_t* Map, void* Key, void* Value, size_t KeySize, size_t ValueSize, ReleaseFunc_t* ValueReleaseFunc);

/*
    Hashmap_Retrieve

    This function will return a pointer to the value corresponding to the given Key
    in the Hashmap if it exists. This pointer points to the value within the Hashmap,
    and any modifications will affect the contents within the Hashmap.

    Inputs:
    Map     -   Pointer to the Hashmap_t to operate on.
    Key     -   Pointer to the Key Value to use.
    KeySize -   Size of the Key value, as measured in bytes.

    Outputs:
    void*   -   Untyped pointer to the value corresponding to the given Key if it exists,
                    or NULL if the Key is not within the Hashmap.

    Note: The KeySize argument is optional, as the Hashmap will cache the value
            provided during Hashmap_Create(). Good practice still provides this,
            but it's not entirely necessary.
*/
void *Hashmap_Retrieve(Hashmap_t *Map, const void *Key, size_t KeySize);

/*
    Hashmap_Keys

    This function returns the full set of current Keys contained in the Hashmap
    as an unsorted Array_t.

    Inputs:
    Map     -   Pointer to the Hashmap_t to operate on.

    Outputs:
    Array_t*    -   Pointer to an Array_t containing all of the Keys contained in the Hashmap.

    Note:
    This returned Array will contain pointers to the original items. If the
    original items are manipulated or deleted, this returned Array_t has no knowledge
    of this. It is the responsibility of the caller to release the returned
    Array_t with Array_Release().
*/
Array_t* Hashmap_Keys(Hashmap_t* Map);

/*
    Hashmap_KeysSorted

    This function is equivalent to Hashmap_Keys(), but additionally explicitly sorts
    the resulting Array based on the given CompareFunc.

    Inputs:
    Map         -   Pointer to the Hashmap_t to operate on.
    CompareFunc -   Pointer to the comparison function to use to
                        order the returned Keys.

    Outputs:
    Array_t*    -   Pointer to an Array_t containing all of the Keys contained in the Hashmap.

    Note:
    This returned Array will contain pointers to the original items. If the
    original items are manipulated or deleted, this returned Array_t has no knowledge
    of this. It is the responsibility of the caller to release the returned
    Array_t with Array_Release().
*/
Array_t* Hashmap_KeysSorted(Hashmap_t* Map, CompareFunc_t* CompareFunc);

/*
    Hashmap_Pop

    This function will return the value corresponding to the given Key from the Hashmap, transferring
    ownership to the caller. Regardless of the underlying type, the returned pointer must
    be release with a call to free() or equivalent for the type. This functions roughly
    equivalent to a Hashmap_Retrieve() followed by a Hashmap_Remove(), but adds the ownership
    transfer.

    Inputs:
    Map     -   Pointer to the Hashmap_t to operate on.
    Key     -   Pointer to the Key Value to use.
    KeySize -   Size of the Key value, as measured in bytes.

    Outputs:
    void*   -   Untyped pointer to the value corresponding to the given Key if it exists,
                    or NULL if the Key is not within the Hashmap.

    Note: The KeySize argument is optional, as the Hashmap will cache the value
            provided during Hashmap_Create(). Good practice still provides this,
            but it's not entirely necessary.
*/
void* Hashmap_Pop(Hashmap_t *Map, const void *Key, size_t KeySize);

/*
    Hashmap_Remove

    This function will remove the value corresponding to the Key from the Hashmap
    if it exists, or do nothing if it doesn't exist.

    Inputs:
    Map     -   Pointer to the Hashmap_t to operate on.
    Key     -   Pointer to the Key Value to use.
    KeySize -   Size of the Key value, as measured in bytes.

    Outputs:
    int -   Returns 0 on success, nonzero on failure.

    Note: The KeySize argument is optional, as the Hashmap will cache the value
            provided during Hashmap_Create(). Good practice still provides this,
            but it's not entirely necessary.
*/
int Hashmap_Remove(Hashmap_t* Map, const void* Key, size_t KeySize);

/*
    Hashmap_Next

    This function returns the "Next" in the Hashmap, allowing for single-step iteration
    over the container in a richer manner than is provided by the Callback() API.
    This provides a "forward" iteration over the map, which given the "random" ordering
    of Keys, the exact iteration order is not well-defined. All that it guaranteed is that
    Next() -> Previous() -> Next() will return the same Key-Value pair from each call
    to Next().

    Inputs:
    Map     -   Pointer to the Hashmap to operate on.

    Outputs:
    Hashmap_KeyValuePair_t  -   A simple struct, containing pointers to the Key
                                    and value for the returned Item.
*/
Hashmap_KeyValuePair_t Hashmap_Next(Hashmap_t* Map);

/*
    Hashmap_SortedNext

    This function is equivalent to Hashmap_Next(), with the added functionality
    of ensuring the items are traversed in Key-Sorted order, with ordering
    defined by the CompareFunc provided.

    Inputs:
    Map     -   Pointer to the Hashmap to operate on.
    CompareFunc -   Pointer to the function to call to help sort the Keys into
                        a well-defined ordering. See the documentation for a
                        CompareFunc_t for more information on what can be passed
                        here.

    Outputs:
    Hashmap_KeyValuePair_t  -   A simple struct, containing pointers to the Key
                                    and value for the returned Item.
*/
Hashmap_KeyValuePair_t Hashmap_SortedNext(Hashmap_t* Map, CompareFunc_t* CompareFunc);

/*
    Hashmap_Previous

    This function is equivalent to Hashmap_Next(), but provides iteration in the reverse
    direction. For this unsorted case, there is no major difference between calling
    Hashmap_Next() versus Hashmap_Previous(), as the Hashmap key values do not have
    a well-defined ordering by default.

    Inputs:
    Map     -   Pointer to the Hashmap to operate on.

    Outputs:
    Hashmap_KeyValuePair_t  -   A simple struct, containing pointers to the Key
                                    and value for the returned Item.
*/
Hashmap_KeyValuePair_t Hashmap_Previous(Hashmap_t* Map);

/*
    Hashmap_SortedPrevious

    This function is equivalent to Hashmap_SortedNext(), except in that it traverses
    the sorted Keys in the opposite direction. If, for example Hashmap_NextSorted()
    operates in ascending order, this would operate in descending order.

    Inputs:
    Map     -   Pointer to the Hashmap to operate on.
    CompareFunc -   Pointer to the function to call to help sort the Keys into
                        a well-defined ordering. See the documentation for a
                        CompareFunc_t for more information on what can be passed
                        here.

    Outputs:
    Hashmap_KeyValuePair_t  -   A simple struct, containing pointers to the Key
                                    and value for the returned Item.
*/
Hashmap_KeyValuePair_t Hashmap_SortedPrevious(Hashmap_t* Map, CompareFunc_t* CompareFunc);

/*
    Hashmap_DoCallback

    This function will perform the given callback function on each of the elements of
    the hashmap in sequence. This is helpful for things like printing or otherwise manipulating
    the hashmap value.

    Inputs:
    Map         -   Pointer to the hashmap to operate on.
    Callback    -   Pointer to the Callback function to call for each item of the hashmap.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Hashmap_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    For a Hashmap Callback function, the *Value* pointer provided to the callback
    is a pointer to a Hashmap_KeyValuePair_t.
*/

int Hashmap_DoCallback(Hashmap_t* Map, CallbackFunc_t* Callback);
/*
    Hashmap_DoCallbackArg

    This function will perform the given callback function on each of the elements
    of the hashmap in sequence. This extends the Hashmap_DoCallback function by allowing
    additional parameters to be passed in to this callback function to allow for
    richer and more complex behaviour.

    Inputs:
    Map         -   Pointer to the hashmap to operate on.
    Callback    -   Pointer to the Callback function to call for each item of the hashmap.
    Args        -   Pointer to some additional argument(s), values, resources, etc. to pass
                        in to the Callback function along with the item value.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Hashmap_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    For a Hashmap Callback function, the *Value* pointer provided to the callback
    is a pointer to a Hashmap_KeyValuePair_t.
*/

int Hashmap_DoCallbackArg(Hashmap_t* Map, CallbackArgFunc_t* Callback, void* Args);

/*
    Hashmap_Clear

    This function will release all entries from the Hashmap, without releasing
    the Hashmap itself.

    Inputs:
    Map     -   Pointer to the Hashmap to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Hashmap_Clear(Hashmap_t* Map);

/*
    Hashmap_Release

    This function will fully release the Hashmap_t and all of the entries it contains.
    As long as the provided KeyReleaseFunc and ValueReleaseFunc values satisfy the expectation
    of fully releasing their corresponding contents, this will fully and safely release
    all held resources.

    Inputs:
    Map     -   Pointer to the Hashmap_t to operate on.

    Outputs:
    None, the Hashmap_t and all of the held contents are fully released with the previously provided
    ReleaseFunc_t values.
*/
void Hashmap_Release(Hashmap_t* Map);

/* ---------- Public Hashmap_t Functions ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_BINARY_TREE
/* +++++++++ Public Binary_Tree_t Functions +++++++++ */

/*
    Binary_Tree_Create

    This function will create and initialize a new self-balancing binary search tree container.
    All entries within a single Tree must be of the same type, so the ValueSize and ReleaseFunc
    provided here are cached for when items are added.

    Inputs:
    KeyCompareFunc  -   Pointer to the function to use to compare keys. Leave NULL to default to memcmp().
    KeySize         -   The size of the Key, measured in bytes, if the keys are all the same size.
                            Allows caching to not be required for future operations on the tree.
    KeyReleaseFunc  -   Pointer to the function to use to release a Key. Leave NULL to default to free().
    Policy          -   The policy for how to handle duplicated Keys within the Tree.

    Outputs:
    Binary_Tree_t*  -   Pointer to a fully constructed and initialized Binary Tree, or NULL
                            on failure.
*/
Binary_Tree_t* Binary_Tree_Create(CompareFunc_t* KeyCompareFunc, size_t KeySize, ReleaseFunc_t* KeyReleaseFunc, Binary_Tree_DuplicatePolicy_t Policy);

/*
    Binary_Tree_Length

    This function will return the "length" of the Tree, i.e. the number of items it contains.
    This is called *_Length() purely for consistency with the other containers in this library.

    Inputs:
    Tree    -   Pointer to the Tree to return the length of.

    Outputs:
    size_t  -   The number of items contained in the Tree. 0 if a NULL tree is provided.

    Note:
    This is an O(1) operation on the Tree. No traversal is performed to count the items.
*/
size_t Binary_Tree_Length(Binary_Tree_t* Tree);

/*
    Binary_Tree_Insert

    This function adds a new Key/Value pair to the Tree. If the given Key is already
    found within the Tree, the existing item will be overwritten with the new Value.

    Inputs:
    Tree                -   Pointer to the Tree to operate on.
    Key                 -   Pointer to the Key value to insert.
    KeySize             -   The size of the Key value to be inserted.
    Value               -   Pointer to the Value to insert.
    ValueSize           -   The size of the Value to be inserted.
    ValueReleaseFunc    -   Pointer to the function to call to release the resources associated
                                with the Value.

    Outputs:
    int     -   Returns 0 on success, positive on error. If the Duplicate Policy is set to "Policy_Error",
                    returns negative if the item already exists.
*/
int Binary_Tree_Insert(Binary_Tree_t* Tree, void* Key, size_t KeySize, void* Value, size_t ValueSize, ReleaseFunc_t* ValueReleaseFunc);

/*
    Binary_Tree_KeyExists

    This function will check if the given Key exists within the Tree.
    If this returns true, and no *_Remove() or *_Pop() are performed,
    the next *_Get() or *_Pop() will guarantee a non-NULL return value.

    Inputs:
    Tree    -   Pointer to the Tree to operate on.
    Key     -   Pointer to the Key value to insert.
    KeySize -   The size of the Key value to be inserted.

    Outputs:
    bool    -   Returns true if the Key exists within the tree, false if it does not.
*/
bool Binary_Tree_KeyExists(Binary_Tree_t* Tree, void* Key, size_t KeySize);

/*
    Binary_Tree_Get

    This function returns a pointer to the value associated with the given Key,
    if it exists within the Tree.

    Inputs:
    Tree    -   Pointer to the Tree to operate on.
    Key     -   Pointer to the Key value to insert.
    KeySize -   The size of the Key value to be inserted.

    Outputs:
    void*   -   Pointer to the Value corresponding to the given Key. NULL on error,
        or if the key doesn't exist.
*/
void* Binary_Tree_Get(Binary_Tree_t* Tree, void* Key, size_t KeySize);

/*
    Binary_Tree_Pop

    This function returns the value associated with the given Key,
    if it exists within the Tree. The Key and Value are removed from
    the Tree, having been transferred to the caller.

    Inputs:
    Tree    -   Pointer to the Tree to operate on.
    Key     -   Pointer to the Key value to insert.
    KeySize -   The size of the Key value to be inserted.

    Outputs:
    Binary_Tree_KeyValuePair_t  -   A pair of pointers holding the returned
        Key-Value pair from the Tree.

    Note:
    Ownership of the resources of the Value popped from the tree are transferred to the
    caller.
*/
Binary_Tree_KeyValuePair_t Binary_Tree_Pop(Binary_Tree_t* Tree, void* Key, size_t KeySize);

/*
    Binary_Tree_DoCallback

    This function traverses the Tree, calling the given Callback function on
    each of the items. The tree is traversed in-order.

    Inputs:
    Tree        -   Pointer to the Tree to operate on.
    Direction   -   The (forward-only) direction to traverse the tree in order to perform
                        the Callback function on each item.
    Callback    -   Pointer to the Callback function to call for each item
                        in the Tree.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Binary_Tree_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" provided to the Callback function consists of a
        void*[2] = { Key, Value }
    A pointer to the Key, and the pointer to the Value of each item.
*/
int Binary_Tree_DoCallback(Binary_Tree_t* Tree, Binary_Tree_Direction_t Direction, CallbackFunc_t* Callback);

/*
    Binary_Tree_DoCallbackArg

    This function traverses the Tree, calling the given Callback function on
    each of the items. The tree is traversed in-order.

    Inputs:
    Tree        -   Pointer to the Tree to operate on.
    Direction   -   The (forward-only) direction to traverse the tree in order to perform
                        the Callback function on each item.
    Callback    -   Pointer to the Callback function to call for each item
                        in the Tree.
    Args        -   Optional additional arguments to pass to the Callback
                        function along with the Key-Value pair.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Binary_Tree_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" provided to the Callback function consists of a
        void*[2] = { Key, Value }
    A pointer to the Key, and the pointer to the Value of each item.
*/
int Binary_Tree_DoCallbackArg(Binary_Tree_t* Tree, Binary_Tree_Direction_t Direction, CallbackArgFunc_t* Callback, void* Args);

/*
    Binary_Tree_Remove

    This function will remove the item associated with the given Key from the Tree,
    if it exists. If the key does not exist, this does nothing.

    Inputs:
    Tree    -   Pointer to the Tree to operate on.
    Key     -   Pointer to the Key value to insert.
    KeySize -   The size of the Key value to be inserted.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Binary_Tree_Remove(Binary_Tree_t* Tree, void* Key, size_t KeySize);

/*
    Binary_Tree_Next

    This function allows for single-step iteration over the Binary Tree, in the specified
    direction. This iterates in the "Forward" direction, defined based on the ordering
    of the Key field of the Binary Tree items. This is more efficient for iterating
    over the tree than trying to retrieve the keys and perform a traversal to retrieve
    each item.

    Inputs:
    Tree        -   Pointer to the Binary Tree to iterate over.
    Direction   -   The (forward-only) direction to traverse the tree in order to perform
                        the Callback function on each item.

    Outputs:
    Binary_Tree_KeyValuePair_t  -   A simple struct containing a copy of the integer
                                        Key associated with the item, and a pointer to the
                                        actual item contents itself.

    Note:
    Since the Key field of the returned Binary_Tree_KeyValuePair_t is an integer, the Value
    pointer is used to define whether or not the returned value has meaning. A NULL Value
    pointer implies that iteration has finished.

    Also note that changing the *Direction* value during iteration may not lead to helpful
    behaviour. The returned value is correct for the given Direction and most recently
    returned value, however by changing direction, this may ignore or repeat sections of the
    tree.
*/
Binary_Tree_KeyValuePair_t Binary_Tree_Next(Binary_Tree_t* Tree, Binary_Tree_Direction_t Direction);

/*
    Binary_Tree_Previous

    This function is exactly the same as Binary_Tree_Next(), except that it iterates
    in the reverse order for a given Tree and Direction.

    Inputs:
    Tree        -   Pointer to the Binary Tree to iterate over.
    Direction   -   The (forward-only) direction to traverse the tree in order to perform
                        the Callback function on each item.

    Outputs:
    Binary_Tree_KeyValuePair_t  -   A simple struct containing a copy of the integer
                                        Key associated with the item, and a pointer to the
                                        actual item contents itself.

    Note:
    Since the Key field of the returned Binary_Tree_KeyValuePair_t is an integer, the Value
    pointer is used to define whether or not the returned value has meaning. A NULL Value
    pointer implies that iteration has finished.

    Also note that changing the *Direction* value during iteration may not lead to helpful
    behaviour. The returned value is correct for the given Direction and most recently
    returned value, however by changing direction, this may ignore or repeat sections of the
    tree.
*/
Binary_Tree_KeyValuePair_t Binary_Tree_Previous(Binary_Tree_t* Tree, Binary_Tree_Direction_t Direction);

/*
    Binary_Tree_Clear

    This function releases all items within the Tree, without releasing
    the tree itself.

    Inputs:
    Tree    -   Pointer to the tree to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Binary_Tree_Clear(Binary_Tree_t* Tree);

/*
    Binary_Tree_Release

    This function fully releases the tree and all of the items added to it.
    As long as the provided ReleaseFunc correctly releases the Values passed
    to this container, this will fully release everything safely.

    Inputs:
    Tree    -   Pointer to the Tree to operate on.

    Outputs:
    None, The Tree and all contents are fully released.
*/
void Binary_Tree_Release(Binary_Tree_t* Tree);

/* --------- Public Binary_Tree_t Functions --------- */
#endif

#ifdef LIBCONTAINER_ENABLE_BINARY_HEAP
/* ++++++++++ Public Binary_Heap_t Functions ++++++++++ */

/*
    Binary_Heap_Create

    This function creates and initializes a new Binary_Heap_t structure.

    Inputs:
    KeyCompareFunc  -   Pointer to the function to use to compare two Key values within the Heap.
    KeyReleaseFunc  -   Pointer to the function to use to release the resources associated with a Key
                            within the Heap.

    Outputs:
    Binary_Heap_t*  -   Returns a pointer to an initialized and ready-to-use
                            Binary_Heap_t on success, or NULL on failure.
*/
Binary_Heap_t* Binary_Heap_Create(CompareFunc_t* KeyCompareFunc, ReleaseFunc_t* KeyReleaseFunc);

/*
    Binary_Heap_Length

    This function returns the count of the number of items contained
    within the Heap.

    Inputs:
    Heap    -   Pointer to the Binary_Heap_t to operate on.

    Outputs:
    size_t  -   Count of the items within the Heap. 0 if Heap is NULL.
*/
size_t Binary_Heap_Length(Binary_Heap_t* Heap);

/*
    Binary_Heap_IsEmpty

    This function allows for simply inspection of whether or not the
    Heap contains items or not.

    Inputs:
    Heap    -   Pointer to the Binary_Heap_t to operate on.

    Outputs:
    bool    -   Returns true if Heap is NULL, or it contains 0 items.
*/
bool Binary_Heap_IsEmpty(Binary_Heap_t* Heap);

/*
    Binary_Heap_Peek

    This function inspects the Root of the Heap, returning a Binary_Heap_KeyValuePair_t
    containing pointers t the Root Key-Value pair. This does NOT remove the item
    from the Heap, and does NOT transfer ownership to the caller.

    Inputs:
    Heap    -   Pointer to the Binary_Heap_t to operate on.

    Outputs:
    Binary_Heap_KeyValuePair_t  -   Struct containing the Key* and Value* pointers
                                        for the requested item within the Heap.

    Note:
    This function does NOT transfer ownership of the returned items to the caller.
    This simply lets the caller inspect the root of the Heap in order to make
    a decision about whether or not to pop the value or remove it later.
*/
Binary_Heap_KeyValuePair_t Binary_Heap_Peek(Binary_Heap_t* Heap);

/*
    Binary_Heap_Pop

    This function removes the Root of the Heap, returning the Key-Value
    pair to the caller and transferring ownership of the resources.
    This also removes the item from the Heap.

    Inputs:
    Heap    -   Pointer to the Binary_Heap_t to operate on.

    Outputs:
    Binary_Heap_KeyValuePair_t  -   Struct containing the Key* and Value* pointers
                                        for the popped item from the Heap.
*/
Binary_Heap_KeyValuePair_t Binary_Heap_Pop(Binary_Heap_t* Heap);

/*
    Binary_Heap_Remove

    This function removes the Root from the Heap, but does not return it.

    Inputs:
    Heap    -   Pointer to the Binary_Heap_t to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Binary_Heap_Remove(Binary_Heap_t* Heap);

/*
    Binary_Heap_Push

    This function adds a new Key-Value pair to the Heap, ensuring that the
    Heap maintains the correct shape and structure guarantees.

    Inputs:
    Heap    -   Pointer to the Binary_Heap_t to operate on.
    Key                 -   Pointer to the Key of the Key-Value pair to add.
    KeySize             -   Size of the Key (in bytes). 0 for a Reference-Type.
    Value               -   Pointer to the Value of the Key-Value pair to add.
    ValueSize           -   Size of the Value (in bytes). 0 for a Reference-Type.
    ValueReleaseFunc    -   Pointer to the function to call to release the resources
                                associated with the Value. Leave NULL to default to free().

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Binary_Heap_Push(Binary_Heap_t* Heap, void* Key, size_t KeySize, void* Value, size_t ValueSize, ReleaseFunc_t* ValueReleaseFunc);

/*
    Binary_Heap_Next

    This function provides an iterator interface over the Heap.
    This iterator provides level-order iteration over the Heap.
    This is resumable, as long as there are no intervening
    Push(), Pop(), or Remove() calls between pause and resumption.

    Inputs:
    Heap    -   Pointer to the Binary_Heap_t to operate on.

    Outputs:
    Binary_Heap_KeyValuePair_t  -   Struct containing the Key* and Value* pointers
                                        for the "next" item within the Heap.

    Note:
    This will return a Binary_Heap_KeyValuePair_t with NULL for both Key and Value
    pointers when iteration has finished.
*/
Binary_Heap_KeyValuePair_t Binary_Heap_Next(Binary_Heap_t* Heap);

/*
    Binary_Heap_DoCallback

    This function performs a Callback function on each item contained within the
    Heap.

    Inputs:
    Heap        -   Pointer to the Binary_Heap_t to operate on.
    Callback    -   Pointer to a function to call on each item contained
                        within the Binary_Heap_t.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Heap could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" passed into the Callback function is a Binary_KeyValuePair_t.
*/
int Binary_Heap_DoCallback(Binary_Heap_t* Heap, CallbackFunc_t* Callback);

/*
    Binary_Heap_DoCallbackArg

    This function performs a Callback function on each item contained within the
    Heap.

    Inputs:
    Heap        -   Pointer to the Binary_Heap_t to operate on.
    Callback    -   Pointer to a function to call on each item contained
                        within the Binary_Heap_t.
    Args        -   (Optional) Pointer to additional data to pass into the Callback
                        function along with the Key-Value pair from the Heap.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Heap could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" passed into the Callback function is a Binary_KeyValuePair_t.
*/
int Binary_Heap_DoCallbackArg(Binary_Heap_t* Heap, CallbackArgFunc_t* Callback, void* Args);

/*
    Binary_Heap_Clear

    This function removes and releases all contents of the Binary_Heap_t,
    but retains the Heap itself for future use. This is a weaker function
    than Binary_Heap_Release().

    Inputs:
    Heap    -   Pointer to the Binary_Heap_t to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.

    Note:
    The Heap pointer is still valid for future use after being passed to this function.
    This simply allows for easily resetting the Heap to a default initialized state.
*/
int Binary_Heap_Clear(Binary_Heap_t* Heap);

/*
    Binary_Heap_Release

    This function fully releases the resources held by the Binary_Heap_t
    and all of the contents it holds. After being passed to this function,
    the pointer is no longer valid for use.

    Inputs:
    Heap    -   Pointer to the Binary_Heap_t to operate on.

    Outputs:
    None, the Binary_Heap_t and all held contents are fully released.
*/
void Binary_Heap_Release(Binary_Heap_t* Heap);

/* ---------- Public Binary_Heap_t Functions ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_SET
/* ++++++++++ Public Set_t Functions ++++++++++ */

/*
    Set_Create

    This function will create and initialize a new Set_t, preparing it to
    be used.

    Inputs:
    CompareFunc -   Pointer to the function to use to compare different values within
                        the Set_t, to provide the uniqueness and ordering guarantees.
                        if NULL, this will default to memcmp(). See the documentation
                        for CompareFunc_t for more information on creating a function to satisfy
                        this type.
    ValueSize   -   The size of the values to be added to the Set_t. Pass 0
                        if the values are pointer-types. This value, if non-zero,
                        will be cached for use with the rest of the Set_t API functions.
    ReleaseFunc -   Pointer to the function to call to release a Value from the Set_t.
                        If NULL, this will default to free().

    Outputs:
    Set_t*  -   Pointer to a created and initialized Set_t on success, NULL on failure.
*/
Set_t* Set_Create(CompareFunc_t* CompareFunc, size_t ValueSize, ReleaseFunc_t* ReleaseFunc);

/*
    Set_Length

    This function returns the number of items contained within the Set_t.

    Inputs:
    Set     -   Pointer to the Set_t to operate on.

    Outputs:
    size_t  -   The number of items contained within the Set_t.

    Note:
    This function also reports 0 for a NULL Set_t.
*/
size_t Set_Length(Set_t* Set);

/*
    Set_Insert

    This function attempts to add the given Value to the Set_t. This will fail if the item
    already exists in the Set.

    Inputs:
    Set         -   Pointer to the Set_t to operate on.
    Value       -   Pointer to the Value to <> to the Set_t.
    ValueSize   -   The size of the Value, in bytes.

    Outputs:
    int     -   Returns 0 on success, positive on error, negative if the item already exists.
*/
int Set_Insert(Set_t* Set, void* Value, size_t ValueSize);

/*
    Set_ValueExists

    This function checks if a given Value exists within the Set_t.

    Inputs:
    Set         -   Pointer to the Set_t to operate on.
    Value       -   Pointer to the Value to <> to the Set_t.
    ValueSize   -   The size of the Value, in bytes.

    Outputs:
    bool    -   True if the item exists, false if the item doesn't exist.

    Note:
    This also returns false if given a NULL Set_t.
*/
bool Set_ValueExists(Set_t* Set, void* Value, size_t ValueSize);

/*
    Set_Next

    This function returns the "Next" value from the Set_t. The ordering
    depends on the ordering implied by the CompareFunc_t given when
    the container was initialized.

    Inputs:
    Set     -   Pointer to the Set_t to operate on.

    Outputs:
    void*   -   Pointer to the Next item from the container on success,
                    NULL on error, or when iteration finished.

    Note:
    The Set_t container provides a SORTED iterator over the items it contains.
    The exact ordering depends on the order defined by the comparison function
    the Set_t uses. For integral types, this is generally ascending order,
    and for char* (C-String) types, this is lexicographic order.

    Any ordering is possible, you simply have to specify a comparison function
    which defines that ordering over the Value type.
*/
void* Set_Next(Set_t* Set);

/*
    Set_DoCallback

    This function performs a Callback function on each item within the container,
    in sorted order. This provides a more compact mechanism than directly using the
    *_Next() function.

    Inputs:
    Set     -   Pointer to the Set_t to operate on.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Set_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" pointer provided to the Callback function is a pointer to the Value added to
    the Set_t by a Set_Insert() call.
*/
int Set_DoCallback(Set_t* Set, CallbackFunc_t* Callback);

/*
    Set_DoCallbackArgs

    This function performs a Callback function on each item within the container,
    in sorted order. This provides a more compact mechanism than directly using the
    *_Next() function. This function extends the behaviour of Set_DoCallback()
    to allow providing an additional void* of arbitrary arguments to the Callback function.
    See the documentation for pthread_create() for an example of a similarly
    themed function.

    Inputs:
    Set     -   Pointer to the Set_t to operate on.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Set_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" pointer provided to the Callback function is a pointer to the Value added to
    the Set_t by a Set_Insert() call.
*/
int Set_DoCallbackArgs(Set_t* Set, CallbackArgFunc_t* Callback, void* Args);

/*
    Set_Remove

    This function removes the given Value from the Set_t, if it exists.
    If the Value does not exist within the Set_t, this call does nothing.

    Inputs:
    Set         -   Pointer to the Set_t to operate on.
    Value       -   Pointer to the Value to <> to the Set_t.
    ValueSize   -   The size of the Value, in bytes.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Set_Remove(Set_t* Set, void* Value, size_t ValueSize);

/*
    Set_Clear

    This function removes ALL items from the Set_t, while maintaining
    ownership of the Set_t. This is a looser function than Set_Release(),
    in that the Set_t is still valid for use after a call to Clear().

    Inputs:
    Set     -   Pointer to the Set_t to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Set_Clear(Set_t* Set);

/*
    Set_Release

    This function fully releases a Set_t and all held items. Once passed
    to this function, the given Set_t is no longer valid for use.

    Inputs:
    Set     -   Pointer to the Set_t to operate on.

    Outputs:
    None, the Set_t and all held contents are fully and completely released.
*/
void Set_Release(Set_t* Set);

/* ---------- Public Set_t Functions ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_STACK
/* ++++++++++ Public Stack_t Functions ++++++++++ */

/*
    Stack_Create

    This function creates and initializes a new Stack_t object for use. All items
    within a single Stack_t must be of the same type and size. For Reference-Type items,
    simply pass 0 as the ValueSize to indicate that their resources are held elsewhere.
    If passing string literals, these must be treated as non-reference types to avoid
    attempting to free() or otherwise release them.

    Inputs:
    ValueSize   -   The size (in bytes) of the items to contain. A 0 value indicates Reference
                        values.
    ReleaseFunc -   Pointer to the function to call to release the resources associated with
                        each item. Defaults to free() from libstd.h if NULL is given.

    Outputs:
    Stack_t*    -   Pointer to a fully initialized Stack_t object, or NULL on failure.
*/
Stack_t* Stack_Create(size_t ValueSize, ReleaseFunc_t* ReleaseFunc);

/*
    Stack_Length

    This function returns the "length", or number of items in the Stack.

    Inputs:
    Stack   -   Pointer to the Stack_t to operate on.

    Outputs:
    size_t  -   The count of items in the Stack. Returns 0 if given NULL.
*/
size_t Stack_Length(Stack_t* Stack);

/*
    Stack_IsEmpty

    This function returns a boolean indicating whether or not the stack is empty.

    Inputs:
    Stack   -   Pointer to the Stack_t to operate on.

    Outputs:
    bool    -   True if Stack is non-NULL and contains at least one item, false otherwise.
*/
bool Stack_IsEmpty(Stack_t* Stack);

/*
    Stack_Push

    This function pushes the given item to the top of the stack, adding it.

    Inputs:
    Stack       -   Pointer to the Stack_t to operate on.
    Value       -   Pointer to the value to push to the Stack.
    ValueSize   -   Size of the Value, in bytes. Leave 0 to use the cached value
                        from Stack initialization, or for reference-types.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Stack_Push(Stack_t* Stack, void* Value, size_t ValueSize);

/*
    Stack_Peek

    This function returns a pointer to the item at the top of the stack, without removing
    it from the Stack.

    Inputs:
    Stack   -   Pointer to the Stack_t to operate on.

    Outputs:
    void*   -   Pointer to the item currently at the top of the Stack.
                    NULL on failure or if the Stack is empty.
*/
void* Stack_Peek(Stack_t* Stack);

/*
    Stack_Pop

    This function removes the item from the top of the stack, returning it to the caller
    and transferring ownership to the caller.

    Inputs:
    Stack   -   Pointer to the Stack_t to operate on.

    Outputs:
    void*   -   Pointer to the item currently at the top of the Stack.
                    NULL on failure or if the Stack is empty.

    Note:
    Memory and resource ownership of the popped item is transferred to the caller,
    and must be safely handled and released to ensure no resource leaks.
*/
void* Stack_Pop(Stack_t* Stack);

/*
    Stack_DoCallback

    This function calls the given Callback function on each item contained in the Stack.

    Inputs:
    Stack       -   Pointer to the Stack_t to operate on.
    Callback    -   Pointer to the Callback function to apply to each item in the Stack.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Stack_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" pointer passed in to the Callback function is a pointer to the raw value
    of each item of the stack, as added by the Stack_Push() function.
*/
int Stack_DoCallback(Stack_t* Stack, CallbackFunc_t* Callback);

/*
    Stack_DoCallbackArg

    This function calls the given Callback function on each item contained in the Stack.
    This function additionally passes in the Args pointer to the callback.

    Inputs:
    Stack       -   Pointer to the Stack_t to operate on.
    Callback    -   Pointer to the Callback function to apply to each item in the Stack.
    Args        -   (Optional) Pointer to additional arguments/values to pass in to the Callback function.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Stack_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" pointer passed in to the Callback function is a pointer to the raw value
    of each item of the stack, as added by the Stack_Push() function.
*/
int Stack_DoCallbackArg(Stack_t* Stack, CallbackArgFunc_t* Callback, void* Args);

/*
    Stack_Clear

    This function removes and releases all items from the Stack, while retaining
    the Stack itself to be used again.

    Inputs:
    Stack   -   Pointer to the Stack_t to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Stack_Clear(Stack_t* Stack);

/*
    Stack_Release

    This function fully and safely releases all items held by the Stack, as well
    as the Stack itself.

    Inputs:
    Stack   -   Pointer to the Stack_t to operate on.

    Outputs:
    None, the Stack and all items it held are released. The pointer given is no longer
    valid for use after calling this function.

    Note:
    For this function to satisfy the promise of fully releasing all items
    it holds, the ReleaseFunc it is initialized with must safely and fully
    release all resources held by a given item.
*/
void Stack_Release(Stack_t* Stack);

/* ---------- Public Stack_t Functions ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_QUEUE
/* ++++++++++ Public Queue_t Functions ++++++++++ */

/*
    Queue_Create

    This function creates and initializes a new Queue_t object for use. All items
    within a single Queue_t must be of the same type and size. For Reference-Type items,
    simply pass 0 as the ValueSize to indicate that their resources are held elsewhere.
    If passing string literals, these must be treated as non-reference types to avoid
    attempting to free() or otherwise release them.

    Inputs:
    ValueSize   -   The size (in bytes) of the items to contain. A 0 value indicates Reference
                        values.
    ReleaseFunc -   Pointer to the function to call to release the resources associated with
                        each item. Defaults to free() from libstd.h if NULL is given.

    Outputs:
    Queue_t*    -   Pointer to a fully initialized Queue_t object, or NULL on failure.
*/
Queue_t* Queue_Create(size_t ValueSize, ReleaseFunc_t* ReleaseFunc);

/*
    Queue_Length

    This function returns the "length", or number of items in the Queue.

    Inputs:
    Queue   -   Pointer to the Queue_t to operate on.

    Outputs:
    size_t  -   The count of items in the Queue. Returns 0 if given NULL.
*/
size_t Queue_Length(Queue_t* Queue);

/*
    Queue_IsEmpty

    This function returns a boolean indicating whether or not the Queue is empty.

    Inputs:
    Queue   -   Pointer to the Queue_t to operate on.

    Outputs:
    bool    -   True if Queue is non-NULL and contains at least one item, false otherwise.
*/
bool Queue_IsEmpty(Queue_t* Queue);

/*
    Queue_Push

    This function pushes the given item to the back of the Queue, adding it.

    Inputs:
    Queue       -   Pointer to the Queue_t to operate on.
    Value       -   Pointer to the value to push to the Queue.
    ValueSize   -   Size of the Value, in bytes. Leave 0 to use the cached value
                        from Queue initialization, or for reference-types.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Queue_Push(Queue_t* Queue, void* Value, size_t ValueSize);

/*
    Queue_Peek

    This function returns a pointer to the item at the front of the Queue, without removing
    it from the Queue.

    Inputs:
    Queue   -   Pointer to the Queue_t to operate on.

    Outputs:
    void*   -   Pointer to the item currently at the top of the Queue.
                    NULL on failure or if the Queue is empty.
*/
void* Queue_Peek(Queue_t* Queue);

/*
    Queue_Pop

    This function removes the item from the front of the Queue, returning it to the caller
    and transferring ownership to the caller.

    Inputs:
    Queue   -   Pointer to the Queue_t to operate on.

    Outputs:
    void*   -   Pointer to the item currently at the top of the Queue.
                    NULL on failure or if the Queue is empty.

    Note:
    Memory and resource ownership of the popped item is transferred to the caller,
    and must be safely handled and released to ensure no resource leaks.
*/
void* Queue_Pop(Queue_t* Queue);

/*
    Queue_DoCallback

    This function calls the given Callback function on each item contained in the Queue.

    Inputs:
    Queue       -   Pointer to the Queue_t to operate on.
    Callback    -   Pointer to the Callback function to apply to each item in the Queue.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Queue_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" pointer passed in to the Callback function is a pointer to the raw value
    of each item of the Queue, as added by the Queue_Push() function.
*/
int Queue_DoCallback(Queue_t* Queue, CallbackFunc_t* Callback);

/*
    Queue_DoCallbackArg

    This function calls the given Callback function on each item contained in the Queue.
    This function additionally passes in the Args pointer to the callback.

    Inputs:
    Queue       -   Pointer to the Queue_t to operate on.
    Callback    -   Pointer to the Callback function to apply to each item in the Queue.
    Args        -   (Optional) Pointer to additional arguments/values to pass in to the Callback function.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Queue_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    The "Value*" pointer passed in to the Callback function is a pointer to the raw value
    of each item of the Queue, as added by the Queue_Push() function.
*/
int Queue_DoCallbackArg(Queue_t* Queue, CallbackArgFunc_t* Callback, void* Args);

/*
    Queue_Clear

    This function removes and releases all items from the Queue, while retaining
    the Queue itself to be used again.

    Inputs:
    Queue   -   Pointer to the Queue_t to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Queue_Clear(Queue_t* Queue);

/*
    Queue_Release

    This function fully and safely releases all items held by the Queue, as well
    as the Queue itself.

    Inputs:
    Queue   -   Pointer to the Queue_t to operate on.

    Outputs:
    None, the Queue and all items it held are released. The pointer given is no longer
    valid for use after calling this function.

    Note:
    For this function to satisfy the promise of fully releasing all items
    it holds, the ReleaseFunc it is initialized with must safely and fully
    release all resources held by a given item.
*/
void Queue_Release(Queue_t* Queue);

/* ---------- Public Queue_t Functions ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_PRIORITY_QUEUE
/* ++++++++++ Public Priority_Queue_t Functions ++++++++++ */

/*
    Priority_Queue_Create

    This function creates and initializes a new Priority_Queue_t, getting
    it fully ready for use.

    Inputs:
    Ascending   -   Set true if the highest priority values should be returned first,
                        or false if the lowest priority values should be returned first.

    Outputs:
    Priority_Queue_t*   -   Pointer to a created and initialized Priority_Queue_t
                                on success, or NULL on failure.
*/
Priority_Queue_t* Priority_Queue_Create(bool Ascending);

/*
    Priority_Queue_Length

    This function returns the "Length" or the number of items contained within the
    Priority_Queue_t.

    Inputs:
    Queue   -   Pointer to the Priority_Queue_t to operate on.

    Outputs:
    size_t  -   Returns the count of items within the Queue. Also returns 0
                    if the given Queue is NULL.
*/
size_t Priority_Queue_Length(Priority_Queue_t* Queue);

/*
    Priority_Queue_IsEmpty

    This function simply returns a boolean equivalent to (0 == Priority_Queue_Length(Queue)),
    making for a more visually explicit emptiness check.

    Inputs:
    Queue   -   Pointer to the Priority_Queue_t to operate on.

    Outputs:
    bool    -   True if the Queue contains 0 items or is NULL, false otherwise.
*/
bool Priority_Queue_IsEmpty(Priority_Queue_t* Queue);

/*
    Priority_Queue_Push

    This function adds a new item to the Queue, with the specified priority.

    Inputs:
    Queue       -   Pointer to the Priority_Queue_t to operate on.
    Priority    -   The Priority to assign to this entry.
    Value       -   Pointer to the raw item to add to the Queue.
    ValueSize   -   The size of the item (in bytes) to add to the Queue.
    ReleaseFunc -   Pointer to the function to call to release any resources
                        associated with the Value pointer. Leave NULL to
                        default to free().

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Priority_Queue_Push(Priority_Queue_t* Queue, int Priority, void* Value, size_t ValueSize, ReleaseFunc_t* ReleaseFunc);

/*
    Priority_Queue_Peek

    This function returns the front-most item from the Queue, without removing
    it from the Queue. Ownership of the returned value is NOT transferred to the
    caller.

    Inputs:
    Queue   -   Pointer to the Priority_Queue_t to operate on.

    Outputs:
    Priority_Queue_Item_t   -   Record-type containing the item at the front
                                    of the Queue, as well as the Priority value
                                    it was registered with.

    Note:
    Ownership of the returned Value pointer in the Priority_Queue_Item_t is NOT
    transferred to the caller. This function is primarily intended to query and
    inspect the Queue, not for working with the returned item itself.
*/
Priority_Queue_Item_t Priority_Queue_Peek(Priority_Queue_t* Queue);

/*
    Priority_Queue_Pop

    This function returns the item from the front of the Queue, removing it
    from the Queue and transferring ownership of the resources to the caller.

    Inputs:
    Queue   -   Pointer to the Priority_Queue_t to operate on.

    Outputs:
    Priority_Queue_Item_t   -   Record-type containing the item at the front
                                    of the Queue, as well as the Priority value
                                    it was registered with.

    Note:
    Ownership of the Value pointer within the Priority_Queue_Item_t is transferred
    to the caller, and must be handled accordingly so as to not leak resources.
*/
Priority_Queue_Item_t Priority_Queue_Pop(Priority_Queue_t* Queue);

/*
    Priority_Queue_Remove

    This function removes the item at the front of the Queue, without
    returning it to the caller. Essentially, Pop() is equivalent to a
    Peek() followed by a Remove().

    Inputs:
    Queue   -   Pointer to the Priority_Queue_t to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int Priority_Queue_Remove(Priority_Queue_t* Queue);

/*
    Priority_Queue_Next

    This function iterates over the Priority_Queue_t, returning each item
    in sequence to the caller. Ownership of the resources associated with
    the items is NOT transferred to the caller, and the items remain within
    the Queue following this function.

    Inputs:
    Queue   -   Pointer to the Priority_Queue_t to operate on.

    Outputs:
    Priority_Queue_Item_t   -   Record-type containing the item at the front
                                    of the Queue, as well as the Priority value
                                    it was registered with.

    Note:
    This iterator provides no guarantees about the order in which
    the items within the Queue are traversed. The only guarantee is that
    the FIRST item will have priority equal to the highest in the Queue.
*/
Priority_Queue_Item_t Priority_Queue_Next(Priority_Queue_t* Queue);

/*
    Priority_Queue_DoCallback

    This function performs a Callback function on each item contained within
    the Queue.

    Inputs:
    Queue   -   Pointer to the Priority_Queue_t to operate on.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Stack_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    This iterator provides no guarantees about the order in which
    the items within the Queue are traversed. The only guarantee is that
    the FIRST item will have priority equal to the highest in the Queue.
*/
int Priority_Queue_DoCallback(Priority_Queue_t* Queue, CallbackFunc_t* Callback);

/*
    Priority_Queue_DoCallbackArg

    This function performs a Callback function on each item contained within
    the Queue. This extends the basic Priority_Queue_DoCallback() by accepting
    and passing to the Callback, an additional Args pointer containing arbitrary data.

    Inputs:
    Queue   -   Pointer to the Priority_Queue_t to operate on.

    Outputs:
    int     -   Returns 0 on success, negative if iteration over the Stack_t could not happen,
                    and positive to indicate the number of Callback functions which returned non-zero.

    Note:
    This iterator provides no guarantees about the order in which
    the items within the Queue are traversed. The only guarantee is that
    the FIRST item will have priority equal to the highest in the Queue.
*/
int Priority_Queue_DoCallbackArg(Priority_Queue_t* Queue, CallbackArgFunc_t* Callback, void* Args);

/*
    Priority_Queue_Clear

    This function removes and releases all items from the Queue,
    while retaining the Queue itself for further operations.
    This is less harsh than a full Release(), in that the Queue
    is still usable following this function.

    Inputs:
    Queue   -   Pointer to the Priority_Queue_t to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure or error.
*/
int Priority_Queue_Clear(Priority_Queue_t* Queue);

/*
    Priority_Queue_Release

    This function removes and releases all items held by the Queue,
    as well as the Queue itself. Following a call to this function,
    the Queue is no longer valid for use.

    Inputs:
    Queue   -   Pointer to the Priority_Queue_t to operate on.

    Outputs:
    None, the queue and all resources held by the items it contained
    are fully released.
*/
void Priority_Queue_Release(Priority_Queue_t* Queue);

/* ---------- Public Priority_Queue_t Functions ---------- */
#endif

#ifdef LIBCONTAINER_ENABLE_STRING
/* ++++++++++ Public String_t Functions ++++++++++ */

/*
    String_Create

    This function creates and initializes a new String_t with the given character
    character sequence and Length. The Value can be NULL, in which case the
    String_t will simply be initialized in an empty state.

    Inputs:
    Value   -   Pointer to the character sequence to use as the initial value,
                    or NULL to initialize to an empty state.
    Length  -   The length of the initialization sequence. If 0 and Value
                    is Non-NULL, strlen() will be used to compute this value.

    Outputs:
    String_t*   -   Pointer to a fully prepared and ready-to-use String_t, or NULL on failure.

    Note:
    The difference between String_Create() and String_CreateConst() is that String_t values
    created with this function can be modified later. A String_t created with String_CreateConst()
    CANNOT be modified after it is initialized. Generally, this function will be used as
    the primary method of creating String_t values.
*/
String_t* String_Create(char* Value, size_t Length);

/*
    String_CreateConst

    This function creates and initializes an immutable new String_t value with
    the given character sequence. This function is intended to generally wrap
    String literals to allow using the String_t API on them. A String_t
    value created with this function does NOT own the memory associated with
    the char* it is given - it only contains the pointer.

    Inputs:
    Value   -   Pointer to a C-string to set the String_t value to.

    Outputs:
    String_t*   -   Pointer to a fully prepared and immutable String_t, or NULL on failure.

    Note:
    The difference between String_Create() and String_CreateConst() is that String_t values
    created with this function can be modified later. A String_t created with String_CreateConst()
    CANNOT be modified after it is initialized. Generally, this function will be used to wrap
    string literals to access the String_t API.
*/
String_t* String_CreateConst(char* Value);

/*
    String_Length

    This function returns the Length of the String_t. If a NULL string is provided,
    returns 0.

    Inputs:
    String  -   Pointer to the String_t to operate on.

    Outputs:
    size_t  -   The length of the String_t, as measured in bytes.

    Note:
    This operation is O(1) in the length of the string.
*/
size_t String_Length(String_t* String);

/*
    String_IsEmpty

    This function checks whether the String_t is empty or not.

    Inputs:
    String  -   Pointer to the String_t to operate on.

    Outputs:
    bool    -   Returns true if the String_t contains no data, false otherwise.

    Note:
    This operation is O(1) in the length of the string.
*/
bool String_IsEmpty(String_t* String);

/*
    String_GetAtIndex

    This function returns the character at the specified Index from within the String.
    This performs bounds-checking to ensure valid access.

    Inputs:
    String  -   Pointer to the String_t to operate on.
    Index   -   The 0-based index within the String_t of the character to return.

    Outputs:
    char    -   The character value at the specified index, or 0x00 on error.
*/
char String_GetAtIndex(String_t* String, size_t Index);

/*
    String_GetFront

    This function returns the character at the front of the String.

    Inputs:
    String  -   Pointer to the String_t to operate on.

    Outputs:
    char    -   The character value at the specified index, or 0x00 on error.
*/
char String_GetFront(String_t* String);

/*
    String_GetBack

    String_GetFront

    This function returns the character at the end of the String.

    Inputs:
    String  -   Pointer to the String_t to operate on.

    Outputs:
    char    -   The character value at the specified index, or 0x00 on error.
*/
char String_GetBack(String_t* String);

/*
    String_Insert

    This function inserts the given substring of length "Length" into
    the String_t, starting at the specified Index.

    Inputs:
    String      -   Pointer to the String_t to operate on.
    ToInsert    -   Pointer to the new C-String to insert into the String_t.
    Index       -   The 0-based index to begin writing into the String_t.
    Length      -   The length of the C-String passed to this function.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int String_Insert(String_t* String, char* ToInsert, size_t Index, size_t Length);

/*
    String_Prepend

    This function inserts the given substring of length "Length" into
    the String_t, starting at the front of the string.

    Inputs:
    String      -   Pointer to the String_t to operate on.
    ToInsert    -   Pointer to the new C-String to insert into the String_t.
    Length      -   The length of the C-String passed to this function.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int String_Prepend(String_t* String, char* ToInsert, size_t Length);

/*
    String_Append

    This function inserts the given substring of length "Length" into
    the String_t, starting at the front of the string.

    Inputs:
    String      -   Pointer to the String_t to operate on.
    ToInsert    -   Pointer to the new C-String to insert into the String_t.
    Length      -   The length of the C-String passed to this function.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int String_Append(String_t* String, char* ToInsert, size_t Length);

/*
    String_Replace

    This function inserts the given substring of length "Length" into
    the String_t, starting at the front of the string. This overwrites
    the existing contents of the String_t.

    Inputs:
    String      -   Pointer to the String_t to operate on.
    ToInsert    -   Pointer to the new C-String to insert into the String_t.
    Index       -   The 0-based index to begin writing into the String_t.
    Length      -   The length of the C-String passed to this function.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int String_Replace(String_t* String, char* ToInsert, size_t Index, size_t Length);

/*
    String_Set

    This function updates the String_t contents, setting the value to the
    newly provided C-string.

    Inputs:
    String      -   Pointer to the String_t to operate on.
    NewValue    -   Pointer to the new C-String value to set the String_t to.
    Length      -   The length of the C-String value passed to this function.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int String_Set(String_t* String, char* NewValue, size_t Length);

/*
    String_ToCString

    This function returns a C-String compatible pointer to the string
    contents it holds. This is asserted to be NUL-terminated and compatible
    with all standard library functions.

    Inputs:
    String  -   Pointer to the String_t to operate on.

    Outputs:
    char*   -   A pointer to the C-String compatible contents of the String_t.
                    NULL on failure.

    Note:
    This is an O(1) operation, and is reasonably efficient so caching the result is
    not necessary.
*/
char* String_ToCString(String_t* String);

/*
    String_Unwrap

    This function unwraps a String_t into a raw C-String value, transferring
    memory ownership to the caller. The String_t is released after being unwrapped.

    Inputs:
    String  -   Pointer to the String_t to operate on.

    Outputs:
    char*   -   A pointer to the C-String compatible contents of the String_t.
                    NULL on failure.

    Note:
    Memory ownership of the returned pointer is transferred to the caller.
    This must be released eventually with a call to free(), or else this will
    leak memory.
*/
char* String_Unwrap(String_t* String);

/*
    String_Copy

    This function creates an independent copy of the given String_t.

    Inputs:
    String  -   Pointer to the String_t to operate on.

    Outputs:
    String_t*   -   Newly created, independent String_t value containing a copy
                        of the original's contents.
*/
String_t* String_Copy(String_t* String);

/*
    String_Substring

    This function creates a new, independent copy of a section of a given String_t.

    Inputs:
    String  -   Pointer to the String_t to operate on.
    Index   -   The 0-based index to start the Substring from.
    Length  -   The length of the Substring to copy.

    Outputs:
    String_t*   -   Pointer to the newly created String_t containing the
                        requested substring, or NULL on error.

    Note:
    If the requested (Index, Length) pair would overrun the String_t, they will
    be truncated to the end of the String_t.
*/
String_t* String_Substring(String_t* String, size_t Index, size_t Length);

/*
    String_Compare

    This function compares two String_t values. Very similar to strcmp() and
    memcmp(), but with the added memory safety from the String_t API.

    Inputs:
    A       -   Pointer to the first String_t to operate on.
    B       -   Pointer to the second String_t to operate on.

    Outputs:
    Returns:
                Negative if A < B
                0        if A == B
                Positive if A > B

    Note:
    This function treates NULL String_t values as Greater than all legal values,
    as well as NULL == NULL returning 0.
*/
int String_Compare(String_t* A, String_t* B);

/*
    String_Clear

    This function clears the contents of the String_t, releasing
    the contents but retaining the container for later use.

    Inputs:
    String  -   Pointer to the String_t to operate on.

    Outputs:
    int     -   Returns 0 on success, non-zero on failure.
*/
int String_Clear(String_t* String);

/*
    String_Release

    This function will fully release the resources associated with the
    given String_t.

    Inputs:
    String  -   Pointer to the String_t to operate on.

    Outputs:
    None, the String_t is released safely.
*/
void String_Release(String_t* String);

/* ---------- Public String_t Functions ---------- */
#endif

/* ---------- Exported Library Functions ---------- */

#ifdef __cplusplus
}
#endif

#endif
