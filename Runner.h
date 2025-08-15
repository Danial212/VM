#ifndef RUNNER_H // Unique name for this Runner.h
#define RUNNER_H
#include <stdio.h>

int InputReciver(char **, int, FILE *);
void RunPussembler(char **);
int ValueParser(char *token);
int *GetTargetStoragePointer(char *token);
void FileReading();
void EnableShell();

typedef struct
{
    char **tokens;
} CommandStrcuture;

extern CommandStrcuture lines[100];
extern int linesCount;

#endif
