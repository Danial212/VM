#include "stdio.h"
#include "stdint.h"

//  This simple structure, is a simple Memoty of a computer
//  Containing 4 Register and 1 RAM
typedef struct
{
    //  Save 8 bit number in range [0,255] into registers
    uint8_t *R1;
    uint8_t *R2;
    uint8_t *R3;
    uint8_t *R4;

    uint8_t ram[64];

} Board;

Board hardwareBoard;

/// @brief Initializing the basic component of the VM [like Cleaning RAM, Allocating Register, etc...]
/// @warning This function gets executed before any other functions
void InitializeHardWare()
{
    hardwareBoard.R1 = malloc(1);
    hardwareBoard.R2 = malloc(1);

    *(hardwareBoard.R1) = 182;

    printf(">%d<", *(hardwareBoard.R1));

    *(hardwareBoard).R2 = 45;
    printf(">%d<", *(hardwareBoard.R2));
}

/// @brief Cleans Ram, Set all the datas to 0
void RamCleaning()
{
}

/// @brief  Save the data into RAM
/// @warning Better always check to see if the ram location is avalible, or not
void WriteIntoRam(uint8_t data, uint8_t pos)
{
}

/// @brief Retreives the Data from the RAM at the given 'location'
uint8_t ReadFromRAM(uint8_t location)
{
}

/// @brief  Save the data into register
void WriteIntoRegister(uint8_t registerNumber, uint8_t data)
{
}

/// @brief The Data of the register with the given number
uint8_t ReadDataFromRegister(uint8_t registerNumber)
{
}

//  Get register pointer
uint8_t *ReadDataPointerFromRegister(uint8_t registerNumber)
{
}

/// @brief Moves Data from Register into RAM (keeps the register the same it was)
void MoveFrom_Rto_RAM(uint8_t _register_Number, uint8_t RAM_location)
{
}

/// @brief Checks if the given location in the RAM is avalible, for writing data
/// @return Returns 1 if avalible, and 0 is already reserved (Not Avalible)
int IsRamAvalible(uint8_t location)
{
}