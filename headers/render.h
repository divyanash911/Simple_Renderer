#pragma once

#include "scene.h"
#include "bvh.h"
struct Integrator {
    Integrator(Scene& scene);

    long long render();

    Scene scene;
    Texture outputImage;
};