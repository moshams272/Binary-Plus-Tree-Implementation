#include <iostream>
#include <queue>
#include "../functions.h" 
using namespace std;


int create_node(Node* node, int is_leaf) {
    TreeHeader header = read_header();
    int id = header.next_free_id++;

    node->id = id;
    node->is_leaf = is_leaf;
    node->num_keys = 0;
    node->parent = -1;
    node->next_leaf = -1;

    for (int i = 0; i < ORDER - 1; i++)
        node->keys[i] = 0;

    for (int i = 0; i < ORDER; i++)
        node->children[i] = -1;

    update_header(header);
    write_node(id, node);
    return id;
}
int find_leaf(int key) {
    TreeHeader header = read_header();
    if (header.root_node_id == -1) return -1;

    int curr = header.root_node_id;
    Node node;
    read_node(curr, &node);

    while (!node.is_leaf) {
        int i = 0;
        while (i < node.num_keys && key >= node.keys[i])
            i++;
        curr = node.children[i];
        read_node(curr, &node);
    }
    return curr;
}
void insert_into_leaf(Node* leaf, int key) {
    int i = leaf->num_keys - 1;
    while (i >= 0 && leaf->keys[i] > key) {
        leaf->keys[i + 1] = leaf->keys[i];
        i--;
    }
    leaf->keys[i + 1] = key;
    leaf->num_keys++;
}
void split_internal(int node_id, Node* node, int key, int right_id) {
    int temp_keys[ORDER];
    int temp_children[ORDER + 1];

    int i = 0, j = 0;
    while (i < node->num_keys && node->keys[i] < key) {
        temp_keys[j] = node->keys[i];
        temp_children[j] = node->children[i];
        i++; j++;
    }

    temp_keys[j] = key;
    temp_children[j] = node->children[i];
    temp_children[j + 1] = right_id;
    j++;

    while (i < node->num_keys) {
        temp_keys[j] = node->keys[i];
        temp_children[j + 1] = node->children[i + 1];
        i++; j++;
    }

    int split = ORDER / 2;
    int promote = temp_keys[split];

    Node right;
    int right_id_new = create_node(&right, false);
    right.parent = node->parent;

    node->num_keys = split;
    for (i = 0; i < split; i++) {
        node->keys[i] = temp_keys[i];
        node->children[i] = temp_children[i];
    }
    node->children[split] = temp_children[split];

    right.num_keys = 0;
    for (i = split + 1; i < ORDER; i++) {
        right.keys[right.num_keys] = temp_keys[i];
        right.children[right.num_keys] = temp_children[i];
        right.num_keys++;
    }
    right.children[right.num_keys] = temp_children[ORDER];

    for (i = 0; i <= right.num_keys; i++) {
        Node child;
        read_node(right.children[i], &child);
        child.parent = right_id_new;
        write_node(child.id, &child);
    }

    write_node(node_id, node);
    write_node(right_id_new, &right);

    insert_into_parent(node_id, promote, right_id_new);
}
void insert_into_parent(int left_id, int key, int right_id) {
    Node left;
    read_node(left_id, &left);

    if (left.parent == -1) {
        Node root;
        int root_id = create_node(&root, false);

        root.keys[0] = key;
        root.children[0] = left_id;
        root.children[1] = right_id;
        root.num_keys = 1;

        left.parent = root_id;
        Node right;
        read_node(right_id, &right);
        right.parent = root_id;

        TreeHeader header = read_header();
        header.root_node_id = root_id;
        update_header(header);

        write_node(left_id, &left);
        write_node(right_id, &right);
        write_node(root_id, &root);
        return;
    }

    Node parent;
    read_node(left.parent, &parent);

    if (parent.num_keys < ORDER - 1) {
        int i = parent.num_keys - 1;
        while (i >= 0 && parent.keys[i] > key) {
            parent.keys[i + 1] = parent.keys[i];
            parent.children[i + 2] = parent.children[i + 1];
            i--;
        }
        parent.keys[i + 1] = key;
        parent.children[i + 2] = right_id;
        parent.num_keys++;

        Node right;
        read_node(right_id, &right);
        right.parent = parent.id;

        write_node(parent.id, &parent);
        write_node(right_id, &right);
    } else {
        split_internal(parent.id, &parent, key, right_id);
    }
}
void split_leaf(int leaf_id, Node* leaf, int key) {
    int temp[ORDER];
    int i = 0, j = 0;

    while (i < leaf->num_keys && leaf->keys[i] < key)
        temp[j++] = leaf->keys[i++];

    temp[j++] = key;

    while (i < leaf->num_keys)
        temp[j++] = leaf->keys[i++];

    int split = ORDER / 2;

    Node new_leaf;
    int new_id = create_node(&new_leaf, true);

    leaf->num_keys = split;
    for (i = 0; i < split; i++)
        leaf->keys[i] = temp[i];

    new_leaf.num_keys = ORDER - split;
    for (i = 0; i < new_leaf.num_keys; i++)
        new_leaf.keys[i] = temp[i + split];

    new_leaf.next_leaf = leaf->next_leaf;
    leaf->next_leaf = new_id;
    new_leaf.parent = leaf->parent;

    write_node(leaf_id, leaf);
    write_node(new_id, &new_leaf);

    insert_into_parent(leaf_id, new_leaf.keys[0], new_id);
}
void insert(int key) {
    TreeHeader header = read_header();

    // Case 1: The tree is completely empty
    if (header.root_node_id == -1) {
        Node root;
        // create_node automatically increments next_free_id on disk
        int root_id = create_node(&root, true); 
        
        root.keys[0] = key;
        root.num_keys = 1;
        
        // Re-read the header to get the updated next_free_id
        // before setting the root_id and saving.
        header = read_header(); 
        header.root_node_id = root_id;
        update_header(header);
        
        write_node(root_id, &root);
        cout<<"<h3 style= 'color:green;'>Inserted Correctly</h3>";
        return;
    }

    // Case 2: The tree is not empty (Standard Logic)
    int leaf_id = find_leaf(key);
    Node leaf;
    read_node(leaf_id, &leaf);

    for (int i = 0; i < leaf.num_keys; i++)
        if (leaf.keys[i] == key){
            cout<<"<h3 style= 'color:red;'>Can't insert cause duplicate</h3>";
            return; // duplicate check
        } 

    if (leaf.num_keys < ORDER - 1) {
        insert_into_leaf(&leaf, key);
        write_node(leaf_id, &leaf);
    } else {
        split_leaf(leaf_id, &leaf, key);
    }
    cout<<"<h3 style= 'color:green;'>Inserted Correctly</h3>";
}
int search(int key) {
    int leaf_id = find_leaf(key);
    if (leaf_id == -1) return 0;

    Node leaf;
    read_node(leaf_id, &leaf);

    for (int i = 0; i < leaf.num_keys; i++)
        if (leaf.keys[i] == key){
            cout<<"<h3 style= 'color:green;'>Found</h3>";
            return 1;
        }
        cout<<"<h3 style= 'color:red;'>Not Found</h3>";
    return 0;
}

