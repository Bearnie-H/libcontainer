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
#include <string.h>

#include "../logging/logging.h"
#include "include/stack.h"

static int StackCallbackFunc(void *Value) {

    if (NULL == Value) {
        DEBUG_PRINTF("%s", "NULL Value* Provided.");
        return 1;
    }

    DEBUG_PRINTF("Stack item has value: [ %d ].", *(int *)Value);
    return 0;
}

static int StackCallbackArgFunc(void *Value, void *Args) {

    if (NULL == Value) {
        DEBUG_PRINTF("%s", "NULL Value* Provided.");
        return 1;
    }

    if (NULL == Args) {
        DEBUG_PRINTF("%s", "NULL Args* provided.");
        DEBUG_PRINTF("Stack item has value: [ %d ].", *(int *)Value);
        return 0;
    }

    DEBUG_PRINTF("Stack item has value: [ %d ] (%s).", *(int *)Value, (char *)Args);
    return 0;
}

static int StackRefCallbackFunc(void *Value) {

    if (NULL == Value) {
        DEBUG_PRINTF("%s", "NULL Value* Provided.");
        return 1;
    }

    DEBUG_PRINTF("Stack item has value: [ %s ].", (char *)Value);
    return 0;
}

static int StackRefCallbackArgFunc(void *Value, void *Args) {

    if (NULL == Value) {
        DEBUG_PRINTF("%s", "NULL Value* Provided.");
        return 1;
    }

    if (NULL == Args) {
        DEBUG_PRINTF("%s", "NULL Args* provided.");
        DEBUG_PRINTF("Stack item has value: [ %s ].", (char *)Value);
        return 0;
    }

    DEBUG_PRINTF("Stack item has value: [ %s ] (%s).", (char *)Value, (char *)Args);
    return 0;
}

int Test_stack(void) {

    int FailedTests = 0;

    FailedTests += Test_Stack_Create();
    FailedTests += Test_Stack_RefCreate();
    FailedTests += Test_Stack_Length();
    FailedTests += Test_Stack_RefLength();
    FailedTests += Test_Stack_IsEmpty();
    FailedTests += Test_Stack_RefIsEmpty();
    FailedTests += Test_Stack_Push();
    FailedTests += Test_Stack_RefPush();
    FailedTests += Test_Stack_Peek();
    FailedTests += Test_Stack_RefPeek();
    FailedTests += Test_Stack_Pop();
    FailedTests += Test_Stack_RefPop();
    FailedTests += Test_Stack_DoCallback();
    FailedTests += Test_Stack_DoCallbackArg();
    FailedTests += Test_Stack_RefDoCallback();
    FailedTests += Test_Stack_RefDoCallbackArg();
    FailedTests += Test_Stack_Clear();
    FailedTests += Test_Stack_RefClear();

    return FailedTests;
}

