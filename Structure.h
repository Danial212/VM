#ifndef Structure_H // Unique name for this header
#define Structure_H
#include <stdio.h>
#include "Runner.h"

int Init_Data_Structures();

//////////////////////////////////////////////////////////////////
//                  STACK STRUCTURE OPERATIONS
//////////////////////////////////////////////////////////////////
static int Stack_Initialize();
int Stack_length();
int Stack_IsEmpty();
int Stack_hasSpace();
int Stack_Push(int value);
int Stack_Pop();
void StackManitoring();

extern const int copacity;
extern const int startLocation;

//////////////////////////////////////////////////////////////////
//                  RETURN STACK OPERATIONS
//////////////////////////////////////////////////////////////////
int Push_ReturnAddress(int address);
int Pop_ReturnAddress();
int Return_Stack_IsEmpty();
void ReturnStackManitoring();

///////////////////////////////////////////////////////////
//                 FUNCTION STRUCTURE
///////////////////////////////////////////////////////////
typedef struct
{
    char *funcName;
    int startLine;
} Function;

extern Function functionList[20];
extern int functionCount;

int findFunctionLine(char *funcName); // Returns the line number where the specified function starts
void FunctionsManitoring();           // Prints the list of all recorded functions with their line numbers
void FunctionListing();               // Scans the code lines to find and record all function names and their starting line numbers for later use

//////////////////////////////////////////////////////////////////
//                  LABELS STRUCTURE OPERATIONS
//////////////////////////////////////////////////////////////////
void LabelListing();
int findLabelLine(char *labelName);
void LabelsManitoring();

typedef struct
{
    char *labelName;
    int labelLine;
} Label;

extern Label labelsList[10];
extern int labelsCount;

//////////////////////////////////////////////////////////////////
//                  CONSTANT STRING STRUCTURE
//////////////////////////////////////////////////////////////////
typedef struct
{
    char *name;
    char *string;
} ConstString;

extern ConstString stringList[100];
extern int stringListCount;

void saveConstString(char *name, char *str);
void savedStringManitoring();
char *get_saved_string(char *name);

//////////////////////////////////////////////////////////////////
//                  File Management operations
//////////////////////////////////////////////////////////////////
typedef struct
{
    char *name;
    FILE *file;
    int offset;
} FileStruct;

extern FileStruct filesList[100];
extern int filesListCount;

int addFileToList(char *name);
FILE *openFileFromList(char *name);
void closeFileFromList(char *name);
void filesList_Manitoring();
FileStruct *findFileFromList(char *name);

//                      PCB Stack Structure
//////////////////////////////////////////////////////////////////

extern PCB PCB_Stack[100];
extern int PCB_StackIndex;

int PCB_stack_push(PCB pcb);
int PCB_stack_isEmpty();
PCB PCB_stack_pop();
void PCB_stack_Manitoring();

#endif
