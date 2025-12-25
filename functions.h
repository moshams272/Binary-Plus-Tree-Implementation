#include "common.h"
// دوال التعامل مع الملفات (مسؤولية الشخص الثاني)
void write_node(int node_id, Node* node);
void read_node(int node_id, Node* node);
void init_tree_file(); // إنشاء الملف وتصفيره إذا لم يكن موجوداً
TreeHeader read_header();
void update_header(TreeHeader header);

// دوال الشجرة الأساسية (مسؤولية الباقين)
void insert(int key);
int search(int key);
void delete_key(int key);
void print_tree();