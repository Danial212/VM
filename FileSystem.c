#include "CommonTools.h"
#include "FileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char disk[DISK_SIZE];

// Function to add a new node at the beginning of the list
Node *addToBeginning(Node *head, FileCell fileData)
{
    // Allocate memory for new node
    Node *newNode = (Node *)malloc(sizeof(Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        return head;
    }

    // Copy the file data to the new node
    strcpy(newNode->data.fileName, fileData.fileName);
    newNode->data.location = fileData.location;
    newNode->data.size = fileData.size;

    // Point new node to current head
    newNode->next = head;

    // Return new node as the new head
    return newNode;
}

/////////////////////////////////////////////////////
/////////////// BASIC FILE OPERATIONS ///////////////
/////////////////////////////////////////////////////

// Adding a file (as a new head)
Node *addFile(Node *head, char *name, int location, int size)
{
    FileCell cell;

    strncpy(cell.fileName, name, sizeof(cell.fileName) - 1);
    cell.fileName[sizeof(cell.fileName) - 1] = '\0';
    cell.location = location;
    cell.size = size;

    head = addToBeginning(head, cell);
    return head;
}

// Delete a file using a name
Node *deleteFile(Node *head, char *name)
{
    Node *current = head;
    Node *prev = NULL;

    while (current != NULL)
    {

        if (strcmp(current->data.fileName, name) == 0)
        {
            // If it's the head node
            if (prev == NULL)
            {
                Node *newHead = current->next;
                free(current);
                return newHead; // Return new head
            }
            else
            {
                prev->next = current->next;
                free(current);
                return head; // Head stays the same
            }
        }

        prev = current;
        current = current->next;
    }

    printf("File '%s' not found!\n", name);
    return head; // No change
}

void defragmentation(Node *head)
{
    int currentLocation = 0;
    Node *current = head;

    while (current != NULL)
    {
        current->data.location = currentLocation;
        currentLocation += current->data.size;
        current = current->next;
    }

    DebugLog("Defragmentation completed!\n"); // Or you could change it into printf. idk man its 4:49 AM and i'm kinda tired
}

// Search for a file by its name, returns the location in the disk
int searchFile(Node *head, char *name)
{
    Node *current = head;
    while (current != NULL)
    {
        if (strcmp(current->data.fileName, name) == 0)
            return current->data.location;
        current = current->next;
    }

    return -1; // Not found
}

Node *getFileTable(Node *head, char *name)
{
    Node *current = head;
    while (current != NULL)
    {
        if (strcmp(current->data.fileName, name) == 0)
            return current;
        current = current->next;
    }

    return NULL; // Not found
}

//////////////////////////////////////////////////////////
/////////////// LOW-LEVEL SPACE OPERATIONS ///////////////
//////////////////////////////////////////////////////////

// All of linked list's used spase
int getTotalUsedSpace(Node *head)
{
    int total = 0;
    Node *current = head;

    while (current != NULL)
    {
        total += current->data.size;
        current = current->next;
    }
    return total;
}

// How much free space hasn't been used?
int availbleFreeSpace(Node *head)
{
    int used = getTotalUsedSpace(head);
    return DISK_SIZE - used;
}

/////////////////////////////////////////////////////////
/////////////// LOW-LEVEL FILE OPERATIONS ///////////////
/////////////////////////////////////////////////////////

// Save FAT to disk
int saveFAT(Node *head)
{
    FILE *fp = fopen(FAT_NAME, "wb");
    if (!fp)
        return 0;

    Node *current = head;

    while (current != NULL)
    {
        fwrite(&current->data, sizeof(FileCell), 1, fp);
        current = current->next;
    }

    fclose(fp);
    return 1;
}

// Load FAT from disk
Node *loadFAT()
{
    FILE *fp = fopen(FAT_NAME, "rb");
    if (!fp)
        return NULL;

    Node *head = NULL;
    FileCell temp;

    while (fread(&temp, sizeof(FileCell), 1, fp))
    {
        head = addToBeginning(head, temp);
    }

    fclose(fp);
    return head;
}

void saveDisk()
{
    FILE *f = fopen("Vdisk", "wb");

    for (size_t i = 0; i < DISK_SIZE; i++)
    {
        fwrite(&disk[i], sizeof(char), 1, f);
    }

    fclose(f);
}

void loadDisk()
{
    FILE *f = fopen("Vdisk", "rb");

    for (size_t i = 0; i < DISK_SIZE; i++)
    {
        char r;
        fread(&r, sizeof(char), 1, f);
        disk[i] = r;
    }

    fclose(f);
}

// formating the whole file system
void formatFileSystem(Node *head)
{
    printf("Formatting file system... all files deleted!\n");
    freeList(head);
}

/////////////// FILEs OPERATIONS ///////////////
////////////////////////////////////////////////

// Writing into file(from FAT into disk)
int writeFileContent(Node *head, char *fileName, char *content)
{
    Node *currentFile = getFileTable(head, fileName);
    if (currentFile == NULL)
        return 0;
    int location = currentFile->data.location;
    int contentSize = strlen(content);
    if (availbleFreeSpace(head) < contentSize)
        return -1;

    currentFile->data.size = contentSize;

    for (size_t i = 0; i < contentSize; i++)
        disk[location + i] = content[i];

    return 1;
}

// appending to a file
int appendToFile(char *filename, char *content)
{
    return 1;
}

// reading from a file
int readFileContent(Node *head, char *filename)
{
    // fopen(fp);
    Node *currentFile = getFileTable(head, filename);
    int headLocation = currentFile->data.location;
    int size = currentFile->data.size;

    char *buffer = malloc(sizeof(char) * size);

    for (size_t i = 0; i < size; i++)
        buffer[i] = disk[headLocation + i];

    buffer[size] = '\0'; // null terminate

    printf("Content of '%s':\n%s\n", filename, buffer);

    free(buffer);
    // fclose(fp);
    return 1;
}

// Function to free all nodes in the list
void freeList(Node *head)
{
    Node *current = head;
    Node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
}

// Helper function to print the list (for testing)
void printList(Node *head)
{
    Node *current = head;
    int count = 1;

    printf("File List:\n");
    printf("----------\n");

    while (current != NULL)
    {
        printf("%d. File: %s, Location: %d, Size: %d bytes\n",
               count, current->data.fileName, current->data.location, current->data.size);
        current = current->next;
        count++;
    }
    printf("\n");
}

/////////////// HELPER FUNCTIONS ///////////////
////////////////////////////////////////////////

// Counting files
int getFilesCount(Node *head)
{
    int count = 0;
    Node *current = head;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}

// Changing a name of a file
int renameFile(Node *head, char *lastName, char *newName)
{
    Node *current = head;
    while (current != NULL)
    {
        if (StrEqul(current->data.fileName, lastName))
        {
            strncpy(current->data.fileName, newName, sizeof(newName) - 1);
            printf("Changing last anem '%s' into new one: '%s'\n", lastName, newName);
            current->data.fileName[sizeof(current->data.fileName) - 1] = '\0'; // null terminate
            return 1;                                                          // success
        }
        current = current->next;
    }

    return 0; // file not found
}

// Check if file exists
int fileExists(Node *head, char *name)
{
    return searchFile(head, name) != -1;
}

// A full checkup I guess?
void printFileSystemStats(Node *head)
{
    int used = getTotalUsedSpace(head);
    int free = availbleFreeSpace(head);
    int fileCount = getFilesCount(head);

    printf("=== File System Stats ===\n");
    printf("Total space: %d bytes\n", DISK_SIZE);
    printf("Used space : %d bytes\n", used);
    printf("Free space : %d bytes\n", free);
    printf("Files count: %d\n", fileCount);
    printf("=========================\n");
}


//  -1 -->  can't allocate enough memmory

int bootDisk()
{
    Node *head = malloc(sizeof(Node));
    if (!head)
        return -1;

    //  loads the File Allocation Table (if not saved, initialize a new one)
    head = loadFAT();

    //  load the disk, (if not saved, initialize a new one)
    loadDisk();

    //  show the status
    printList(head);


    //  Save the file system(FAT + Disk) when we're done working with OS
    
    saveFAT(head);
    saveDisk();
    return 1;
}