#include <iostream>
#include <queue>
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
Node mock_read_node(int id) {
    Node n;
    n.id = id;
    n.num_keys = 2;
    
    if (id == 0) { 
        n.is_leaf = false;
        n.keys[0] = 15; n.keys[1] = 40;
        n.children[0] = 1;
        n.children[1] = 2;
        n.children[2] = 3;
        n.num_keys = 2; 
    } else { 
        n.is_leaf = true;
        n.parent = 0;
        if(id == 1) { n.keys[0] = 5;  n.keys[1] = 10; }
        if(id == 2) { n.keys[0] = 20; n.keys[1] = 30; }
        if(id == 3) { n.keys[0] = 50; n.keys[1] = 60; }
    }
    return n;
}

void print_tree() {
    cout << "<style>"
         << ".tree-wrapper { display: flex; flex-direction: column; align-items: center; margin-top: 20px; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; }"
         << ".level { display: flex; justify-content: center; gap: 20px; margin-bottom: 40px; position: relative; }"
         << ".node { "
         << "   border: 2px solid #444; background: #fff; border-radius: 8px; "
         << "   min-width: 80px; text-align: center; box-shadow: 0 4px 6px rgba(0,0,0,0.1); position: relative; z-index: 1;"
         << "}"
         << ".node.internal { background: #fdf2e9; border-color: #d35400; }" // لون مختلف للـ Internal
         << ".node.leaf { background: #eafaf1; border-color: #27ae60; }" // لون مختلف للـ Leaf
         << ".node-id { font-size: 10px; padding: 2px; color: #fff; background: #444; border-radius: 6px 6px 0 0; }"
         << ".node-keys { padding: 10px; font-weight: bold; font-size: 16px; color: #333; }"
         << ".arrow-down { font-size: 24px; color: #ccc; margin-top: -35px; margin-bottom: 5px; }"
         << "</style>";

    cout << "<div class='tree-wrapper'>";
    cout << "<h3>🌳 Dynamic Tree Visualization</h3>";

    queue<int> q;
    
    int root_id = 0;
    q.push(root_id);

    while (!q.empty()) {
        int level_size = q.size();
        
        cout << "<div class='level'>"; 
        for (int i = 0; i < level_size; i++) {
            int current_id = q.front();
            q.pop();

            // Exchange that => read_node(current_id, &current_node);
            Node current_node = mock_read_node(current_id); 

            string typeClass = current_node.is_leaf ? "leaf" : "internal";
            cout << "<div class='node " << typeClass << "'>";
            cout << "<div class='node-id'>Page " << current_node.id << "</div>";

            cout << "<div class='node-keys'>";
            for(int k=0; k < current_node.num_keys; k++) {
                cout << current_node.keys[k];
                if(k < current_node.num_keys - 1) cout << " | ";
            }
            cout << "</div>";
            cout << "</div>";
            if (!current_node.is_leaf) {
                for (int child_idx = 0; child_idx <= current_node.num_keys; child_idx++) {
                    q.push(current_node.children[child_idx]);
                }
            }
        }
        cout << "</div>";

        if (!q.empty()) {
            cout << "<div class='arrow-down'>⬇</div>";
        }
    }

    cout << "</div>";
}