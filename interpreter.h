// Example: CommonTools.h
#ifndef INTERPRETER_H // Unique name for this header
#define INTERPRETER_H

// Declarations go here
void LoadImmediateIntoRegister(int destinationRegister, int value);
void Add_Registers(int registerA, int registerB);
void Subtract_Registers(int registerA, int registerB);
void Multiply_Registers(int registerA, int registerB);
void Divide_Registers(int registerA, int registerB);
void ModuloRegisters(int registerA, int registerB);
void LoadImmediateIntoRAM(int ramLocation, int value);
void WriteStringIntoRam(int address, char *str);
char *ReadStringFromRam(int address);
void PrintRamValue(int ramLocation);
void PrintRegisterValue(int destinationRegister);

#endif