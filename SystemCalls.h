#ifndef SYSTEMCALLS_H // Unique name for this header
#define SYSTEMCALLS_H

void handleSystemCalls();

//  File Operations System Calls
void createFile_SystemCall();
void deleteFile_SystemCall();
void readFile_SystemCall();
void writeFile_SystemCall();
void listFile_SystemCall();
void fileExist_SystemCall();
void renameFile_SystemCall();
void copyFile_SystemCall();

void clearScreen_SystemCall();
void exisProgram_SystemCall();

void version_SystemCall();

void runFile_SystemCall();


char *fileNameFromRegister();

#endif