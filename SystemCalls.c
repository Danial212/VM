#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "SystemCalls.h"
#include "CommonTools.h"
#include "FileSystem.h"
#include "Structure.h"
#include "Runner.h"
#include "interpreter.h"
#include "Hardware.h"

void handleSystemCalls()
{
    // tok[0]   tok[1]
    //  LOAD R1 1
    //  SYSCALL
    int callCode = ReadDataFromRegister(1);

    switch (callCode)
    {
    case 1:
        createFile_SystemCall();
        break;
    case 2:
        deleteFile_SystemCall();
        break;
    case 3:
        readFile_SystemCall();
        break;
    case 4:
        writeFile_SystemCall();
        break;
    case 5:
        listFile_SystemCall();
        break;
    case 6:
        fileExist_SystemCall();
        break;
    case 7:
        renameFile_SystemCall();
        break;
    case 9:
        copyFile_SystemCall();
        break;
    case 11:
        clearScreen_SystemCall();
        break;
    case 31:
        version_SystemCall();
        break;
    case 41:
        runFile_SystemCall();
        break;

    default:
    printf("None of supported System Calls\n");
        break;
    }
}

//  File name --> R2
//  File size --> R3
void createFile_SystemCall()
{
    //  Get file name string location in RAM, from R2 register
    char *fileName = fileNameFromRegister();
    //  Get file's size from R3 register
    int size = ReadDataFromRegister(3);

    int result = createFile(fileName, size);
    LoadImmediateIntoRegister(1, result);
}

//  File name --> R2
void deleteFile_SystemCall()
{
    char *fileName = fileNameFromRegister();
    int result = deleteFile(fileName);
    LoadImmediateIntoRegister(1, result);
}

//  File name --> R2
//  string address to write into RAM --> R3
void readFile_SystemCall()
{
    //  Reads the string from Disk based on the address in 3th register ...
    char *fileName = fileNameFromRegister();
    int bufferAddress = ReadDataFromRegister(3);
    Node *file = getFileTable(fileName);
    int size = file->data.size;

    //  And save it into the RAM
    for (size_t i = 0; i < size; i++)
        WriteIntoRam(bufferAddress + i, getDiskChar(file->data.location + i));

    WriteIntoRam(bufferAddress + size, '\0');

    int result = bufferAddress + size <= RAM_SIZE;
    LoadImmediateIntoRegister(1, result);
}

//  File name --> R2
//  string address to read from RAM --> R3
void writeFile_SystemCall()
{
    char *fileName = fileNameFromRegister();
    //  Reads the string from RAM based on the pointer in 3th register ...
    int bufferAddress = ReadDataFromRegister(3);
    char *content = ReadStringFromRam(bufferAddress);

    printf("writing '%s' into disk", content);

    //  And writes that string into the file
    int result = writeFileContent(fileName, content);
    LoadImmediateIntoRegister(1, result);
}

void listFile_SystemCall()
{
    printList();
    LoadImmediateIntoRegister(1, getFilesCount());
}

//  File name --> R2
void fileExist_SystemCall()
{
    char *fileName = fileNameFromRegister();

    int result = fileExists(fileName);
    LoadImmediateIntoRegister(1, result);
}

// Old name ---> R2
// New name ---> R3
void renameFile_SystemCall()
{
    char *fileName = fileNameFromRegister();

    int fileNameLocation = ReadDataFromRegister(3);
    char *newName = ReadStringFromRam(fileNameLocation);

    int result = renameFile(fileName, newName);
    LoadImmediateIntoRegister(1, result);
}

void clearScreen_SystemCall()
{
    printf("\033[2J\033[H");
}

void version_SystemCall()
{
    printf("=========================================\n");
    printf("=============>>PussemblyOS<<=============\n");
    printf("============= Version: %fv =============\n", OS_VERSION);
    printf("== Open Source = Under GNU 3 Lincense ===\n");
    printf("======= Full source at github page: =====\n");
    printf("==== https://github.com/Danial212/VM ====\n");
    printf("======= All right reserved, 2025 ========\n");
    printf("=========================================\n");
}

void copyFile_SystemCall()
{
//     char *fileName = fileNameFromRegister();
//     int targetNameAddress = ReadDataFromRegister(3);
//     char *targetFileName = ReadStringFromRam(targetNameAddress);

//     Node *file = getFileTable(fileName);
//     Node *targetFile = getFileTable(targetFileName);
//     int size = min(file->data.size, targetFile->data.size);

//     //  And save it into the RAM
//     for (size_t i = 0; i < size; i++)
//         disk[targetFile->data.location + i] = getDiskChar(file->data.location + i);
}
void runFile_SystemCall()
{
    char *fileName = fileNameFromRegister();

    printf("-------------------- running %s-------------------- \n", fileName);
    PCB pcb = {
        .blockName = fileName,
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
    FileReading(fileName);
}

/// @brief Gets the target name based on 2th register's pointer
char *
fileNameFromRegister()
{
    int fileNameLocation = ReadDataFromRegister(2);
    char *fileName = ReadStringFromRam(fileNameLocation);
    return fileName;
}