#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "SystemCalls.h"
#include "FileSystem.h"
#include "Runner.h"
#include "interpreter.h"
#include "Hardware.h"

void handleSystemCalls()
{
    // tok[0]   tok[1]
    //  LOAD R1 1
    //  SYSCALL
    int callCode = ReadDataFromRegister(1);
    printf("Calling System Calls with code %d \n", callCode);

    switch (callCode)
    {
    case 1:
        createFile_SystemCall();
        break;
    case 2:
        /* code */
        break;
    case 3:
        /* code */
        break;

    default:
        break;
    }
}


/// @brief File Name Address pointer = R2, File Size = R3
void createFile_SystemCall()
{
    //  Get file name string location in RAM, from R2 register
    int fileNameLocation = ReadDataFromRegister(2);
    char *fileName = ReadStringFromRam(fileNameLocation);
    //  Get file's size from R3 register
    int size = ReadDataFromRegister(3);

    createFile(fileName, size);
}
void deleteFile_SystemCall()
{
}
void readFile_SystemCall()
{
}
void writeFile_SystemCall()
{
}
void listFile_SystemCall()
{
}
void fileExist_SystemCall()
{
}
void renameFile_SystemCall()
{
}