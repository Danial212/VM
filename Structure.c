#include "Structure.h"
#include "Hardware.h"
#include "CommonTools.h"
#include "GlobalVariables.h"
#include "Runner.h"
#include <string.h>
#include <stdlib.h>

int Init_Data_Structures()
{
    Stack_Initialize();
}

//                  STACK STRUCTURE OPERATIONS
//////////////////////////////////////////////////////////////////

//  Saving The Stack into the Ram, starts at the 'startLocation' location of the RAM
//  and Ends at the 'startLocation' + 'copacity'

const int copacity = 96;
const int startLocation = 4000;

int stackIndex = 0;

static int Stack_Initialize()
{
    //  'copacity' + 'startLocation' must be less than RAM's size
    if (startLocation + copacity > RAM_SIZE)
    {
        printf("E.96\nStack Over RAM");
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
        printf("|");
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

//                  RETURN STACK OPERATIONS
//////////////////////////////////////////////////////////////////

int returnStack[100];
int returnStackIndex = 0;

/// @param address The current line you're calling a function
/// @return 0 --> StackOverFlow, 1 --> Success
int Push_ReturnAddress(int address)
{
    if (returnStackIndex >= 100)
        return 0;
    returnStack[returnStackIndex++] = address;
    return 1;
}

int Return_Stack_IsEmpty()
{
    return (returnStackIndex == 0);
}

int Pop_ReturnAddress()
{
    // if (returnStackIndex == 0) return -1;
    if (returnStackIndex - 1 < 0){

        printf("Error: Stack is empty");
        exit(EXIT_FAILURE);
    }

    return returnStack[--returnStackIndex];
}

void ReturnStackManitoring()
{

    printf("####################### RETURN STACK #######################\n|");

    for (size_t i = 0; i < returnStackIndex; i++)
    {
        int number = returnStack[i];
        int spaceLangth = 5 - int_length(number);
        int L1 = spaceLangth / 2;
        int L2 = spaceLangth - L1;
        printf("|");
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

//                 FUNCTION STRUCTURE
///////////////////////////////////////////////////////////
Function functionList[20];
int functionCount = 0;

int findFunctionLine(char *funcName)
{
    for (int i = 0; i < functionCount; i++)
    {
        if (StrEqul(funcName, functionList[i].funcName))
            return functionList[i].startLine;
    }

    DebugLog("Function %s not found", funcName);
    return -1;
}

void FunctionListing()
{
    for (int i = 0; i < currentProcess.linesCount; i++)
    {
        if (StrEqul(currentProcess.lines[i].tokens[0], "FUNC"))
        {
            functionList[functionCount].funcName = currentProcess.lines[i].tokens[1];
            functionList[functionCount].startLine = i;
            functionCount++;
        }
    }
}

void FunctionsManitoring()
{
    if (functionCount == 0)
        printf("No function found in yor program\n");
    else
        for (size_t i = 0; i < functionCount; i++)
            printf("%d) FUNC <%s> at line .%d\n", i + 1, functionList[i].funcName, functionList[i].startLine);
}

//                  LABELS STRUCTURE OPERATIONS
//////////////////////////////////////////////////////////////////

Label labelsList[10];
int labelsCount = 0;

//  List all the label, sith their name and line number, for later jumping actions
void LabelListing()
{
    for (size_t i = 0; i < currentProcess.linesCount; i++)
    {
        if (StrEqul(currentProcess.lines[i].tokens[0], "LABEL"))
        {
            labelsList[labelsCount].labelLine = i;
            labelsList[labelsCount].labelName = currentProcess.lines[i].tokens[1];
            labelsCount++;
        }
    }
}

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

//                  CONSTANT STRING STRUCTURE
//////////////////////////////////////////////////////////////////

ConstString stringList[100];
int stringListCount = 0;

void saveConstString(char *name, char *str)
{
    stringList[stringListCount].name = malloc(strlen(name + 2));
    stringList[stringListCount].name = name;
    stringList[stringListCount].string = malloc(strlen(str + 2));
    stringList[stringListCount].string = str;

    stringListCount++;
}

void savedStringManitoring()
{
    printf("#################### String Varibles #######################\n");
    for (size_t i = 0; i < stringListCount; i++)
        printf("%d)%s--> %s\n", i + 1, stringList[i].name, stringList[i].string);
}

char *get_saved_string(char *name)
{
    for (size_t i = 0; i < stringListCount; i++)
        if (StrEqul(name, stringList[i].name))
            return stringList[i].string;
    return NULL;
}

//                  File Management operations
//////////////////////////////////////////////////////////////////

FileStruct filesList[100];
int filesListCount = 0;

int addFileToList(char *name)
{
    filesList[filesListCount].name = malloc(strlen(name + 2));
    strcpy(filesList[filesListCount].name, name);
    filesList[filesListCount].file = NULL;
    filesList[filesListCount].offset = 0;
    filesListCount++;
    return 1;
}
FILE *openFileFromList(char *name)
{
    FILE *file = fopen(name, "r+");

    if (file == NULL)
        //  You add the file to list, the file doesn't exist in diretory
        return NULL;

    findFileFromList(name)->file = file;

    return file;
}

void closeFileFromList(char *name)
{
    FileStruct *fileStruct = findFileFromList(name);
    fclose(fileStruct->file);
    fileStruct->file = NULL;
}

void filesList_Manitoring()
{
    for (size_t i = 0; i < filesListCount; i++)
        printf("%d)%s [%s]\n", i + 1, filesList[i].name, (filesList[i].file == NULL) ? "Closed" : "Opened");
    printf("---------------------------------\n");
}

FileStruct *findFileFromList(char *name)
{
    for (size_t i = 0; i < filesListCount; i++)
        if (StrEqul(filesList[i].name, name))
            return (filesList + i);

    //  Coudln't find the specific file, you must forget to add it to the list,
    //  Or it doesn't exist at all
    DebugLog("Error: File %s not found\n", name);
    return NULL;
}

//                      PCB Stack Structure
//////////////////////////////////////////////////////////////////
PCB PCB_Stack[100];
int PCB_StackIndex = 0;

/// @return 0 --> StackOverFlow, 1 --> Success
int PCB_stack_push(PCB targetPCB)
{
    printf("----------------------------------\n");
    printf("name:%s, line:%d, line count:%d, \n ", targetPCB.blockName, targetPCB.currentLine, targetPCB.linesCount);
    printf("----------------------------------\n");
    if (PCB_StackIndex >= 100){
        DebugLog("Error: PCB_Stack overflow");
        return 0;
    }
    PCB_Stack[PCB_StackIndex++] = targetPCB;
    return 1;
}

int PCB_stack_isEmpty()
{
    return (PCB_StackIndex == 0);
}

PCB PCB_stack_pop()
{
    if (PCB_stack_isEmpty){

        DebugLog("Error: Stack is empty");
        exit(EXIT_FAILURE);
    }
    return PCB_Stack[--PCB_StackIndex];
}

void PCB_stack_Manitoring()
{
    printf("####################### PCB STACK #######################\n|");
    for (size_t i = 0; i < PCB_StackIndex; i++)
    {
        char *name = PCB_Stack[i].blockName;
        int number = i + 1;
        int size = int_length(number) + 2 + strlen(name);

        printf("%d) %s", number, name);
        for (size_t j = 0; j < (20-size); j++)
            printf(" ");

        if (number % 5 == 0){
            
            printf("\n");
        }
        printf("|");
    }

    printf("\n------------------------------------------------------------\n");
}
