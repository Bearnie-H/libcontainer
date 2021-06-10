/*
    Copyright (c) 2021 Bearnie-H

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
*/

#include <stdio.h>

#define LIBCONTAINER_ENABLE_ARRAY
#define LIBCONTAINER_ENABLE_LIST
#define LIBCONTAINER_ENABLE_HASHMAP
#define LIBCONTAINER_ENABLE_BINARY_TREE

#include "../include/libcontainer.h"
#include "array/include/array.h"
#include "hashmap/include/hashmap.h"
#include "list/include/list.h"
#include "stack/include/stack.h"
#include "tree/binary-tree/include/binary_tree.h"

int Test_main(int argc, char **argv) {

    int i;

    /* Placeholder code here to do *something* outwardly visible,
        and to suppress the 'w-unused-parameter' compiler warning for this
       function. */
    printf("Executing test suite for: %s", argv[0]);
    for (i = 1; i < argc; i++) {
        printf(" %s", argv[i]);
    }
    printf("\n");

    /* Implement the testing suite which has been put together for this project.
    Typically, this should be simply composed of the component-level testing
    entry points generated by the "NewComponent.sh" script. */
    int FailedTests = 0;

    FailedTests += Test_array();
    FailedTests += Test_list();
    FailedTests += Test_hashmap();
    FailedTests += Test_binary_tree();
    FailedTests += Test_stack();

    /* Returning non-zero indicates to the Makefile during make to abort.
        This lets failing tests block further builds with the "all" target
        until the tests pass. */

    if (0 == FailedTests) {
        printf("All tests passed!\n");
    } else {
        printf("Error: %d tests failed!\n", FailedTests);
    }
    return FailedTests;
}
