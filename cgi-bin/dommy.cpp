#include <iostream>
#include "../functions.h" 
using namespace std;

void write_node(int node_id, Node* node) {}
void read_node(int node_id, Node* node) {}
void init_tree_file() { cout << "<p>Mock: File Initialized</p>"; }
TreeHeader read_header() { TreeHeader h; return h; }
void update_header(TreeHeader header) { }

void insert(int key) {
    cout << "<div style='background:#e6fffa; padding:10px; border:1px solid teal;'>";
    cout << "✅ <b>Mock Logic:</b> Inserted key (" << key << ") successfully.";
    cout << "</div>";
}

int search(int key) {
    cout << "<div style='background:#fffbea; padding:10px; border:1px solid orange;'>";
    cout << "🔍 <b>Mock Logic:</b> Searching for key (" << key << ")... Found at Index 1.";
    cout << "</div>";
    return 1;
}

void delete_key(int key) {
    cout << "<div style='background:#ffe6e6; padding:10px; border:1px solid red;'>";
    cout << "🗑️ <b>Mock Logic:</b> Successfully deleted key (" << key << ") from the tree.";
    cout << "</div>";
}

void print_tree() {
    cout << "<pre> [Root] -> [Leaf 1] -> [Leaf 2] (Visualizing Tree...) </pre>";
}