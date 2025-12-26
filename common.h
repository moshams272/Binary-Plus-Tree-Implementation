#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 1. ثوابت المشروع (Constants)
#define ORDER 5             // درجة الشجرة (يمكن تغييرها لاحقاً)
#define FILENAME "tree.dat" // اسم الملف الذي سيخزن الشجرة

// 2. هيكل النود (The Node Structure)
typedef struct
{
    int id;       // رقم النود في الملف (للتسهيل أثناء الـ Debugging)
    int is_leaf;  // هل هي ورقة أم نود داخلية؟
    int num_keys; // عدد الأرقام الموجودة حالياً في النود

    int keys[ORDER - 1]; // مصفوفة الأرقام (المفاتيح)

    // في النود الداخلية: هذه تشير إلى النودات الأبناء
    // في النود الورقية: هذه قد لا نستخدمها أو نستخدمها للإشارة للداتا
    int children[ORDER];

    int next_leaf; // مؤشر للنود التالية (مهم جداً للـ B+ Tree Leaf Linking)
    int parent;    // مؤشر للأب (يسهل عملية الـ Splitting جداً)
} Node;

// 3. هيكل المعلومات الأساسية (Metadata)
// هذا سيتم تخزينه في أول مكان في الملف (Block 0)
// لكي نعرف دائماً أين الروت وأين نكتب النود الجديدة
typedef struct
{
    int root_node_id; // أين يقع الروت؟
    int next_free_id; // ما هو رقم النود الفارغة التالية للكتابة فيها؟
} TreeHeader;

#endif