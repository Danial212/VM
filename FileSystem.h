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



//  Function to add a new node at the beginning of the list
Node *addToBeginning(Node *head, FileCell fileData);
//  Function to free all nodes in the list
void freeList(Node *head);
//  Helper function to print the list (for testing)
void printList(Node *head);

/////////////// BASIC FILE OPERATIONS ///////////////
Node *addFile(Node *head, char *name, int location, int size);
Node *deleteFile(Node *head, char *name);
void defragmentation(Node *head);
int searchFile(Node *head, char *name);
Node *getFileTable(Node *head, char *name);


/////////////// BOOT-LEVEL FILE SYSTEM ///////////////
// Load/Save FAT to/from disk
Node *loadFAT();
int saveFAT(Node *head);
void formatFileSystem(Node *head);

/////////////// LOW-LEVEL FILE OPERATIONS ///////////////
//  First free space
int availbleFreeSpace(Node *head);
int getTotalUsedSpace(Node *head);

/////////////// FILEs OPERATIONS ///////////////
// Read/Write actual file content
int readFileContent(Node *head, char *filename);
int writeFileContent(Node *head, char *filename, char *content);
int appendToFile(char *filename, char *content);

/////////////// HELPER FUNCTIONS ///////////////
//  FAT's size (all the files we have)
int getFilesCount(Node *head);
//  Rename given files's name
int renameFile(Node *head, char *lastname, char *newName);
void printFileSystemStats(Node *head);
//  Check if the given file exists
int fileExists(Node *head, char *name);

int bootDisk();

#endif