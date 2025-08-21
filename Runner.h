#ifndef RUNNER_H // Unique name for this Runner.h
#define RUNNER_H
#include <stdio.h>

int InputReciver(char **, int, FILE *);
void RunPussembler(char **);
int ValueParser(char *token);
int *GetTargetStoragePointer(char *token);
void FileReading(char *fileNmae);
void EnableShell();

typedef struct
{
    char **tokens;
} CommandStrcuture;

typedef struct
{
    CommandStrcuture lines[100];
    char *blockName;
    int linesCount;
    int currentLine;
    struct PCB *childNode;
} PCB;

extern PCB currentProcess;

#endif
