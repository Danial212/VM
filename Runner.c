#include "CommonTools.h"
#include "Hardware.h"
#include "Structure.h"
#include "interpreter.h"
#include "GlobalVariables.h"
#include "FileSystem.h"
#include "Runner.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

FILE *file;

PCB currentProcess;

int main(int argc, char const *argv[])
{
    InitializeHardWare();
    currentProcess.blockName = "Init Process";

    FileReading("calc.txt");
    LabelListing();
    FunctionListing();
    Init_Data_Structures();

    while (1)
    {
        for (; currentProcess.currentLine < currentProcess.linesCount; currentProcess.currentLine++)
            RunPussembler(currentProcess.lines[currentProcess.currentLine].tokens);
        if (!PCB_stack_isEmpty())
            currentProcess = PCB_stack_pop();
        else
            break;
    }

    // Manitoring data structures, hardware, ...
    // printf("\n");
    // FunctionsManitoring();
    // printf("\n");
    // savedStringManitoring();
    // printf("\n");
    // RegistersManitoring();
    // printf("\n");
    // StackManitoring();
    // printf("\n");
    // LabelsManitoring();
    // printf("\n");
    // RamManitoring();
}


//  Open and read all the codes inside the target file
void FileReading(char *fileName)
{
    file = fopen(fileName, "r");

    if (file == NULL)
        DebugLog("The target file to read the code couldn't be read");

    int FileLines = 100;
    for (size_t i = 0; i < FileLines; i++)
    {
        currentProcess.linesCount = i + 1;
        currentProcess.lines[i].tokens = malloc(MAX_Command_Tokens * sizeof(char *));

        int canContinue = InputReciver(currentProcess.lines[i].tokens, MAX_Command_Tokens, file);
        if (canContinue == -1)
            break;
        
        if ((i+1) >= FileLines) FileLines = FileLines*2;
    }
    fclose(file);
}

void EnableShell()
{
    while (1)
    {
        currentProcess.lines[currentProcess.linesCount].tokens = malloc(MAX_Command_Tokens * sizeof(char *));
        InputReciver(currentProcess.lines[currentProcess.linesCount].tokens, MAX_Command_Tokens, stdin);
        RunPussembler(currentProcess.lines[currentProcess.linesCount].tokens);
        currentProcess.linesCount++;
    }
}

