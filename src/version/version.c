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

#include "../../include/libcontainer.h"

#ifndef LIBCONTAINER_MAJOR_VERSION
#define LIBCONTAINER_MAJOR_VERSION 0
#endif

#ifndef LIBCONTAINER_MINOR_VERSION
#define LIBCONTAINER_MINOR_VERSION 0
#endif

#ifndef LIBCONTAINER_PATCH_VERSION
#define LIBCONTAINER_PATCH_VERSION 0
#endif

#ifndef LIBCONTAINER_BUILD_DATE
#define LIBCONTAINER_BUILD_DATE "Unknown"
#endif

#ifndef LIBCONTAINER_BUILD_DATESTAMP
#define LIBCONTAINER_BUILD_DATESTAMP 0
#endif

void Print_Libcontainer_Library_Version(void) {

    printf("libcontainer - A C library providing generic container structs.\n"
           "Version: %02d.%02d.%02d\n"
           "Compiled on: %s\n",
           LIBCONTAINER_MAJOR_VERSION, LIBCONTAINER_MINOR_VERSION, LIBCONTAINER_PATCH_VERSION,
           LIBCONTAINER_BUILD_DATE);

    return;
}

long Libcontainer_Library_Version(void) {

    long FormattedVersionNumber = 0;
    char VersionString[7] = {0x00};

    sprintf(VersionString, "%02d%02d%02d", LIBCONTAINER_MAJOR_VERSION, LIBCONTAINER_MINOR_VERSION,
            LIBCONTAINER_PATCH_VERSION);

    FormattedVersionNumber = atol(VersionString);

    return FormattedVersionNumber;
}

long Libcontainer_Build_Time(void) { return (long)LIBCONTAINER_BUILD_DATESTAMP; }
