#include <stdio.h>
#include <stdlib.h>
#include "Hardware.c"
#include "string.h"

void operate(char *c1, char *c2, char *c3, char *c4);
void runCLI(char *input);

Register _reg;

/// @param _registerNumber The Data of the register with the given number
int getRegisterData(int _registerNumber)
{
    switch (_registerNumber)
    {
    case 1:
        return _reg.R1;
        break;
    case 2:
        return _reg.R2;
        break;
    case 3:
        return _reg.R3;
        break;
    case 4:
        return _reg.R4;
        break;
    }
    return -999;
}

void MOV(int regNumber, int data)
{
    switch (regNumber)
    {
    case 1:
        _reg.R1 = data;
        break;
    case 2:
        _reg.R2 = data;
        break;
    case 3:
        _reg.R3 = data;
        break;
    case 4:
        _reg.R4 = data;
        break;
    }
}

//  Register Number 1  1 + 2 = 3
void ADD(int opRegN1, int opRegN2, int opRegN3)
{
    //  Data 1 + data2 = register3
    int data = getRegisterData(opRegN1) + getRegisterData(opRegN2);

    switch (opRegN3)
    {
    case 1:
        _reg.R1 = data;
        break;
    case 2:
        _reg.R2 = data;
        break;
    case 3:
        _reg.R3 = data;
        break;
    case 4:
        _reg.R4 = data;
        break;
    }
}

int main()
{
    while (1)
    {
        char *input = malloc(32 * sizeof(char));
        
        gets(input);
        runCLI(input);
    }

    // runCLI("MOV R1 15");
    // runCLI("MOV R2 10");
    // runCLI("ADD R1 R2 R3");
    // runCLI("PRI R3");

    // operate("MOV", "R1", "15", "");
    // operate("MOV", "R2", "11", "");
    // operate("ADD", "R1", "R2", "R3");
    // operate("PRI", "R3", "", "");
}

void runCLI(char *input)
{
    int maxOpcodeSize = 8 * sizeof(char);
    char *c1 = malloc(maxOpcodeSize), *c2 = malloc(maxOpcodeSize), *c3 = malloc(maxOpcodeSize), *c4 = malloc(maxOpcodeSize);
    int l1 = 0, l2 = 0, l3 = 0, l4 = 0;

    int inputLength = strlen(input);
    int currentToken = 1;

    for (int i = 0; i < inputLength; i++)
    {
        char c = *(input + i);

        if (c == ' ')
        {
            currentToken++;
        }
        else if (c == '\n' || c == EOF)
            continue;
        else
        {
            switch (currentToken)
            {
            case 1:
                *(c1 + l1) = c;
                l1++;
                *(c1 + l1) = '\0';
                break;
            case 2:
                *(c2 + l2++) = c;
                *(c2 + l2) = '\0';
                break;
            case 3:
                *(c3 + l3++) = c;
                *(c3 + l3) = '\0';
                break;
            case 4:
                *(c4 + l4++) = c;
                *(c4 + l4) = '\0';
                break;
            }
        }
    }

    operate(c1, c2, c3, c4);
}

void operate(char *c1, char *c2, char *c3, char *c4)
{
    if (strcmp(c1, "MOV") == 0)
    {
        int _regNumber = c2[1] - '0';
        int data = atoi(c3);
        MOV(_regNumber, data);
    }
    else if (strcmp(c1, "ADD") == 0)
    {
        int _regNumber = c2[1] - '0';
        int __regNumber = c3[1] - '0';
        int ___regNumber = c4[1] - '0';
        ADD(_regNumber, __regNumber, ___regNumber);
    }
    else if (strcmp(c1, "PRI") == 0)
    {
        int registerNumber = c2[1] - '0';
        printf(">%d\n", getRegisterData(registerNumber));
    }
}
