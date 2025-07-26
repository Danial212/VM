#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>

//  This simple structure, is a simple Memoty of a computer
//  Containing 4 Register and 1 RAM
typedef struct
{
    //  Save 8 bit number in range [0,255] into registers
    uint8_t *R1;
    uint8_t *R2;
    uint8_t *R3;
    uint8_t *R4;

    uint8_t ram[1024];

} Board;

Board hardwareBoard;

/// @brief Initializing the basic component of the VM [like Cleaning RAM, Allocating Register, etc...]
/// @warning This function has to get executed before any other functions
void InitializeHardWare()
{
    hardwareBoard.R1 = malloc(4);
    hardwareBoard.R2 = malloc(4);
    hardwareBoard.R3 = malloc(4);
    hardwareBoard.R4 = malloc(4);

    // RamCleaning();
}

/// @brief Cleans Ram, Set all the datas to 0
// void RamCleaning()
// {
//     for (size_t i = 0; i < RAM_SIZE; i++)
//         hardwareBoard.ram[i] = 0;
// }

/// @brief  Save the data into RAM
/// @warning Better always check to see if the ram location is avalible, or not
// void WriteIntoRam(uint8_t data, uint8_t location)
// {
//     hardwareBoard.ram[location] = data;
// }

// /// @brief Retreives the Data from the RAM at the given 'location'
// uint8_t ReadFromRAM(uint8_t location)
// {
//     return hardwareBoard.ram[location];
// }

// /// @brief  Save the data into register
// void WriteIntoRegister(uint8_t registerNumber, uint8_t data)
// {
//     &ReadDataPointerFromRegister(registerNumber) = data;
// }

/// @brief The Data of the register with the given number
uint8_t ReadDataFromRegister(int registerNumber)
{
    return *(hardwareBoard).R1;
    // switch (registerNumber)
    // {
    // case 1:
    //     return *hardwareBoard.R1;
    //     break;
    // case 2:
    //     return *hardwareBoard.R2;
    //     break;
    // case 3:
    //     return *hardwareBoard.R3;
    //     break;
    // case 4:
    //     return *hardwareBoard.R4;
    //     break;
    // }
}

// //  Get register pointer
// uint8_t *ReadDataPointerFromRegister(uint8_t registerNumber)
// {
//     switch (registerNumber)
//     {
//     case 1:
//         return hardwareBoard.R1;
//         break;
//     case 2:
//         return hardwareBoard.R2;
//         break;
//     case 3:
//         return hardwareBoard.R3;
//         break;
//     case 4:
//         return hardwareBoard.R4;
//         break;
//     }
// }

// /// @brief Moves Data from Register into RAM (keeps the register the same it was)
// void MoveFrom_R_to_RAM(uint8_t _register_Number, uint8_t RAM_location)
// {
// }

// /// @brief Checks if the given location in the RAM is avalible, for writing data
// /// @return Returns 1 if avalible, and 0 is already reserved (Not Avalible)
// int IsRamAvalible(uint8_t location)
// {
// }

int main()
{
    InitializeHardWare();

    *hardwareBoard.R1 = 25;

    printf("%d,", ReadDataFromRegister(1));
}
