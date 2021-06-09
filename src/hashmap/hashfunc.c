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

#include <stdlib.h>
#include <string.h>

#define LIBCONTAINER_ENABLE_HASHMAP

#include "../../include/libcontainer.h"
#include "../logging/logging.h"

unsigned int HashFunc_Int(const void *Key, size_t KeySize) {

    unsigned int HashValue = 0;

    if (NULL == Key) {
        DEBUG_PRINTF("%s", "Error, NULL Key* provided!");
        return 0;
    }

    if (0 == KeySize) {
        DEBUG_PRINTF("%s", "Note: KeySize of 0 provided.");
    }

    HashValue = *(unsigned int *)(Key);
    HashValue ^= HashValue >> 13;
    HashValue *= 0x5bd1e995;
    HashValue ^= HashValue >> 15;

    return HashValue;
}

unsigned int HashFunc_Long(const void *Key, size_t KeySize) {

    long KeyValue = 0;
    unsigned int HashValue = 0;

    if (NULL == Key) {
        DEBUG_PRINTF("%s", "Error, NULL Key* provided!");
        return 0;
    }

    if (0 == KeySize) {
        DEBUG_PRINTF("%s", "Note: KeySize of 0 provided.");
    }

    KeyValue = *(long *)Key;
    HashValue = (unsigned int)((KeyValue * (long)0xCAFECAFE) % (long)0xDEADBEEF);

    return HashValue;
}

unsigned int HashFunc_Double(const void *Key, size_t KeySize) {

    double KeyValue = 0.0f;
    unsigned int HashValue = 0;

    if (NULL == Key) {
        DEBUG_PRINTF("%s", "Error, NULL Key* provided!");
        return 0;
    }

    if (0 == KeySize) {
        DEBUG_PRINTF("%s", "Note: KeySize of 0 provided.");
    }

    KeyValue = *(double *)Key;
    HashValue = (unsigned int)KeyValue;
    HashValue *= 0xCAFEBEEF;

    return HashValue;
}

unsigned int HashFunc_String(const void *Key, size_t KeySize) {

    char *KeyValue = NULL;
    unsigned int HashValue = 1;
    size_t Index = 0;

    if (NULL == Key) {
        DEBUG_PRINTF("%s", "Error: NULL Key* provided!");
        return 0;
    }

    KeyValue = (char *)Key;

    if (0 == KeySize) {
        DEBUG_PRINTF(
            "%s", "Warning: KeySize of 0 provided; using strlen() to compute key string length.");
        KeySize = strlen(KeyValue);
    }

    for (Index = 0; Index < KeySize; Index++) {
        HashValue = (HashValue << 5) + HashValue + (unsigned int)KeyValue[Index];
    }

    return HashValue;
}
