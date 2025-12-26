#include "common.h"
// دوال التعامل مع الملفات (مسؤولية الشخص الثاني)
void write_node(int node_id, Node* node);
void read_node(int node_id, Node* node);
void init_tree_file(); // إنشاء الملف وتصفيره إذا لم يكن موجوداً
TreeHeader read_header();
void update_header(TreeHeader header);

// دوال الشجرة الأساسية (مسؤولية الباقين)
void insert(int key);
void insert_into_parent(int left_id, int key, int right_id);
int search(int key);
void print_tree();


int create_node(Node* node, bool is_leaf) {
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

    if (header.root_node_id == -1) {
        Node root;
        int root_id = create_node(&root, true);
        root.keys[0] = key;
        root.num_keys = 1;
        header.root_node_id = root_id;
        update_header(header);
        write_node(root_id, &root);
        return;
    }

    int leaf_id = find_leaf(key);
    Node leaf;
    read_node(leaf_id, &leaf);

    for (int i = 0; i < leaf.num_keys; i++)
        if (leaf.keys[i] == key) return;

    if (leaf.num_keys < ORDER - 1) {
        insert_into_leaf(&leaf, key);
        write_node(leaf_id, &leaf);
    } else {
        split_leaf(leaf_id, &leaf, key);
    }
}
int search(int key) {
    int leaf_id = find_leaf(key);
    if (leaf_id == -1) return 0;

    Node leaf;
    read_node(leaf_id, &leaf);

    for (int i = 0; i < leaf.num_keys; i++)
        if (leaf.keys[i] == key)
            return 1;

    return 0;
}
void print_tree() {
    TreeHeader header = read_header();
    if (header.root_node_id == -1) {
        printf("Empty tree\n");
        return;
    }

    int q[1024], h = 0, t = 0;
    q[t++] = header.root_node_id;
    q[t++] = -1;

    while (h < t) {
        int id = q[h++];
        if (id == -1) {
            printf("\n");
            if (h < t) q[t++] = -1;
            continue;
        }

        Node n;
        read_node(id, &n);

        printf("[");
        for (int i = 0; i < n.num_keys; i++)
            printf("%d ", n.keys[i]);
        printf("] ");

        if (!n.is_leaf) {
            for (int i = 0; i <= n.num_keys; i++)
                q[t++] = n.children[i];
        }
    }
    printf("\n");
}
