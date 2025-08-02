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