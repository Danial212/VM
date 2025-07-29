#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>

#define RAM_SIZE 1024
#define RAM_EMPTY_DATA -1

const int REGISTER_COUNT = 4;
const int RAM_SPACE = RAM_SIZE;

//  This simple structure, is a simple Memoty of a computer
//  Containing 4 Register and 1 RAM
//  Board structure (already defined in your file)
typedef struct
{
    int *R1;
    int *R2;
    int *R3;
    int *R4;
    int ram[RAM_SIZE];
} Board;

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

// Function declarations
void InitializeHardWare(void);
void RamCleaning(void);
void WriteIntoRam(int data, int location);
int ReadFromRAM(int location);
void WriteIntoRegister(int registerNumber, int data);
int ReadDataFromRegister(int registerNumber);
int *ReadRegisterPointer(int registerNumber);
void MoveFrom_R_to_RAM(int _register_Number, int RAM_location);
void MoveFrom_RAM_to_R(int _register_Number, int RAM_location);
int IsRamAvalible(int location);

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
        fprintf(stderr, "Memory allocation failed!");
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