/// @returns 0:  Continue getting input,  -1: End of input
/// @warning If you're reading from a 'file', it should be opened first
int InputReciver(char **buffer, int count, FILE *inputStream)
{
    for (size_t i = 0; i < count; i++)
        buffer[i] = malloc(BUFFER_SIZE);

    //  Skip the line if it's a comment line with symbol '$'
    int ignoreLine = 0;

    int hasBegeningSpace = 0;

    //  When we want to save constant data like strings
    int savingVarible = 0, savingVarible_name = 0;
    char *stringVar, *stringName;
    int stringVarIndex = 0, stringNameIndex = 0;

    int charIndex = 0;
    int tokenIndex = 0;
    char c;

    while ((c = fgetc(inputStream)) != '\n' && c != EOF)
    {
        //  Ignore and pass the lines that contain Comment in the code
        if (c == '$')
            ignoreLine = 1;

        if (ignoreLine)
            continue;

        if (!savingVarible && StrEqul_with_length(buffer[0], ".", 1))
        {
            savingVarible = True;
            stringVar = malloc(128 * sizeof(char));
            savingVarible_name = True;
            stringName = malloc(64 * sizeof(char));
        }

        if (savingVarible)
        {
            if (savingVarible_name)
            {
                if (c == '>') //    Start Scanning constant string
                {
                    stringName[stringNameIndex++] = '\0';
                    savingVarible_name = False;
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
        if (c == ' ')
        {
            if (tokenIndex == 0 && charIndex == 0)
            {
                while ((c = fgetc(inputStream)) != '\n' && c != EOF)
                    if (c != ' ')
                        break;
            }
            else
            {
                if (!(tokenIndex == 0 && charIndex == 0))
                    buffer[tokenIndex][charIndex] = '\0';
                while ((c = fgetc(inputStream)) != '\n' && c != EOF)
                    if (c != ' ')
                    {
                        tokenIndex++;
                        break;
                    }
            }
            if (c == '$')
            {
                ignoreLine = True;
                continue;
            }

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
        int *targetStorage = GetTargetStoragePointer(tokens[1]);
        int data = ValueParser(tokens[2]);

        *targetStorage = data;
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
            //  Reading address from RAM  [...]
            else if (tokens[2][0] == '[')
            {
                int stringLocation = ValueParser(tokens[2]);
                printf("%d>>%s\n", stringLocation, ReadStringFromRam(stringLocation));
            }
            //  Reading string from RAM starts with .
            else if (tokens[2][0] == '.')
            {
                int insertNewLine = 0;
                char *stringVaribleName;

                if (tokens[2][1] == '.')
                {
                    insertNewLine = 1;
                    stringVaribleName = tokens[2] + 2;
                }
                else
                    stringVaribleName = tokens[2] + 1;
                printf(">>%s", get_saved_string(stringVaribleName));

                if (insertNewLine)
                    printf("\n");
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
        currentProcess.currentLine = findLabelLine(tokens[1]);
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
            currentProcess.currentLine = findLabelLine(label);
        else if (StrEqul(tokens[4], "CALL"))
        {
            currentProcess.currentLine = findFunctionLine(label);
        }
        else if (StrEqul(tokens[4], "RET"))
        {
            int returnLine = Pop_ReturnAddress();
            currentProcess.currentLine = returnLine;
        }
    }

    else if (StrEqul(tokens[0], "INP"))
    {
        //  Write the input string into the RAM
        if (tokens[2][0] == '#' || tokens[2][0] == '[')
        {
            int inputLength = atoi(tokens[1]);
            int ramlocation = -1;
            if (tokens[2][0] == '[')
                ramlocation = ValueParser(slice_string(tokens[2], 1, -1));
            else
                ramlocation = atoi(tokens[2] + 1);

            //  Including the '\0' symbol to save at the end
            char *string = malloc((1 + inputLength) * sizeof(char));

            printf("\n>");

            char c;
            int index = 0;
            while ((c = getchar()) != '\n' && c != EOF)
                if (index < inputLength)
                    string[index++] = c;

            string[index] = '\0';

            WriteStringIntoRam(ramlocation, string);
        }
        //  Getting integer input from user
        else
        {
            int *targetStorage = GetTargetStoragePointer(tokens[1]);
            //  Includeing the '\0' symbol to save at the end
            char *number = malloc((10) * sizeof(char));
            printf("\n>");
            fgets(number, 10, stdin);
            *targetStorage = atoi(number);
        }
    }

    else if (StrEqul(tokens[0], "FUNC"))
    {
        // Skip all lines until we reach ENDF
        while (!StrEqul(currentProcess.lines[currentProcess.currentLine].tokens[0], "ENDF"))
            currentProcess.currentLine++;
    }

    // Go to the funtion decelared using 'FUNC'
    else if (StrEqul(tokens[0], "CALL"))
    {
        int returnAddress = currentProcess.currentLine; // Store the address of the next line after CALL to return to it later

        if (!Push_ReturnAddress(returnAddress))
        {
            printf("Error: Stack overflow while saving return address!\n");
            return;
        }
        currentProcess.currentLine = findFunctionLine(tokens[1]);
    }

    else if (StrEqul(tokens[0], "RET"))
    {
        if (Return_Stack_IsEmpty())
        {
            printf("Error: RETURN used without a CALL!\n");
            return;
        }

        int returnLine = Pop_ReturnAddress();
        currentProcess.currentLine = returnLine;
    }

    else if (StrEqul(tokens[0], "FOPEN"))
    {
        addFileToList(tokens[1]);
        if (openFileFromList(tokens[1]) == NULL)
        {
            printf("You must add file to list to open it");
        }
    }
    else if (StrEqul(tokens[0], "FCLOSE"))
    {
        closeFileFromList(tokens[1]);
    }
    else if (StrEqul(tokens[0], "F_FLU"))
    {
        FileStruct *fileStruct = findFileFromList(tokens[1]);
        fflush(fileStruct->file);
    }
    else if (StrEqul(tokens[0], "F_REA"))
    {
        FileStruct *fileStruct = findFileFromList(tokens[1]);
        int *targetStorage = GetTargetStoragePointer(tokens[2]);
        if (targetStorage == NULL)
            printf("There's problem in tokenizing address");
        *targetStorage = fgetc(fileStruct->file);
    }
    else if (StrEqul(tokens[0], "F_WRI"))
    {
        FileStruct *fileStruct = findFileFromList(tokens[1]);
        char c = ValueParser(tokens[2]);
        fputc(c, fileStruct->file);
    }
    else if (StrEqul(tokens[0], "F_GOTO"))
    {
        FileStruct *fileStruct = findFileFromList(tokens[1]);
        char *offsetToken = tokens[2];

        if (offsetToken[0] == '+' || offsetToken[0] == '-')
        {
            int offset = atoi(offsetToken + 1);
            if (offsetToken[0] == '-')
                offset = -offset;
            printf("Moving %d\n", offset);
            fseek(fileStruct->file, offset, SEEK_CUR);
        }
        else
        {
            int offset = atoi(offsetToken);
            fseek(fileStruct->file, offset, SEEK_SET);
        }
    }
    else if (StrEqul(tokens[0], "FRUN"))
    {
        PCB pcb = {
            .blockName = tokens[1],
            .childNode = NULL,
            .currentLine = 0,
        };

        //  Continue from the line after the line we jump out of the instructions
        currentProcess.currentLine++;
        //  Initiliza the current node's child process
        currentProcess.childNode = &pcb;
        //  Saving the current process into PCB list
        PCB_stack_push(currentProcess);
        //  Switching to our target process
        currentProcess = pcb;
        //  Loading the pussembly instructions from the file into our current PCB
        FileReading(tokens[1]);
    }
    else
        return;
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
        return ReadFromRAM(ValueParser(slice_string(token, 1, -1)));
    }
    else
    {
        // Handle negative numbers (e.g., "-123") and regular integers
        if (token[0] == '-')
        {
            return -1 * atoi(token + 1); // Convert remaining digits and negate
        }
        return atoi(token); // Positive number or 0
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
    else if (token[0] == '[')
        return GetRamPointer(ValueParser(slice_string(token, 1, -1)));
    return NULL;
}
