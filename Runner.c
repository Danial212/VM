#include <stdio.h>
#include <stdint.h>
#include "interpreter.c"
#include <string.h>
#include <stdlib.h>

void InputReciver(char **, int);
int StrEqul(char *, char *);
void RunPussembler(char **);
int ValueParser(char *token);
void DebugLog(const char *__format, ...);

const int DEBUG_LOG = 1;

int main(int argc, char const *argv[])
{

    InitializeHardWare();

    char **tokens = malloc(4 * sizeof(char *));

    while (1)
    {
        InputReciver(tokens, 4);

        RunPussembler(tokens);
    }
}

void InputReciver(char **buffer, int count)
{
    for (size_t i = 0; i < count; i++)
        buffer[i] = malloc(8);

    int charIndex = 0;
    int tokenIndex = 0;
    char c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        //  Handle multi space input, and treat like it's just one space
        //  So the command 'LOAD   R2', is the same as 'LOAD R2'
        if (c == ' ')
        {
            buffer[tokenIndex][charIndex] = '\0';
            tokenIndex++;
            while ((c = getchar()) != '\n')
                if (c != ' ')
                    break;

            charIndex = 0;
        }

        buffer[tokenIndex][charIndex++] = c;
    }
    buffer[tokenIndex][charIndex] = '\0';
}

int StrEqul(char *str1, char *str2)
{
    return stricmp(str1, str2) == 0;
}

//  Run the codes, [now sure about the Pussembler Spell (:]
void RunPussembler(char **tokens)
{
    if (StrEqul(tokens[0], "LOAD"))
    {
        //  The data we want to write into RAM/Register
        //  It could be from any source, like RAM, Register or directly thorugh user input
        int data = ValueParser(tokens[2]);

        //  Writing data into Register
        if (tokens[1][0] == 'R')
        {
            int destinationRegister = atoi(tokens[1] + 1);
            if (destinationRegister > REGISTER_COUNT)
                DebugLog("Out of Register Acsess Error.\nMax Register Count is %d", REGISTER_COUNT);

            LoadImmediateIntoRegister(destinationRegister, data);
        }
        //  Writing data into RAM
        else if (tokens[1][0] == '#')
        {
            int destinationRAM = atoi(tokens[1] + 1);
            if (destinationRAM > RAM_SPACE)
                DebugLog("Out of RAM Acsess Error.\nRAM's space size is %d", destinationRAM);
            WriteIntoRam(destinationRAM, data);
        }
    }
}

//  Extract the data from the given token.
//  If it's in a Register-Address-Form ---> Extract data from the Register
//  If it's in a      RAM-Address-Form ---> Extract data from the RAM
//  If it's not in the mentioned forms ---> Extract data from the user input
int ValueParser(char *token)
{
    if (token[0] == 'R')
    {
        int destinationRegister = atoi(token + 1);
        int data = ReadDataFromRegister(destinationRegister);
        return data;
    }
    else if (token[0] == '#')
    {
        int RamLocation = atoi(token + 1);
        int data = ReadFromRAM(RamLocation);
        return data;
    }
    else
        return atoi(token);
}

void DebugLog(const char *__format, ...)
{
    if (DEBUG_LOG)
        printf(__format);
}