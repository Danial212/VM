#include "Structure.h"
#include "Hardware.h"
#include "CommonTools.h" 
#include "GlobalVariables.h"
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////
//                  STACK STRUCTURE OPERATIONS
//////////////////////////////////////////////////////////////////

//  Saving The Stack into the Ram, starts at the 'startLocation' location of the RAM
//  and Ends at the 'startLocation' + 'copacity'

const int copacity = 24;
const int startLocation = 1000;

int stackIndex = 0;

int Stack_Initialize()
{
    //  'copacity' + 'startLocation' must be less than RAM's size
    if (startLocation + copacity > RAM_SIZE)
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

//  Printing the states of the stack
void StackManitoring()
{
    printf("########################## STACK ###########################\n|");

    for (size_t i = 0; i < Stack_length(); i++)
    {
        int number = ReadFromRAM(startLocation + i);
        int spaceLangth = 5 - int_length(number);
        int L1 = spaceLangth / 2;
        int L2 = spaceLangth - L1;
        for (size_t j = 0; j < L1; j++)
            printf(" ");
        printf("%d", number);
        for (size_t j = 0; j < L2; j++)
            printf(" ");

        if ((i + 1) % 10 == 0)
            printf("\n");

        printf("|");
    }

    printf("\n------------------------------------------------------------\n");
}

//////////////////////////////////////////////////////////////////
//                  LABELS STRUCTURE OPERATIONS
//////////////////////////////////////////////////////////////////

Label labelsList[10];
int labelsCount = 0;

//  Find the line number of the given label
int findLabelLine(char *labelName)
{
    for (size_t i = 0; i <= labelsCount; i++)
        if (StrEqul(labelName, labelsList[i].labelName))
            return labelsList[i].labelLine;

    DebugLog("The target label <%s> cound't be found\n", labelName);
    return -1;
}

//  Print all the labels in our code
void LabelsManitoring()
{
    for (size_t i = 0; i < labelsCount; i++)
        printf("Label <%s> at the line #%d", labelsList[i].labelName, labelsList[i].labelLine + 1);
}
