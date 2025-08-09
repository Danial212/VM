#ifndef RUNNER_H // Unique name for this Runner.h
#define RUNNER_H

int InputReciver(char **, int);
void RunPussembler(char **);
int ValueParser(char *token);
int *GetTargetStoragePointer(char *token);
void FileReading();

typedef struct
{
    char **tokens;
} CommandStrcuture;

extern CommandStrcuture lines[100];
extern int linesCount;

#endif
