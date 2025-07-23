#include "stdio.h"
#include "stdint.h"

typedef struct
{
    //  Save number in range [0,255] into registers
    uint8_t R1;
    uint8_t R2;
    uint8_t R3;
    uint8_t R4;
    uint8_t ram[65536];
} Register;
