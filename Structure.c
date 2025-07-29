#include <stdio.h>
#include <stdlib.h>
#include "interpreter.c"

//////////////////////////////////////////////////////////////////
//                  STACK STRUCTURE OPERATIONS                   
//////////////////////////////////////////////////////////////////

//  Saving The Stack into the Ram, starts at the 'startLocation' location of the RAM
//  and Ends at the 'startLocation' + 'copacity'
int Stack_length();
int Stack_IsEmpty();
int Stack_hasSpace();
int Stack_Push(int value);
int Stack_Pop();

const int copacity = 24;
const int startLocation = 1000;
static int stackIndex = 0;

int Stack_Initialize()
{
    //  'copacity' + 'startLocation' must be less than RAM's size
    if (startLocation + copacity > RAM_SPACE)
    {
        printf("E.24\nStack Over RAM");
        exit(EXIT_FAILURE);
    }
}

//  Push the given value into the stack
//  returns: 1 --> successfully Added, 0 --> StackOverFlow Error
int Stack_Push(int value)
{
    if (Stack_hasSpace())
    {
        WriteIntoRam(startLocation + stackIndex, value);
        stackIndex++;
        return 1;
    }
    return 0;
}

//  Retrive last value from stack, and return it
int Stack_Pop()
{
    if (!Stack_IsEmpty())
    {
        stackIndex--;
        return ReadFromRAM(startLocation + stackIndex);
    }
    return -9999;
}

//  Check if the stack is totaly empty
int Stack_IsEmpty()
{
    return (stackIndex == 0);
}

//  Check if the stack has empty space to hold new values
int Stack_hasSpace()
{
    return stackIndex < copacity;
}

//  The length of the occupied locations in stack
int Stack_length()
{
    return stackIndex;
}
