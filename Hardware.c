#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Hardware.h"
#include "GlobalVariables.h"
#include <math.h>

const int REGISTER_COUNT = 16;
const int RAM_SPACE = RAM_SIZE;

//  This simple structure, is a simple Memoty of a computer
//  Containing 4 Register and 1 RAM
//  Board structure (already defined in your file)

// Initialize RAM with empty values
Board hardwareBoard = {
    .R[0] = 0,
    .R[1] = 0,
    .R[2] = 0,
    .R[3] = 0,
    .R[4] = 0,
    .R[5] = 0,
    .R[6] = 0,
    .R[7] = 0,
    .R[8] = 0,
    .R[9] = 0,
    .R[10] = 0,
    .R[11] = 0,
    .R[12] = 0,
    .R[13] = 0,
    .R[14] = 0,
    .R[15] = 0,
    .ram = {RAM_EMPTY_DATA}, // Initializes all elements to -1
};

// Global variable
Board hardwareBoard;

/// @brief Initializing the basic component of the VM [like Cleaning RAM, Allocating Register, etc...]
/// @warning This function has to get executed before any other functions
void InitializeHardWare()
{
    RamCleaning();
}

/// @brief Cleans Ram, Set all the datas to 0
void RamCleaning()
{
    for (size_t i = 0; i < RAM_SIZE; i++)
        hardwareBoard.ram[i] = RAM_EMPTY_DATA;
}

int *GetRamPointer(int ramLocation)
{
    if (ramLocation < 0 || ramLocation >= RAM_SPACE) {
        
        fwrite("Error in getting ram location [%d]", ramLocation, 40, stderr);
        return NULL;
    }
    
    return (hardwareBoard.ram + ramLocation);
}

/// @brief  Save the data into RAM
/// @warning Better always check to see if the ram location is avalible, or not
void WriteIntoRam(int location, int data)
{
    hardwareBoard.ram[location] = data;
}

/// @brief Retreives the Data from the RAM at the given 'location'
int ReadFromRAM(int location)
{
    return hardwareBoard.ram[location];
}

/// @brief  Save the data into register
void WriteIntoRegister(int registerNumber, int data)
{
    *ReadRegisterPointer(registerNumber) = data;
}

/// @brief The Data of the register with the given number
int ReadDataFromRegister(int registerNumber)
{
    return *ReadRegisterPointer(registerNumber);
}

//  Get register pointer
int *ReadRegisterPointer(int registerNumber)
{
    if (registerNumber <= 0 || registerNumber > REGISTER_COUNT)
    {
        printf("Out of bound register access, can't get [%d]", registerNumber - 1);
        return NULL;
    }
    return &hardwareBoard.R[registerNumber - 1];
}

/// @brief Moves Data from Register into RAM (keeps the register the same it was)
void MoveFrom_R_to_RAM(int _register_Number, int RAM_location)
{
    int data = ReadDataFromRegister(_register_Number);
    WriteIntoRam(RAM_location, data);
}

/// @brief Moves Data from RAM into Register (keeps the RAM the same it was)
void MoveFrom_RAM_to_R(int RAM_location, int _register_Number)
{
    int data = ReadFromRAM(RAM_location);
    WriteIntoRegister(_register_Number, data);
}

/// @brief Checks if the given location in the RAM is avalible, for writing data
/// @return Returns 1 if avalible, and 0 is already reserved (Not Avalible)
int IsRamAvalible(int location)
{
    return (ReadFromRAM(location) == RAM_EMPTY_DATA);
}

//  Printing the states of all register at the time
void RegistersManitoring()
{
    printf("######################## Registers #########################\n");

    for (size_t i = 1; i <= REGISTER_COUNT; i++)
    {
        printf("|");
        int indexLen = int_length(i);

        printf("%d:", i);
        for (size_t i = 0; i < 4 - indexLen; i++)
            printf(" ");

        int valueLen = int_length(hardwareBoard.R[i - 1]);
        printf("%d", hardwareBoard.R[i - 1]);
        for (size_t i = 0; i < 10 - valueLen; i++)
            printf(" ");

        if ((i) % 4 == 0)
            printf("\n");
    }

    printf("------------------------------------------------------------\n");
}

void RamManitoring()
{
    int printerCount = 0;
    printf("\n########################### RAM ############################\n");

    for (size_t i = 0; i < 1024; i++)
    {
        if (hardwareBoard.ram[i] != RAM_EMPTY_DATA)
        {
            printerCount++;
            int len = int_length(hardwareBoard.ram[i]) + int_length(i);
            if (hardwareBoard.ram[i] < 0)
                len++;
            printf("%d]:%d", i, hardwareBoard.ram[i]);
            for (size_t i = 0; i < 24 - len - 2; i++)
                printf(" ");
            printf("|");
            if (printerCount % 3 == 0)
                printf("\n");
        }
    }

    printf("\n------------------------------------------------------------\n");
}
