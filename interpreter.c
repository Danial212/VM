#include "interpreter.h"
#include "Hardware.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

//  Writing string, starts at the given address, and ends with symbol '\0'
void WriteStringIntoRam(int address, char *str)
{
    int len = strlen(str);
    for (size_t i = 0; i < len; i++)
    {
        WriteIntoRam(address + i, str[i]);
    }
    // Ending symbol for a string in Memory
    WriteIntoRam(address + len, '\0');
}

//  Worked my ass to write this shity function, so be careful about changing it
char *ReadStringFromRam(int address)
{
    int index = 0;
    while (ReadFromRAM(address + index) != '\0')
        index++;

    char *str = malloc(index * sizeof(char));

    for (size_t i = 0; i < index + 1; i++)
        str[i] = ReadFromRAM(address + i);

    return str;
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