int Test_Stack_Create(void) {

    Stack_t *Stack = NULL;
    int Value = 0;

    Stack = Stack_Create(sizeof(Value), NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_RefCreate(void) {

    Stack_t *Stack = NULL;

    Stack = Stack_Create(0, NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_Length(void) {

    Stack_t *Stack = NULL;
    int Value = 0;

    Stack = Stack_Create(sizeof(Value), NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    if (0 != Stack_Length(Stack)) {
        TEST_PRINTF("%s", "Test Failure - Length of empty Stack not equal to 0.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (0 != Stack_Push(Stack, &Value)) {
        TEST_PRINTF("%s", "Test Failure - Failed to push new Value onto Stack.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (0 == Stack_Length(Stack)) {
        TEST_PRINTF("%s",
                    "Test Failure - Length of Stack equal to 0 after successful Push() operation.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_RefLength(void) {

    Stack_t *Stack = NULL;
    const char Value[] = "Test Value";
    char *ValueContents = NULL;

    Stack = Stack_Create(0, NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    ValueContents = (char *)calloc(1, sizeof(Value));
    if (NULL == ValueContents) {
        TEST_PRINTF("%s", "Test Failure - Failed to allocate memory for reference Item.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (0 != Stack_Length(Stack)) {
        TEST_PRINTF("%s", "Test Failure - Length of empty Stack not equal to 0.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (0 != Stack_Push(Stack, ValueContents)) {
        TEST_PRINTF("%s", "Test Failure - Failed to push new Value onto Stack.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (0 == Stack_Length(Stack)) {
        TEST_PRINTF("%s",
                    "Test Failure - Length of Stack equal to 0 after successful Push() operation.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_IsEmpty(void) {

    Stack_t *Stack = NULL;
    int Value = 0;

    Stack = Stack_Create(sizeof(Value), NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    if (!Stack_IsEmpty(Stack)) {
        TEST_PRINTF("%s", "Test Failure - New Stack not reporting empty.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_RefIsEmpty(void) {

    Stack_t *Stack = NULL;

    Stack = Stack_Create(0, NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    if (!Stack_IsEmpty(Stack)) {
        TEST_PRINTF("%s", "Test Failure - New Stack not reporting empty.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_Push(void) {

    Stack_t *Stack = NULL;
    int Value = 0;

    Stack = Stack_Create(sizeof(Value), NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    if (!Stack_IsEmpty(Stack)) {
        TEST_PRINTF("%s", "Test Failure - New Stack not reporting empty.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (0 != Stack_Push(Stack, &Value)) {
        TEST_PRINTF("%s", "Test Failure - Failed to push new Value onto Stack.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (Stack_IsEmpty(Stack)) {
        TEST_PRINTF("%s",
                    "Test Failure - Stack reporting empty after successful Push() operation.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_RefPush(void) {

    Stack_t *Stack = NULL;
    const char Value[] = "Test Value";
    char *ValueContents = NULL;

    Stack = Stack_Create(0, NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    if (!Stack_IsEmpty(Stack)) {
        TEST_PRINTF("%s", "Test Failure - New Stack not reporting empty.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    ValueContents = (char *)calloc(1, sizeof(Value));
    if (NULL == ValueContents) {
        TEST_PRINTF("%s", "Test Failure - Failed to allocate memory for reference Item.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (0 != Stack_Push(Stack, ValueContents)) {
        TEST_PRINTF("%s", "Test Failure - Failed to push new Value onto Stack.");
        Stack_Release(Stack);
        free(ValueContents);
        TEST_FAILURE;
    }

    if (Stack_IsEmpty(Stack)) {
        TEST_PRINTF("%s",
                    "Test Failure - Stack reporting empty after successful Push() operation.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_Peek(void) {

    Stack_t *Stack = NULL;
    int Value = 0xBEEF, *PeekValue = NULL;

    Stack = Stack_Create(sizeof(Value), NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    if (0 != Stack_Push(Stack, &Value)) {
        TEST_PRINTF("%s", "Test Failure - Failed to push new Value onto Stack.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    PeekValue = (int *)Stack_Peek(Stack);
    if (NULL == PeekValue) {
        TEST_PRINTF("%s", "Test Failure - Failed to Peek() item at the top of the Stack.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (Value != *PeekValue) {
        TEST_PRINTF("Test Failure - Item Peek()'d (%d) does not have expected value (%d).",
                    *PeekValue, Value);
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_RefPeek(void) {

    Stack_t *Stack = NULL;
    const char Value[] = "Test Value";
    char *ValueContents = NULL, *PeekContents = NULL;

    Stack = Stack_Create(0, NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    ValueContents = (char *)calloc(1, sizeof(Value));
    if (NULL == ValueContents) {
        TEST_PRINTF("%s", "Test Failure - Failed to allocate memory for reference Item.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (0 != Stack_Push(Stack, ValueContents)) {
        TEST_PRINTF("%s", "Test Failure - Failed to push new Value onto Stack.");
        Stack_Release(Stack);
        free(ValueContents);
        TEST_FAILURE;
    }

    PeekContents = (char *)Stack_Peek(Stack);
    if (NULL == PeekContents) {
        TEST_PRINTF("%s", "Test Failure - Failed to Peek() item at the top of the Stack.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (0 != memcmp(PeekContents, ValueContents, sizeof(Value))) {
        TEST_PRINTF("Test Failure - Item Peek()'d (%s) does not have expected value (%s).",
                    PeekContents, ValueContents);
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_Pop(void) {

    Stack_t *Stack = NULL;
    int Value = 0xBEEF, *PopValue = NULL;

    Stack = Stack_Create(sizeof(Value), NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    if (0 != Stack_Push(Stack, &Value)) {
        TEST_PRINTF("%s", "Test Failure - Failed to push new Value onto Stack.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    PopValue = (int *)Stack_Pop(Stack);
    if (NULL == PopValue) {
        TEST_PRINTF("%s", "Test Failure - Failed to Pop() item at the top of the Stack.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (Value != *PopValue) {
        TEST_PRINTF("Test Failure - Item Pop()'d (%d) does not have expected value (%d).",
                    *PopValue, Value);
        Stack_Release(Stack);
        free(PopValue);
        TEST_FAILURE;
    }

    if (0 != Stack_Length(Stack)) {
        TEST_PRINTF("%s", "Test Failure - Stack Length not 0 after Pop().");
        Stack_Release(Stack);
        free(PopValue);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    free(PopValue);
    TEST_SUCCESSFUL;
}

int Test_Stack_RefPop(void) {

    Stack_t *Stack = NULL;
    const char Value[] = "Test Value";
    char *ValueContents = NULL, *PopContents = NULL;

    Stack = Stack_Create(0, NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    ValueContents = (char *)calloc(1, sizeof(Value));
    if (NULL == ValueContents) {
        TEST_PRINTF("%s", "Test Failure - Failed to allocate memory for reference Item.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (0 != Stack_Push(Stack, ValueContents)) {
        TEST_PRINTF("%s", "Test Failure - Failed to push new Value onto Stack.");
        Stack_Release(Stack);
        free(ValueContents);
        TEST_FAILURE;
    }

    PopContents = (char *)Stack_Pop(Stack);
    if (NULL == PopContents) {
        TEST_PRINTF("%s", "Test Failure - Failed to Pop() item at the top of the Stack.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (0 != memcmp(PopContents, ValueContents, sizeof(Value))) {
        TEST_PRINTF("Test Failure - Item Pop()'d (%s) does not have expected value (%s).",
                    PopContents, ValueContents);
        Stack_Release(Stack);
        free(PopContents);
        TEST_FAILURE;
    }

    if (0 != Stack_Length(Stack)) {
        TEST_PRINTF("%s", "Test Failure - Stack Length not 0 after Pop().");
        Stack_Release(Stack);
        free(PopContents);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    free(PopContents);
    TEST_SUCCESSFUL;
}

int Test_Stack_DoCallback(void) {

    Stack_t *Stack = NULL;
    int Count = 16, i = 0;

    Stack = Stack_Create(sizeof(Count), NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != Stack_Push(Stack, &i)) {
            TEST_PRINTF("%s", "Test Failure - Failed to Push() value onto Stack.");
            Stack_Release(Stack);
            TEST_FAILURE;
        }
    }

    if (0 != Stack_DoCallback(Stack, StackCallbackFunc)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Callback on Stack contents.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_DoCallbackArg(void) {

    Stack_t *Stack = NULL;
    int Count = 16, i = 0;
    char CallbackArg[] = "Callback Arguments";

    Stack = Stack_Create(sizeof(Count), NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != Stack_Push(Stack, &i)) {
            TEST_PRINTF("%s", "Test Failure - Failed to Push() value onto Stack.");
            Stack_Release(Stack);
            TEST_FAILURE;
        }
    }

    if (0 != Stack_DoCallbackArg(Stack, StackCallbackArgFunc, (void *)CallbackArg)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Callback on Stack contents.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_RefDoCallback(void) {

    Stack_t *Stack = NULL;
    char *TempValue = NULL;
    int Count = 16, i = 0;
    size_t BufferLength = 32;

    Stack = Stack_Create(0, NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {

        TempValue = calloc(BufferLength, sizeof(char));
        if (NULL == TempValue) {
            TEST_PRINTF("%s", "Test Failure - Failed to allocate memory for reference item.");
            Stack_Release(Stack);
            TEST_FAILURE;
        }

        snprintf(TempValue, BufferLength, "Stack Value [ %d ].", i);

        if (0 != Stack_Push(Stack, TempValue)) {
            TEST_PRINTF("%s", "Test Failure - Failed to Push() value to Stack.");
            free(TempValue);
            Stack_Release(Stack);
            TEST_FAILURE;
        }
    }

    if (0 != Stack_DoCallback(Stack, StackRefCallbackFunc)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Callback on Stack contents.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_RefDoCallbackArg(void) {

    Stack_t *Stack = NULL;
    char *TempValue = NULL;
    char CallbackArg[] = "Callback Argument";
    int Count = 16, i = 0;
    size_t BufferLength = 32;

    Stack = Stack_Create(0, NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {

        TempValue = calloc(BufferLength, sizeof(char));
        if (NULL == TempValue) {
            TEST_PRINTF("%s", "Test Failure - Failed to allocate memory for reference item.");
            Stack_Release(Stack);
            TEST_FAILURE;
        }

        snprintf(TempValue, BufferLength, "Stack Value [ %d ].", i);

        if (0 != Stack_Push(Stack, TempValue)) {
            TEST_PRINTF("%s", "Test Failure - Failed to Push() value to Stack.");
            free(TempValue);
            Stack_Release(Stack);
            TEST_FAILURE;
        }
    }

    if (0 != Stack_DoCallbackArg(Stack, StackRefCallbackArgFunc, (void *)CallbackArg)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Callback on Stack contents.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_Clear(void) {

    Stack_t *Stack = NULL;
    int Count = 16, i = 0;

    Stack = Stack_Create(sizeof(Count), NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {
        if (0 != Stack_Push(Stack, &i)) {
            TEST_PRINTF("%s", "Test Failure - Failed to Push() value onto Stack.");
            Stack_Release(Stack);
            TEST_FAILURE;
        }
    }

    if (0 != Stack_Clear(Stack)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Clear() on Stack.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (!Stack_IsEmpty(Stack)) {
        TEST_PRINTF("%s", "Test Failure - Stack reporting non-empty after Clear().");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}

int Test_Stack_RefClear(void) {

    Stack_t *Stack = NULL;
    char *TempValue = NULL;
    int Count = 16, i = 0;
    size_t BufferLength = 32;

    Stack = Stack_Create(0, NULL);
    if (NULL == Stack) {
        TEST_PRINTF("%s", "Test Failure - Failed to create Stack_t for testing.");
        TEST_FAILURE;
    }

    for (i = 0; i < Count; i++) {

        TempValue = calloc(BufferLength, sizeof(char));
        if (NULL == TempValue) {
            TEST_PRINTF("%s", "Test Failure - Failed to allocate memory for reference item.");
            Stack_Release(Stack);
            TEST_FAILURE;
        }

        snprintf(TempValue, BufferLength, "Stack Value [ %d ].", i);

        if (0 != Stack_Push(Stack, TempValue)) {
            TEST_PRINTF("%s", "Test Failure - Failed to Push() value to Stack.");
            free(TempValue);
            Stack_Release(Stack);
            TEST_FAILURE;
        }
    }

    if (0 != Stack_Clear(Stack)) {
        TEST_PRINTF("%s", "Test Failure - Failed to perform Clear() on Stack.");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    if (!Stack_IsEmpty(Stack)) {
        TEST_PRINTF("%s", "Test Failure - Stack reporting non-empty after Clear().");
        Stack_Release(Stack);
        TEST_FAILURE;
    }

    Stack_Release(Stack);
    TEST_SUCCESSFUL;
}
