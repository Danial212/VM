#ifndef HARDWARE_H // Unique name for this Hardware.h
#define HARDWARE_H

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
int *GetRamPointer(int);
void RegistersManitoring();
int int_length(int num);
void RamManitoring();

extern const int REGISTER_COUNT;
extern const int RAM_SPACE;

typedef struct
{
    int *R1;
    int *R2;
    int *R3;
    int *R4;
    int ram[1024];
} Board;

extern Board hardwareBoard;

#endif