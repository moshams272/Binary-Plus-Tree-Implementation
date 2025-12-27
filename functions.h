#include "common.h"
void write_node(int node_id, Node* node);
void read_node(int node_id, Node* node);
void init_tree_file();
TreeHeader read_header();
void update_header(TreeHeader header);
void insert(int key);
void insert_into_parent(int left_id, int key, int right_id);
int search(int key);
void print_tree();
int create_node(Node* node, bool is_leaf);
int find_leaf(int key);
void insert_into_leaf(Node* leaf, int key);
void split_internal(int node_id, Node* node, int key, int right_id);
void insert_into_parent(int left_id, int key, int right_id);
void split_leaf(int leaf_id, Node* leaf, int key);
void insert(int key);
int search(int key);
void print_tree();