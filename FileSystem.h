#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Data structure for file information
struct FileCell
{
    char fileName[32];
    int location;
    int size;
};

// Node structure for the linked list
struct Node
{
    struct FileCell data;
    struct Node *next;
};

//  Function to add a new node at the beginning of the list
struct Node *addToBeginning(struct Node *head, struct FileCell fileData);
//  Function to free all nodes in the list
void freeList(struct Node *head);
//  Helper function to print the list (for testing)
void printList(struct Node *head);


/////////////// BASIC FILE OPERATIONS ///////////////
void addFile(struct Node *head ,char *name, int location, int size);
struct Node *deleteFile(struct Node *head, char *name);
void defragmentation(struct Node *head);
int searchFile(struct Node *head, char *name);

/////////////// LOW-LEVEL FILE OPERATIONS ///////////////
//  First free space
int findFreeSpace(struct Node *head ,int size);
int getTotalUsedSpace(struct Node *head);

/////////////// LOW-LEVEL FILE OPERATIONS ///////////////
// Load/Save FAT to/from disk
int *loadFATFromDisk(const char *filename);
int saveFATToDisk(struct Node *head, const char *filename);
//  Clear all the files and delete full files table
void formatFileSystem(struct Node *head);

/////////////// FILEs OPERATIONS ///////////////
// Read/Write actual file content
int readFileContent(const char *filename);
int writeFileContent(const char *filename, const char *content);
int appendToFile(const char *filename, const char *content);

/////////////// HELPER FUNCTIONS ///////////////
//  FAT's size (all the files we have)
int getFilesCount(struct Node *head);
//  Rename given files's name
int renameFile(struct Node *head ,char *lastname, char *newName);
//  Not sure what the hell it's going to do
void printFileSystemStats(struct Node *head, int size);
//  Check if the given file exists
int fileExists(struct Node *head ,char *name);

void testFileSystem();


#endif