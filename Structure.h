#ifndef Structure_H // Unique name for this header
#define Structure_H

//////////////////////////////////////////////////////////////////
//                  STACK STRUCTURE OPERATIONS
//////////////////////////////////////////////////////////////////
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
void PushReturnAddress(int address);
int PopReturnAddress();


///////////////////////////////////////////////////////////
//                 FUNCTION STRUCTURE
///////////////////////////////////////////////////////////
typedef struct {
    char *funcName;
    int startLine;
} Function;

extern Function functionList[20];
extern int functionCount;

int findFunctionLine(char *funcName); // Returns the line number where the specified function starts
void FunctionsManitoring(); // Prints the list of all recorded functions with their line numbers
void FunctionListing(); // Scans the code lines to find and record all function names and their starting line numbers for later use


//////////////////////////////////////////////////////////////////
//                  LABELS STRUCTURE OPERATIONS
//////////////////////////////////////////////////////////////////
int findLabelLine(char *labelName);
void LabelsManitoring();

typedef struct
{
    char *labelName;
    int labelLine;
} Label;

extern Label labelsList[10];
extern int labelsCount;

#endif