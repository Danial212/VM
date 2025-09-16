#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISK_SIZE 1024
#define FAT_NAME "FileSystem.at"

// Data structure for file information
typedef struct FileCell
{
    char fileName[32];
    int location;
    int size;
} FileCell;

// Node structure for the linked list
typedef struct Node
{
    struct FileCell data;
    struct Node *next;
} Node;

extern char disk[DISK_SIZE];
extern Node *head;

//  Function to add a new node at the beginning of the list
int addToBeginning(FileCell fileData);
//  Function to free all nodes in the list
void freeList();
//  Helper function to print the list (for testing)
void printList();

/////////////// BASIC FILE OPERATIONS ///////////////
int addFile(char *name, int location, int size);
int deleteFile(char *name);
void defragmentation();
int searchFile(char *name);
Node *getFileTable(char *name);

/////////////// BOOT-LEVEL FILE SYSTEM ///////////////
// Load/Save FAT to/from disk
void loadFAT();
int saveFAT();
void formatFileSystem();

/////////////// LOW-LEVEL FILE OPERATIONS ///////////////
//  First free space
int availbleFreeSpace();
int getTotalUsedSpace();

/////////////// FILEs OPERATIONS ///////////////
// Read/Write actual file content
int readFileContent(char *filename);
int writeFileContent(char *filename, char *content);
int appendToFile(char *filename, char *content);

/////////////// USER_LEVEL File Operatoins ///////////////
int createFile(char *fileName, int size);
char getDiskChar(int index);

/////////////// HELPER FUNCTIONS ///////////////
//  FAT's size (all the files we have)
int getFilesCount();
//  Rename given files's name
int renameFile(char *lastname, char *newName);
void printFileSystemStats();
//  Check if the given file exists
int fileExists(char *name);
int fisrtUsableBlock(int size);

int bootDisk();
void save_Disk_FAT();

#endif