#include <string.h>
#include "interpreter.c"
#include "Structure.c"

int InputReciver(char **, int);
int StrEqul(char *, char *);
void RunPussembler(char **);
int ValueParser(char *token);
void DebugLog(const char *__format, ...);
int *GetTargetStoragePointer(char *token);
void LabelListing();
void Labels_manitoring();
void FileReading();
int findLabelLine(char *labelName);

const int DEBUG_LOG = 1;
static int currentLine = 0;

typedef struct
{
    char **tokens;
} CommandStrcuture;

typedef struct
{
    char *labelName;
    int labelLine;
} Label;

static CommandStrcuture lines[100];
static int linesCount = 0;
static Label labelsList[10];
static int labelsCount = 0;

FILE *file;

int main(int argc, char const *argv[])
{
    InitializeHardWare();
    FileReading();
    LabelListing();

    for (currentLine = 0; currentLine < linesCount; currentLine++)
        RunPussembler(lines[currentLine].tokens);

    RegistersManitoring();
    Stack_Manitoring();
    Labels_manitoring();
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
        lines[i].tokens = malloc(4 * sizeof(char *));
        
        int canContinue = InputReciver(lines[i].tokens, 4);
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

    int ignoreLine = 0;
    int charIndex = 0;
    int tokenIndex = 0;
    char c;
    while ((c = fgetc(file)) != '\n' && c != EOF)
    {
        //  ognore and pass the lines that contain Comment in the code
        if (c == '#')
            ignoreLine = 1;
        if (ignoreLine)
            continue;

        //  Handle multi space input, and treat like it's just one space
        //  So the command "LOAD   R2", is the same as "LOAD R2"
        if (c == ' ')
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

    //  When we hit the end of the code, when we're reading the codes from a file
    if (c == EOF)
        return -1;

    return 0;
}

//  Check if 2 string are equal, case in-setsitive
int StrEqul(char *str1, char *str2)
{
    return stricmp(str1, str2) == 0;
}


//  List all the label, sith their name and line number, for later jumping actions
void LabelListing()
{
    for (size_t i = 0; i < linesCount; i++)
    {
        if (StrEqul(lines[i].tokens[0], "LABEL"))
        {
            labelsList[labelsCount].labelLine = i;
            labelsList[labelsCount].labelName = lines[i].tokens[1];
            labelsCount++;
        }
    }
}

//  Run the codes command line-by-line, [not sure about the Pussembler Spell (:]
//  Run the codes, [not sure about the Pussembler Spell (:] {that's correct dude :))}

void RunPussembler(char **tokens)
{
    if (StrEqul(tokens[0], "//"))
    {
        //  DO NOTHING
    }
    else if (StrEqul(tokens[0], "LOAD"))
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
    else if (StrEqul(tokens[0], "Label"))
    {
        //  DO NOTHING
    }
    else if (StrEqul(tokens[0], "IF"))
    {
        //  IF statement block
    }
    else if (StrEqul(tokens[0], "ELSE"))
    {
        //  ELSE statement block
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

//  Print all the labels in our code
void Labels_manitoring()
{
    for (size_t i = 0; i < labelsCount; i++)
    {
        printf("Label <%s> at the line #%d", labelsList[i].labelName, labelsList[i].labelLine + 1);
    }
}

//  Find the line number of the given label
int findLabelLine(char *labelName)
{
    for (size_t i = 0; i <= labelsCount; i++)
        if (StrEqul(labelName, labelsList[i].labelName))
            return labelsList[i].labelLine;

    DebugLog("The target label <%s> cound't be found", labelName);
    return -1;
}