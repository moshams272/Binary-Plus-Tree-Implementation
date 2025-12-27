#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 1. System Constants & Configuration
#define PAGE_SIZE 4096      // Fixed page size in bytes (aligns with disk blocks)
#define ORDER 5             // Tree Order (Branching Factor). Defines max children.
#define FILENAME "tree.dat" // The binary file name used for persistence

// 2. Node Structure Definition
typedef struct {
    int id;             // Unique Node ID (index in file). Used for referencing and debugging.
    int is_leaf;       // Flag indicating if this is a Leaf node or an Internal node.
    int num_keys;       // Current count of active keys in this node.
    
    int keys[ORDER - 1]; // Array storing the keys/values.
    
    // Child Pointers:
    // - In Internal Nodes: Stores IDs of child nodes.
    // - In Leaf Nodes: Can be used to store record pointers (depending on implementation).
    int children[ORDER];    
    
    int next_leaf;      // Sibling pointer to the next leaf. Critical for B+ Tree range queries.
    int parent;         // Pointer to the parent node. Facilitates upward traversal during splits.

    // Padding / Alignment:
    // Ensures the total struct size is exactly 4096 bytes (PAGE_SIZE).
    // This is crucial for calculating precise offsets during 'fseek' operations.
    char padding[PAGE_SIZE - sizeof(int)*5 - sizeof(int)*(ORDER-1) - sizeof(int)*ORDER]; 

} Node;

// 3. File Metadata (Header)
// Reserved for the first block (Block 0) of the file.
// Stores essential state information such as the Root location and Allocator status.
typedef struct {
    int root_node_id;       // The ID (offset index) of the current Root node.
    int next_free_id;       // The ID of the next available slot for writing a new node.
} TreeHeader;

#endif