#include "FileSystem.h"
#include "CommonTools.h"
// #include "Hardware.h"
// #include "Structure.h"
// #include "interpreter.h"
// #include "GlobalVariables.h"

#include "Runner.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

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