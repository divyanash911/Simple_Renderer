#pragma once

#include "common.h"
#include "texture.h"
#include "box.h"
extern int ray_plane_intersections;
extern int total_faces;
extern int triangles_intersected;
extern int o_edges;
extern int s_edges;
extern int t_edges;
extern int total_faces_f;
extern BoxNode m_root;
struct Surface {
    std::vector<Vector3f> vertices, normals;
    std::vector<Vector3i> indices;
    std::vector<Vector2f> uvs;
    std::vector<Vector3f> box_vertices;
    std::vector<std::vector<Vector3f>> triangles;
    bool isLight;
    uint32_t shapeIdx;
    BoxNode* main_box;
    Vector3f diffuse;
    double alpha;

    Texture diffuseTexture, alphaTexture;

    Interaction rayPlaneIntersect(Ray ray, Vector3f p, Vector3f n);
    Interaction rayTriangleIntersect(Ray ray, Vector3f v1, Vector3f v2, Vector3f v3, Vector3f n);
    Interaction rayIntersect(Ray ray);

private:
    bool hasDiffuseTexture();
    bool hasAlphaTexture();
};



std::vector<Surface> createSurfaces(std::string pathToObj, bool isLight, uint32_t shapeIdx);