#include <string.h>
#include "interpreter.c"
#include "Structure.c"

void InputReciver(char **, int);
int StrEqul(char *, char *);
void RunPussembler(char **);
int ValueParser(char *token);
void DebugLog(const char *__format, ...);
int *GetTargetStoragePointer(char *token);

const int DEBUG_LOG = 1;

int main(int argc, char const *argv[])
{
    InitializeHardWare();

    char **tokens = malloc(4 * sizeof(char *));

    while (1)
    {
        InputReciver(tokens, 4);

        RunPussembler(tokens);

        RegistersManitoring();
        Stack_Manitoring();
    }
}

//  Getting User input 
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

//  if 2 string are equal, case insetsitive
int StrEqul(char *str1, char *str2)
{
    return stricmp(str1, str2) == 0;
}

//  Run the codes, [not sure about the Pussembler Spell (:]
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
    else if (StrEqul(tokens[0], "ADD"))
    {
        /* Addition Operartion Code */
    }
    else if (StrEqul(tokens[0], "SUB"))
    {
        /* Subtraction Operartion Code */
    }
    else if (StrEqul(tokens[0], "MUL"))
    {
        /* Multiplication Operartion Code */
    }
    else if (StrEqul(tokens[0], "DIV"))
    {
        /* division Operartion Code */
    }
    else if (StrEqul(tokens[0], "MOD"))
    {
        /* Modulo Operartion Code */
    }
    else if (StrEqul(tokens[0], "OUT"))
    {
        /* write Output
            it should handle RAM and Register
            */
    }
    else if (StrEqul(tokens[0], "PUSH"))
    {
        int value = ValueParser(tokens[1]);
        Stack_Push(value);
    }
    else if (StrEqul(tokens[0], "POP"))
    {
        int *targetStorage = GetTargetStoragePointer(tokens[1]);
        *targetStorage = Stack_Pop();
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
    else if (StrEqul(token, "POP"))
    {
        if (!Stack_IsEmpty())
        {
            int data = Stack_Pop();
            return data;
        }
        else
            /// STACK EMPTY ERROR
            return -1;
    }
    else
        return atoi(token);
}

// Enable In-Programm debuging, if the 'DEBUG_LOG' is True(1)
void DebugLog(const char *__format, ...)
{
    if (DEBUG_LOG)
        printf(__format);
}

//  Get Register/RAM pointer
int *GetTargetStoragePointer(char *token)
{
    if (token[0] == 'R')
    {
        int destinationRegister = atoi(token + 1);
        int *reg_pointer = ReadRegisterPointer(destinationRegister);
        return reg_pointer;
    }
    else if (token[0] == '#')
    {
        int RamLocation = atoi(token + 1);
        int *ram_poiner = GetRamPointer(RamLocation);
        return ram_poiner;
    }
    return NULL;
}