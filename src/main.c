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

/* Definition of the entry point for the testing suite. */
int Test_main(int argc, char **argv);

int main(int argc, char **argv) {

    int i;

#if defined(TESTING)
    /* If the executable is built in Test-mode, call the main testing function
     * and exit. */
    return Test_main(argc, argv);

#elif defined(DEBUGGER)
    /* If the executable is built in Debugger-mode, call the main testing
     * function and continue with normal operation. */
    Test_main(argc, argv);

#endif

    /* Otherwise, do the standard application logic... */

    /* Placeholder code here to do *something* outwardly visible,
        and to suppress the 'w-unused-parameter' compiler warning for this
       function. */
    printf("Executing: %s", argv[0]);
    for (i = 1; i < argc; i++) {
        printf(" %s", argv[i]);
    }
    printf("\n");

    /* ... */

    return 0;
}
