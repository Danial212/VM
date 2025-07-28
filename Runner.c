#include <stdio.h>
#include <stdint.h>
#include "interpreter.c"
#include <string.h>
#include <stdlib.h>

void InputReciver(char **, int);
int StrEqul(char *, char *);
void RunPussembler(char **);

int main(int argc, char const *argv[])
{
    InitializeHardWare();

    char **tokens = malloc(4 * sizeof(char *));
    InputReciver(tokens, 4);

    RunPussembler(tokens);
}

void InputReciver(char **buffer, int count)
{
    for (size_t i = 0; i < count; i++)
    {
        buffer[i] = malloc(16);
        scanf("%s", buffer[i]);
    }
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
        if (tokens[1][0] == 'R')
        {
            int destinationRegister = atoi(tokens[1] + 1);
            printf("Loadin int to %dth Reg", destinationRegister);

            WriteIntoRegister(destinationRegister, tokens[2]);
        }
    }
}