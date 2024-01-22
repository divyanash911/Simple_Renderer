#pragma once

#include "common.h"
typedef struct BoxNode BoxNode;
extern int total_boxes;
// typedef struct BoxNode BoxNode;
struct BoxNode{

    BoxNode* left;
    BoxNode* right;
    bool isLeaf;
    std::vector<Vector3f> v;
    
    Vector3f min;
    Vector3f max;
};

BoxNode* createBox(std::vector<Vector3f> v);
void divide_box(BoxNode* node);
std::vector<BoxNode*> get_all_boxes(BoxNode* node);
bool slabtest(Ray ray, BoxNode *box);


