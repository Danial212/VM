#include <stdio.h>
#include <stdlib.h>
#include "Hardware.c"

// =============================================
//              Register Operations
// =============================================

// Move data from RegisterA to RegisterB
void MoveRegisterToRegister(int sourceRegister, int destinationRegister)
{
    int value = ReadDataFromRegister(sourceRegister);
    WriteIntoRegister(destinationRegister, value);
}

// Load immediate value into register
void LoadImmediate(int value, int destinationRegister)
{
    WriteIntoRegister(destinationRegister, value);
}

//  Load Value From Ram Into Register
void MoveRamToRegister(int ramLocation, int destinationRegister)
{
    int data = ReadFromRAM(ramLocation);
    WriteIntoRegister(destinationRegister, data);
}
// =============================================
//              Arithmetic Operations
// =============================================

// Add RegisterB to RegisterA (result in RegisterA)
void AddRegisters(int registerA, int registerB)
{
    int result = ReadDataFromRegister(registerA) + ReadDataFromRegister(registerB);
    WriteIntoRegister(registerA, result);
}

// Subtract RegisterB from RegisterA (result in RegisterA)
void SubtractRegisters(int registerA, int registerB)
{
    int result = ReadDataFromRegister(registerA) - ReadDataFromRegister(registerB);
    WriteIntoRegister(registerA, result);
}

// Multiply RegisterA by RegisterB (result in RegisterA)
void MultiplyRegisters(int registerA, int registerB)
{
    int result = ReadDataFromRegister(registerA) * ReadDataFromRegister(registerB);
    WriteIntoRegister(registerA, result);
}

// Divide RegisterA by RegisterB (result in RegisterA)
void DivideRegisters(int registerA, int registerB)
{
    int divisor = ReadDataFromRegister(registerB);
    if (divisor != 0)
    {
        int result = ReadDataFromRegister(registerA) / divisor;
        WriteIntoRegister(registerA, result);
    }
    else
    {
        printf("⚠️ Error: Division by zero!\n");
    }
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
    {
        printf("⚠️ Error: Modulo by zero!\n");
    }
}

// =============================================
//              Memory Operations
// =============================================

//  Load Value From Register Into RAM
void MoveRegisterToRAM(int ramLocation, int sourceRegister)
{
    int data = ReadDataFromRegister(sourceRegister);
    WriteIntoRam(ramLocation, data);
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