#include "CommonTools.h"
#include "FileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char disk[DISK_SIZE];
Node *head;

// Function to add a new node at the beginning of the list
int addToBeginning(FileCell fileData)
{
    // Allocate memory for new node
    Node *newNode = (Node *)malloc(sizeof(Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        return -1;
    }

    // Copy the file data to the new node
    strcpy(newNode->data.fileName, fileData.fileName);
    newNode->data.location = fileData.location;
    newNode->data.size = fileData.size;

    // Point new node to current head
    newNode->next = head;

    // set new node as the new head
    head = newNode;
    return 1;
}

/////////////////////////////////////////////////////
/////////////// BASIC FILE OPERATIONS ///////////////
/////////////////////////////////////////////////////

// Adding a file (as a new head)
int addFile(char *name, int location, int size)
{
    FileCell cell;

    strncpy(cell.fileName, name, sizeof(cell.fileName) - 1);
    cell.fileName[sizeof(cell.fileName) - 1] = '\0';
    cell.location = location;
    cell.size = size;

    return addToBeginning(cell);
}

// Delete a file using a name
int deleteFile(char *name)
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
                head = newHead; // head changes
            }
            else
            {
                prev->next = current->next;
                free(current);
                // Head stays the same
            }
            return 1;
        }

        prev = current;
        current = current->next;
    }

    return -1;
}

void defragmentation()
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
int searchFile(char *name)
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

Node *getFileTable(char *name)
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

/////////////// LOW-LEVEL SPACE OPERATIONS ///////////////
// All of linked list's used spase
int getTotalUsedSpace()
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
int availbleFreeSpace()
{
    int used = getTotalUsedSpace();
    return DISK_SIZE - used;
}

/////////////// LOW-LEVEL FILE OPERATIONS ///////////////
// Save FAT to disk
int saveFAT()
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
void loadFAT()
{
    FILE *fp = fopen(FAT_NAME, "rb");
    if (!fp)
        return;

    FileCell temp; // Just use a local variable instead

    while (fread(&temp, sizeof(FileCell), 1, fp))
    {
        printf("Loaded: %s, size: %d bytes\n", temp.fileName, temp.size);
        addToBeginning(temp);
    }

    fclose(fp);
}

void saveDisk()
{
    FILE *f = fopen("Vdisk", "wb");
    for (size_t i = 0; i < DISK_SIZE; i++)
    {
        char r = disk[i];
        fwrite(&disk[i], sizeof(char), 1, f);
    }

    fclose(f);
}

void loadDisk()
{
    FILE *f = fopen("Vdisk", "rb");

    if (f == NULL)
        return;

    for (size_t i = 0; i < DISK_SIZE; i++)
    {
        char r;
        fread(&r, sizeof(char), 1, f);
        // printf("%c|", r == 0 ? ' ' : r);
        disk[i] = r;
    }

    fclose(f);
}

// formating the whole file system
void formatFileSystem()
{
    printf("Formatting file system... all files deleted!\n");
    freeList();
}

/////////////// FILEs OPERATIONS ///////////////
// Writing into file(from FAT into disk)
int writeFileContent(char *fileName, char *content)
{
    Node *currentFile = getFileTable(fileName);
    if (currentFile == NULL)
        return 0;
    int location = currentFile->data.location;
    int contentSize = strlen(content);
    if (availbleFreeSpace() < contentSize)
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
int readFileContent(char *filename)
{
    // fopen(fp);
    Node *currentFile = getFileTable(filename);
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
void freeList()
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
void printList()
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

/////////////// USER_LEVEL File Operatoins ///////////////
//  Find the first availible space in disk for creating file, with the given size
int createFile(char *fileName, int size)
{
    int firstAddress = fisrtUsableBlock(size);

    if (firstAddress < 0)
        return 0; //    Not enough Space for the given size

    DebugLog("making file %s at address %d in size of %d", fileName, firstAddress, size);
    return addFile(fileName, firstAddress, size);
}
char getDiskChar(int index){
    return disk[index];
}

/////////////// HELPER FUNCTIONS ///////////////
// Counting files
int getFilesCount()
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
int renameFile(char *lastName, char *newName)
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
int fileExists(char *name)
{
    return searchFile(name) != -1;
}

// A full checkup I guess?
void printFileSystemStats()
{
    int used = getTotalUsedSpace();
    int free = availbleFreeSpace();
    int fileCount = getFilesCount();

    printf("=== File System Stats ===\n");
    printf("Total space: %d bytes\n", DISK_SIZE);
    printf("Used space : %d bytes\n", used);
    printf("Free space : %d bytes\n", free);
    printf("Files count: %d\n", fileCount);
    printf("=========================\n");
}

int fisrtUsableBlock(int size)
{
    if (head == NULL)
    {
        return 0; // First file starts at position 0
    }

    // Sort files by location (simple bubble sort for small lists)
    // Create array of used blocks
    int usedBlocks[getFilesCount() * 2]; // Start and end positions
    int blockCount = 0;

    Node *current = head;
    while (current != NULL)
    {
        usedBlocks[blockCount++] = current->data.location;
        usedBlocks[blockCount++] = current->data.location + current->data.size;
        current = current->next;
    }

    // Sort the blocks
    for (int i = 0; i < blockCount - 1; i++)
    {
        for (int j = 0; j < blockCount - i - 1; j++)
        {
            if (usedBlocks[j] > usedBlocks[j + 1])
            {
                int temp = usedBlocks[j];
                usedBlocks[j] = usedBlocks[j + 1];
                usedBlocks[j + 1] = temp;
            }
        }
    }

    // Find first gap that fits the size
    int lastEnd = 0;
    for (int i = 0; i < blockCount; i += 2)
    {
        if (usedBlocks[i] - lastEnd >= size)
        {
            return lastEnd;
        }
        lastEnd = usedBlocks[i + 1];
    }

    // Check if there's space at the end
    if (DISK_SIZE - lastEnd >= size)
    {
        return lastEnd;
    }

    return -1; // No space available
}

//  -1 -->  can't allocate enough memmory
int bootDisk()
{
    head = NULL;
    //  loads the File Allocation Table (if not saved, initialize a new one)
    printf("Loding File Allocation Table ... \n");
    loadFAT();
    //  load the disk, (if not saved, initialize a new one)
    printf("Loding Disk values ... \n");
    loadDisk();
    //  show the status
    printList();

    return 1;
}

//  Save the file system(FAT + Disk) when we're done working with OS
void save_Disk_FAT()
{
    saveFAT();
    saveDisk();
}