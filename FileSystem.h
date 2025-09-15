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
void addFile(char *name, int location, int size);
//  Delete the given node by name
void deleteFile(char *name, struct Node *head);
void difragmetation();
int searchFile(char *name);

/////////////// LOW-LEVEL FILE OPERATIONS ///////////////
//  First free space
int findFreeSpace(int size);
int getTotalUsedSpace();

/////////////// LOW-LEVEL FILE OPERATIONS ///////////////
// Load/Save FAT to/from disk
int loadFATFromDisk();
int saveFATToDisk();
//  Clear all the files and delete full files table
int formatFileSystem();

/////////////// FILEs OPERATIONS ///////////////
// Read/Write actual file content
int readFileContent();
int writeFileContent();
int appendToFile();

/////////////// HELPER FUNCTIONS ///////////////
//  FAT's size (all the files we have)
int getFilesCount();
//  Rename given files's name
int renameFile(char *lastname, char *newName);
//  Not sure what the hell it's going to do
void printFileSystemStats();
//  Check if the given file exists
int fileExists(char *name);

void testFileSystem();


#endif