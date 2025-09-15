#include "CommonTools.h"
#include "FileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

// Function to add a new node at the beginning of the list
struct Node *addToBeginning(struct Node *head, struct FileCell fileData)
{
    // Allocate memory for new node
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));

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

void deleteFile(char *name, struct Node *head)
{
    struct Node *current = head;

    while (current != NULL)
    {
        //  then the next node is our target node to delete
        if (StrEqul(current->next->data.fileName, name))
        {
            struct Node *toDelete = current->next;
            if (toDelete->next != NULL)
            {
                struct Node *next = toDelete->next;
                current->next = next;
            }
            else
                current->next = NULL;

            free(toDelete);
            break;
        }

        current = current->next;
    }
}
/////////////////////////////////////////////////////
/////////////// BASIC FILE OPERATIONS ///////////////
/////////////////////////////////////////////////////

// Adding a file (as a new head)
struct Node *addFile(struct Node *head, char *name, int location, int size)
{

    struct FileCell cell;

    strncpy(cell.fileName, name, sizeof(cell.fileName) - 1);
    cell.fileName[sizeof(cell.fileName) - 1] = '\0';
    cell.location = location;
    cell.size = size;

    head = addToBeginning(head, cell);
    return head;
}

// Delete a file using a name
struct Node *deleteFile(struct Node *head, char *name)
{
    struct Node *current = head;
    struct Node *prev = NULL;

    while (current != NULL)
    {

        if (strcmp(current->data.fileName, name) == 0)
        {
            // If it's the head node
            if (prev == NULL)
            {
                struct Node *newHead = current->next;
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

void defragmentation(struct Node *head)
{
    int currentLocation = 0;
    struct Node *current = head;

    while (current != NULL)
    {

        current->data.location = currentLocation;
        currentLocation += current->data.size;
        current = current->next;
    }

    DebugLog("Defragmentation completed!\n"); // Or you could change it into printf. idk man its 4:49 AM and i'm kinda tired
}

// Search for a file using a name and head of linked list
int searchFile(struct Node *head, char *name)
{
    struct Node *current = head;
    while (current != NULL)
    {
        if (strcmp(current->data.fileName, name) == 0)
        {
            return current->data.location;
        }
        current = current->next;
    }

    return -1; // Not found
}

//////////////////////////////////////////////////////////
/////////////// LOW-LEVEL SPACE OPERATIONS ///////////////
//////////////////////////////////////////////////////////

// All of linked list's used spase
int getTotalUsedSpace(struct Node *head)
{
    int total = 0;
    struct Node *current = head;

    while (current != NULL)
    {
        total += current->data.size;
        current = current->next;
    }
    return total;
}

// How much free space hasn't been used?
int findFreeSpace(struct Node *head, int size)
{
    int used = getTotalUsedSpace(head);
    return size - used; // I'm not sure if this is what I should return...
}

/////////////////////////////////////////////////////////
/////////////// LOW-LEVEL FILE OPERATIONS ///////////////
/////////////////////////////////////////////////////////

// Save FAT to disk
int saveFATToDisk(struct Node *head, const char *filename)
{

    FILE *fp = fopen(filename, "wb");
    if (!fp)
        return 0;

    struct Node *current = head;

    while (current != NULL)
    {

        fwrite(&current->data, sizeof(struct FileCell), 1, fp);
        current = current->next;
    }

    fclose(fp);
    return 1;
}

// Load FAT from disk
struct Node *loadFATFromDisk(const char *filename)
{

    FILE *fp = fopen(filename, "rb");
    if (!fp)
        return NULL;

    struct Node *head = NULL;
    struct FileCell temp;

    while (fread(&temp, sizeof(struct FileCell), 1, fp))
    {
        head = addToBeginning(head, temp);
    }

    fclose(fp);
    return head;
}

// Not sure if this one is correct
// formating the whole file system
struct Node *formatFileSystem(struct Node *head)
{

    printf("Formatting file system... all files deleted!\n");
    freeList(head);
    return NULL; // head = NULL
}

////////////////////////////////////////////////
/////////////// FILEs OPERATIONS ///////////////
////////////////////////////////////////////////

// Writing into file
int writeFileContent(const char *filename, const char *content)
{

    FILE *fp = fopen(filename, "w");
    if (!fp)
        return 0;

    fprintf(fp, "%s", content);
    fclose(fp);
    return 1;
}

// appending to a file
int appendToFile(const char *filename, const char *content)
{

    FILE *fp = fopen(filename, "a");
    if (!fp)
        return 0;

    fprintf(fp, "%s", content);
    fclose(fp);
    return 1;
}

// reading from a file
int readFileContent(const char *filename)
{

    FILE *fp = fopen(filename, "r");
    if (!fp)
        return 0;

    size_t bufSize = 256; // buffer size
    size_t len = 0;       // Everything we have read till now
    char *buffer = malloc(bufSize);

    if (!buffer)
    {
        fclose(fp);
        return 0;
    }

    int c;
    while ((c = fgetc(fp)) != EOF)
    {

        if (len + 1 >= bufSize)
        {
            // if buffer is full make it double
            bufSize *= 2;
            char *temp = realloc(buffer, bufSize);

            if (!temp)
            {
                free(buffer);
                fclose(fp);
                return 0;
            }

            buffer = temp;
        }

        buffer[len++] = (char)c;
    }

    buffer[len] = '\0'; // null terminate

    printf("Content of %s:\n%s\n", filename, buffer);

    free(buffer);
    fclose(fp);
    return 1;
}

// Function to free all nodes in the list
void freeList(struct Node *head)
{
    struct Node *current = head;
    struct Node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

// Helper function to print the list (for testing)
void printList(struct Node *head)
{
    struct Node *current = head;
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

void testFileSystem()
{
    struct Node *head = malloc(sizeof(struct Node));
    struct FileCell cell = {
        .fileName = "testing file 1",
        .location = 134,
        .size = 25};

    head->data = cell;
    head->next = NULL;
    printList(head);

    deleteFile("testing file 1", head);
}

////////////////////////////////////////////////
/////////////// HELPER FUNCTIONS ///////////////
////////////////////////////////////////////////

// Counting files
int getFilesCount(struct Node *head)
{

    int count = 0;
    struct Node *current = head;

    while (current != NULL)
    {

        count++;
        current = current->next;
    }

    return count;
}

// Changing a name of a file
int renameFile(struct Node *head, char *lastName, char *newName)
{
    struct Node *current = head;
    while (current != NULL)
    {

        if (strcmp(current->data.fileName, lastName) == 0)
        {

            strncpy(current->data.fileName, newName, sizeof(current->data.fileName) - 1);
            current->data.fileName[sizeof(current->data.fileName) - 1] = '\0'; // null terminate
            return 1;                                                          // success
        }

        current = current->next;
    }

    return 0; // file not found
}

// Check if file exists
int fileExists(struct Node *head, char *name)
{
    return searchFile(head, name) != -1;
}

// A full checkup I guess?
void printFileSystemStats(struct Node *head, int size)
{
    int used = getTotalUsedSpace(head);
    int free = findFreeSpace(head, size);
    int fileCount = getFilesCount(head);

    printf("=== File System Stats ===\n");
    printf("Total space: %d bytes\n", size);
    printf("Used space : %d bytes\n", used);
    printf("Free space : %d bytes\n", free);
    printf("Files count: %d\n", fileCount);
    printf("=========================\n");
}