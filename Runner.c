#include "CommonTools.h"
#include "Hardware.h"
#include "Structure.h"
#include "interpreter.h"
#include "GlobalVariables.h"
#include "Runner.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static int currentLine = 0;
FILE *file;

CommandStrcuture lines[100];
int linesCount = 0;

int main(int argc, char const *argv[])
{
    InitializeHardWare();
    FileReading();
    LabelListing();
    FunctionListing();
    Init_Data_Structures();

    for (currentLine = 0; currentLine < linesCount; currentLine++)
        RunPussembler(lines[currentLine].tokens);

    printf("\n");
    savedStringManitoring();
    printf("\n");
    RegistersManitoring();
    printf("\n");
    StackManitoring();
    printf("\n");
    LabelsManitoring();
    printf("\n");
    RamManitoring();
}

//  Open and read all the codes inside the target file
void FileReading()
{
    file = fopen("input.txt", "r");

    if (file == NULL)
        DebugLog("The target file to read the code couldn't be read");

    for (size_t i = 0; i < 100; i++)
    {
        linesCount = i + 1;
        lines[i].tokens = malloc(MAX_Command_Tokens * sizeof(char *));

        int canContinue = InputReciver(lines[i].tokens, MAX_Command_Tokens);
        if (canContinue == -1)
            break;
    }
    fclose(file);
}

/// @brief User input
/// @returns 0:  Continue getting input,  -1: End of input
/// @warning The 'file' that we'll read the code from it, should be open first
int InputReciver(char **buffer, int count)
{
    for (size_t i = 0; i < count; i++)
        buffer[i] = malloc(8);

    //  Skip the line if it's a comment line with symbol '$'
    int ignoreLine = 0;

    //  When we want to save constant data like strings
    int savingVarible = 0, savingVarible_name = 0;
    char *stringVar, *stringName;
    int stringVarIndex = 0, stringNameIndex = 0;

    int charIndex = 0;
    int tokenIndex = 0;
    char c;
    while ((c = fgetc(file)) != '\n' && c != EOF)
    {
        //  Ignore and pass the lines that contain Comment in the code
        if (c == '$')
            ignoreLine = 1;

        if (ignoreLine)
            continue;

        if (!savingVarible && StrEqul_with_length(buffer[0], ".", 1))
        {
            savingVarible = 1;
            stringVar = malloc(100 * sizeof(char));
            savingVarible_name = 1;
            stringName = malloc(64 * sizeof(char));
        }

        if (savingVarible)
        {
            if (savingVarible_name){
                if(c == '>'){
                    stringName[stringNameIndex++] = '\0';
                    savingVarible_name = 0;
                }
                else
                    stringName[stringNameIndex++] = c;
            }
            else
                stringVar[stringVarIndex++] = c;

            continue;
        }

        //  Handle multi space input, and treat like it's just one space
        //  So the command "LOAD   R2", is the same as "LOAD R2", unles we're saving a string
        if (c == ' ' && !savingVarible)
        {
            buffer[tokenIndex++][charIndex] = '\0';
            while ((c = fgetc(file)) != '\n')
                if (c != ' ')
                    break;

            charIndex = 0;
        }
        if (c == '\n')
            break;

        buffer[tokenIndex][charIndex++] = c;
    }
    buffer[tokenIndex][charIndex] = '\0';

    if (savingVarible)
    {
        stringVar[stringVarIndex] = '\0';
        buffer[0][0] = '\0';
        saveConstString(stringName, stringVar);
    }

    //  When we hit the end of the code, when we're reading the codes from a file
    if (c == EOF)
        return -1;

    return 0;
}