void print_tree() {
    // 1. Read the header to find the REAL root ID
    TreeHeader header = read_header();
    
    cout << "<style>"
         << ".tree-wrapper { display: flex; flex-direction: column; align-items: center; margin-top: 20px; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; }"
         << ".level { display: flex; justify-content: center; gap: 20px; margin-bottom: 40px; position: relative; }"
         << ".node { "
         << "   border: 2px solid #444; background: #fff; border-radius: 8px; "
         << "   min-width: 80px; text-align: center; box-shadow: 0 4px 6px rgba(0,0,0,0.1); position: relative; z-index: 1;"
         << "}"
         << ".node.internal { background: #fdf2e9; border-color: #d35400; }" 
         << ".node.leaf { background: #eafaf1; border-color: #27ae60; }" 
         << ".node-id { font-size: 10px; padding: 2px; color: #fff; background: #444; border-radius: 6px 6px 0 0; }"
         << ".node-keys { padding: 10px; font-weight: bold; font-size: 16px; color: #333; }"
         << ".arrow-down { font-size: 24px; color: #ccc; margin-top: -35px; margin-bottom: 5px; }"
         << ".empty-msg { color: #7f8c8d; font-style: italic; margin-top: 20px; }"
         << "</style>";

    cout << "<div class='tree-wrapper'>";
    cout << "<h3>🌳 Dynamic Tree Visualization</h3>";

    // 2. Check if the tree is empty
    if (header.root_node_id == -1) {
        cout << "<div class='empty-msg'>The Tree is currently empty. Insert a value to begin.</div>";
        cout << "</div>";
        return;
    }

    queue<int> q;
    q.push(header.root_node_id); // Start from the ACTUAL root, not always 0

    while (!q.empty()) {
        int level_size = q.size();
        
        cout << "<div class='level'>"; 
        for (int i = 0; i < level_size; i++) {
            int current_id = q.front();
            q.pop();

            Node current_node;
            read_node(current_id, &current_node); // Read from file

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

            // If internal, add children to queue
            if (!current_node.is_leaf) {
                // Internal nodes have (num_keys + 1) children
                for (int child_idx = 0; child_idx <= current_node.num_keys; child_idx++) {
                    q.push(current_node.children[child_idx]);
                }
            }
        }
        cout << "</div>";

        // Only draw arrow if there is another level coming
        if (!q.empty()) {
            cout << "<div class='arrow-down'>⬇</div>";
        }
    }

    cout << "</div>";
}