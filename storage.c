#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h" // Include the shared header file definition

// File pointer for the storage file.
// Declared as 'static' to restrict visibility to this translation unit (encapsulation).
static FILE *fp = NULL;

/**
 * 1. File Initialization
 * Opens the storage file. If it does not exist, creates it and initializes
 * the file header with default values.
 */
void init_tree_file() {
    // Attempt to open the file for binary read/write operations
    fp = fopen(FILENAME, "rb+");

    // If the file does not exist, create a new one
    if (fp == NULL) {
        fp = fopen(FILENAME, "wb+"); // 'wb+' creates a new binary file
        if (fp == NULL) {
            printf("Error: Could not create file!\n");
            exit(1);
        }

        // Initialize the default header metadata
        TreeHeader header;
        header.root_node_id = -1;  // -1 indicates the tree is currently empty
        header.next_free_id = 0;   // The first available slot is index 0
        
        // Write the header to the beginning of the file
        fwrite(&header, sizeof(TreeHeader), 1, fp);
        fflush(fp); // Flush the stream to ensure data is physically written to disk
        printf("New tree file created.\n");
    } else {
        printf("Tree file opened successfully.\n");
    }
}

/**
 * 2. Write Node
 * Writes a specific node structure to the file at the calculated offset.
 * @param node_id: The index/ID where the node should be stored.
 * @param node: Pointer to the node data to be written.
 */
void write_node(int node_id, Node* node) {
    if (fp == NULL) {
        printf("Error: File not open!\n");
        return;
    }

    // Calculate the byte offset for this specific node
    // Offset = Header Size + (Node Index * Node Size)
    long offset = sizeof(TreeHeader) + (long)(node_id * sizeof(Node));

    // Move the file pointer to the calculated offset
    // SEEK_SET starts the offset calculation from the beginning of the file
    fseek(fp, offset, SEEK_SET);

    // Write the node data to the file
    fwrite(node, sizeof(Node), 1, fp);
    
    // Flush to ensure data persistence (prevents data loss if program crashes)
    fflush(fp); 
}

/**
 * 3. Read Node
 * Retrieves node data from the file based on the given node ID.
 * @param node_id: The index of the node to read.
 * @param node: Pointer where the read data will be stored.
 */
void read_node(int node_id, Node* node) {
    if (fp == NULL) return;

    // Calculate the byte offset
    long offset = sizeof(TreeHeader) + (long)(node_id * sizeof(Node));

    // Move pointer to the correct location and read the data
    fseek(fp, offset, SEEK_SET);
    fread(node, sizeof(Node), 1, fp);
}

/**
 * 4. Read Header
 * Reads the tree metadata (e.g., root ID, next free slot) from the file.
 */
TreeHeader read_header() {
    TreeHeader header;
    if (fp == NULL) return header;

    // The header is always located at the absolute beginning of the file (Offset 0)
    fseek(fp, 0, SEEK_SET);
    fread(&header, sizeof(TreeHeader), 1, fp);
    return header;
}

/**
 * 5. Update Header
 * Overwrites the existing header with new metadata.
 * Must be called whenever the root changes or new nodes are allocated.
 */
void update_header(TreeHeader header) {
    if (fp == NULL) return;

    fseek(fp, 0, SEEK_SET);
    fwrite(&header, sizeof(TreeHeader), 1, fp);
    fflush(fp);
}

/**
 * 6. Delete Node (Soft Delete)
 * Clears the data of a specific node.
 * Note: Physically shifting data in binary files is inefficient. 
 * Instead, we overwrite the location with an empty/zeroed structure.
 */
void delete_node(int node_id) {
    Node empty_node;
    
    // Zero out the memory structure using memset
    memset(&empty_node, 0, sizeof(Node));
    
    // Mark ID as -1 to explicitly indicate a deleted or empty state
    empty_node.id = -1; 

    // Overwrite the existing data with the empty node structure
    write_node(node_id, &empty_node);
    
    printf("Node %d has been cleared (soft deleted).\n", node_id);
}

/*
[ FILE ON DISK ]
-----------------------------------------------------------------
|  HEADER  |  NODE 0  |  NODE 1  |  NODE 2  | ... |  NODE N  |
-----------------------------------------------------------------
^          ^          ^
Offset 0   Offset X   Offset Y
*/