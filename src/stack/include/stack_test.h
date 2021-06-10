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

#ifndef LIBCONTAINER_STACK_TEST_H
#define LIBCONTAINER_STACK_TEST_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern "C" {
#endif

/*
    Top-level testing entry-point for the component.
*/
int Test_stack(void);

int Test_Stack_Create(void);
int Test_Stack_RefCreate(void);
int Test_Stack_Length(void);
int Test_Stack_RefLength(void);
int Test_Stack_IsEmpty(void);
int Test_Stack_RefIsEmpty(void);
int Test_Stack_Push(void);
int Test_Stack_RefPush(void);
int Test_Stack_Peek(void);
int Test_Stack_RefPeek(void);
int Test_Stack_Pop(void);
int Test_Stack_RefPop(void);
int Test_Stack_DoCallback(void);
int Test_Stack_DoCallbackArg(void);
int Test_Stack_RefDoCallback(void);
int Test_Stack_RefDoCallbackArg(void);
int Test_Stack_Clear(void);
int Test_Stack_RefClear(void);

#ifdef __cplusplus
}
#endif

#endif