//  Run the codes command line-by-line, [not sure about the Pussembler Spell (:]
//  Run the codes, [not sure about the Pussembler Spell (:] {that's correct dude :))}
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

    // Adds value of RegisterB to RegisterA → Result saved in RegisterA
    else if (StrEqul(tokens[0], "ADD"))
    {
        int A = atoi(tokens[1] + 1);
        int B = atoi(tokens[2] + 1);
        Add_Registers(A, B);
    }

    // Subtracts value of RegisterB from RegisterA → Result saved in RegisterA
    else if (StrEqul(tokens[0], "SUB"))
    {
        int A = atoi(tokens[1] + 1);
        int B = atoi(tokens[2] + 1);
        Subtract_Registers(A, B);
    }

    // Multiplies RegisterA by RegisterB → Result saved in RegisterA
    else if (StrEqul(tokens[0], "MUL"))
    {
        int A = atoi(tokens[1] + 1);
        int B = atoi(tokens[2] + 1);
        Multiply_Registers(A, B);
    }

    // Divides RegisterA by RegisterB → Result saved in RegisterA
    else if (StrEqul(tokens[0], "DIV"))
    {
        int A = atoi(tokens[1] + 1);
        int B = atoi(tokens[2] + 1);
        Divide_Registers(A, B);
    }

    // Divides RegisterA by RegisterB → Result saved in RegisterA
    else if (StrEqul(tokens[0], "MOD"))
    {
        int A = atoi(tokens[1] + 1);
        int B = atoi(tokens[2] + 1);
        ModuloRegisters(A, B);
    }

    // Outputs the value of a Register or a RAM cell
    else if (StrEqul(tokens[0], "OUT"))
    {
        // Printing string from RAM or Saved strings
        if (StrEqul(tokens[1], "-s"))
        {
            //  Reading string from RAM starts with #
            if (tokens[2][0] == '#')
            {
                int stringLocation = atoi(tokens[2] + 1);
                printf(">>%s\n", ReadStringFromRam(stringLocation));
            }
            //  Reading string from RAM starts with .
            else if (tokens[2][0] == '.')
            {
                char *stringVaribleName = tokens[2] + 1;
                printf(">>%s\n", get_saved_string(stringVaribleName));
            }
        }
        else // Printing non-string value from Registers/RAM
            printf("%d\n", ValueParser(tokens[1]));
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

    else if (StrEqul(tokens[0], "GOTO"))
    {
        currentLine = findLabelLine(tokens[1]);
    }

    //  'if' programs
    else if (StrEqul(tokens[0], "IF"))
    {
        int result = 0;

        //  Tow int value we want to compare toghater
        int value1 = ValueParser(tokens[1]);
        int value2 = ValueParser(tokens[3]);

        if (StrEqul(tokens[2], "=="))
            result = value1 == value2;
        else if (StrEqul(tokens[2], "!="))
            result = value1 != value2;
        else if (StrEqul(tokens[2], "<"))
            result = value1 < value2;
        else if (StrEqul(tokens[2], "<="))
            result = value1 <= value2;
        else if (StrEqul(tokens[2], ">"))
            result = value1 > value2;
        else if (StrEqul(tokens[2], ">="))
            result = value1 >= value2;
        else
        {
            printf("Warning: Command or condition didn't match any known case!\n");
            return;
        }

        //  The target label/function, we want to jump if the condition was true
        char *label = tokens[5];

        if (!result)
            return;
        if (StrEqul(tokens[4], "GOTO"))
            currentLine = findLabelLine(label);
        else if (StrEqul(tokens[4], "CALL"))
            currentLine = findFunctionLine(label);
    }
    else if (StrEqul(tokens[0], "INP"))
    {
        //  Write the input string into the RAM
        if (tokens[2][0] == '#')
        {
            int inputLength = atoi(tokens[1]);
            int ramlocation = atoi(tokens[2] + 1);

            //  Includeing the '\0' symbol to save at the end
            char *string = malloc((1 + inputLength) * sizeof(char));

            printf("\n>");

            //  Includeing the '\n' too
            fgets(string, inputLength + 1, stdin);

            //  Remove the new-line('\n') symbol from the end of the input
            if (string[strlen(string) - 1] == '\n')
                string[strlen(string) - 1] = '\0';

            WriteStringIntoRam(ramlocation, string);
        }
    }

    // Go to the funtion decelared using 'FUNC'
    else if (StrEqul(tokens[0], "CALL"))
    {
        int returnAddress = currentLine; // Store the address of the next line after CALL to return to it later
        if (!Push_ReturnAddress(returnAddress))
        {
            printf("Error: Stack overflow while saving return address!\n");
            return;
        }

        currentLine = findFunctionLine(tokens[1]);
    }

    else if (StrEqul(tokens[0], "RET"))
    {
        if (Return_Stack_IsEmpty())
        {
            printf("Error: RETURN used without a CALL!\n");
            return;
        }

        int returnLine = Pop_ReturnAddress() + 1;
        currentLine = returnLine;
    }

    else if (StrEqul(tokens[0], "FUNC"))
    {
        // Skip all lines until we reach RET
        while (!StrEqul(lines[currentLine].tokens[0], "RET"))
            currentLine++;
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
        return Stack_Pop();
    }
    else if (token[0] == '[')
    {
        //  To be completed like this:
        //  #[R2]   --> reading from RAM at the location of the secound register's number
        //  #[#24]  --> reading from RAM at the location of the RAM's 24th number
        //  #[#[R3]]--> ram location of ( ram's value at ( R3 value ) )
    }
    else
    {
        return atoi(token);
    }
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
