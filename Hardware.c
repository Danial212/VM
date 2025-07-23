#include "stdio.h"
#include "stdint.h"

//  This simple structure, is a simple Memoty of a computer
//  Containing 4 Register and 1 RAM
typedef struct
{
    //  Save 8 bit number in range [0,255] into registers
    uint8_t R1;
    uint8_t R2;
    uint8_t R3;
    uint8_t R4;

    //  Rasarving ~64kb Ram
    uint8_t ram[65536];
} Register;

void moveToRegister(){
    //  simple code for testing
}