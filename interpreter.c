#include <stdio.h>
#include <stdlib.h>
#include "Hardware.c"

// =============================================
//              Register Operations
// =============================================

// Load immediate value into register
void LoadImmediateIntoRegister(int destinationRegister, int value)
{
    WriteIntoRegister(destinationRegister, value);
}

// =============================================
//              Arithmetic Operations
// =============================================

// Add RegisterB to RegisterA (result in RegisterA)
void Add_Registers(int registerA, int registerB)
{
    int result = ReadDataFromRegister(registerA) + ReadDataFromRegister(registerB);
    WriteIntoRegister(registerA, result);
}

// Subtract RegisterB from RegisterA (result in RegisterA)
void Subtract_Registers(int registerA, int registerB)
{
    int result = ReadDataFromRegister(registerA) - ReadDataFromRegister(registerB);
    WriteIntoRegister(registerA, result);
}

// Multiply RegisterA by RegisterB (result in RegisterA)
void Multiply_Registers(int registerA, int registerB)
{
    int result = ReadDataFromRegister(registerA) * ReadDataFromRegister(registerB);
    WriteIntoRegister(registerA, result);
}

// Divide RegisterA by RegisterB (result in RegisterA)
void Divide_Registers(int registerA, int registerB)
{
    int divisor = ReadDataFromRegister(registerB);
    if (divisor != 0)
    {
        int result = ReadDataFromRegister(registerA) / divisor;
        WriteIntoRegister(registerA, result);
    }
    else
        printf("⚠️ Error: Division by zero!\n");
}

// Store remainder of division in RegisterA
void ModuloRegisters(int registerA, int registerB)
{
    int divisor = ReadDataFromRegister(registerB);
    if (divisor != 0)
    {
        int result = ReadDataFromRegister(registerA) % divisor;
        WriteIntoRegister(registerA, result);
    }
    else
        printf("⚠️ Error: Modulo by zero!\n");
}

// =============================================
//              Memory Operations
// =============================================

//  Load immediate value into RAM
void LoadImmediateIntoRAM(int ramLocation, int value)
{
    WriteIntoRam(ramLocation, value);
}

// =============================================
//              Output Operations
// =============================================

// Print value from RAM at specified location
void PrintRamValue(int ramLocation)
{
    int value = ReadFromRAM(ramLocation);
    if (value != -1)
        printf("📦 RAM[%d] = %d\n", ramLocation, value);
    else
        printf("📦 RAM[%d] is empty.\n", ramLocation);
}

// Print value from Register
void PrintRegisterValue(int destinationRegister)
{
    int value = ReadDataFromRegister(destinationRegister);

    printf("📦 REG[%d] = %d\n", destinationRegister, value);
}