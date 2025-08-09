#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Hardware.h"
#include "GlobalVariables.h"
#include <math.h>

const int REGISTER_COUNT = 4;
const int RAM_SPACE = RAM_SIZE;

//  This simple structure, is a simple Memoty of a computer
//  Containing 4 Register and 1 RAM
//  Board structure (already defined in your file)


// Initialize RAM with empty values
Board hardwareBoard = {
    .R1 = NULL,
    .R2 = NULL,
    .R3 = NULL,
    .R4 = NULL,
    .ram = {RAM_EMPTY_DATA}, // Initializes all elements to -1
};

// Global variable
Board hardwareBoard;


/// @brief Initializing the basic component of the VM [like Cleaning RAM, Allocating Register, etc...]
/// @warning This function has to get executed before any other functions
void InitializeHardWare()
{
    hardwareBoard.R1 = malloc(sizeof(int));
    hardwareBoard.R2 = malloc(sizeof(int));
    hardwareBoard.R3 = malloc(sizeof(int));
    hardwareBoard.R4 = malloc(sizeof(int));
    if (!hardwareBoard.R1 || !hardwareBoard.R2 || !hardwareBoard.R3 || !hardwareBoard.R4)
    {
        fprintf(stderr, "E12: Registers Allocation Failed!");
        exit(EXIT_FAILURE);
    }
    *hardwareBoard.R1 = 11;
    *hardwareBoard.R2 = 12;
    *hardwareBoard.R3 = 13;
    *hardwareBoard.R4 = 14;

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
    switch (registerNumber)
    {
    case 1:
        return hardwareBoard.R1;
    case 2:
        return hardwareBoard.R2;
    case 3:
        return hardwareBoard.R3;
    case 4:
        return hardwareBoard.R4;
    }
    return NULL;
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
    int len = int_length(*hardwareBoard.R1);
    printf("1: %d", *hardwareBoard.R1);
    for (size_t i = 0; i < 15 - len - 4; i++)
        printf(" ");
    printf("|");

    len = int_length(*hardwareBoard.R2);
    printf("2: %d", *hardwareBoard.R2);
    for (size_t i = 0; i < 15 - len - 4; i++)
        printf(" ");
    printf("|");

    len = int_length(*hardwareBoard.R3);
    printf("3: %d", *hardwareBoard.R3);
    for (size_t i = 0; i < 15 - len - 4; i++)
        printf(" ");
    printf("|");

    len = int_length(*hardwareBoard.R4);
    printf("4: %d", *hardwareBoard.R4);
    for (size_t i = 0; i < 15 - len - 4; i++)
        printf(" ");
    printf("|\n");

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
