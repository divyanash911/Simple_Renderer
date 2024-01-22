#pragma once

#include "common.h"
#include "surface.h"
extern int called_count;
typedef struct BvhNode* Bvh;
struct BvhNode {
    BvhNode *left;
    BvhNode *right;
    Vector3f min;
    Vector3f max;
    std::vector<Surface> surfaces;
    std::vector<Vector3f> v;
};

Bvh createBvh(std::vector<Surface> surfaces);
void divideBvh(Bvh root);
Interaction rayIntersectBvh(Bvh root, Ray &ray);