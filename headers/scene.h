#pragma once

#include "camera.h"
#include "surface.h"
#include "bvh.h"
extern long long int total_surfaces;
struct Scene {
    std::vector<Surface> surfaces;
    Camera camera;
    Vector2i imageResolution;
    BoxNode* m_root;
    Bvh m_bvh;

    Scene() {};
    Scene(std::string sceneDirectory, std::string sceneJson);
    Scene(std::string pathToJson);
    
    void parse(std::string sceneDirectory, nlohmann::json sceneConfig);

    Interaction rayIntersect(Ray& ray);
};