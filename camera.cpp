#include "camera.h"
Camera::Camera(Vector3f from, Vector3f to, Vector3f up, float fieldOfView, Vector2i imageResolution)
    : from(from),
    to(to),
    up(up),
    fieldOfView(fieldOfView),
    imageResolution(imageResolution)
{
    this->aspect = imageResolution.x / float(imageResolution.y);

    // Determine viewport dimensions in 3D
    double fovRadians = fieldOfView * M_PI / 180.f;
    double h = std::tan(fovRadians / 2.f);
    double viewportHeight = 2.f * h * this->focusDistance;
    double viewportWidth = viewportHeight * this->aspect;

    // Calculate basis vectors of the camera for the given transform
    this->w = Normalize(this->from - this->to);
    this->u = Normalize(Cross(up, this->w));
    this->v = Normalize(Cross(this->w, this->u));
    Vector3f viewportU = viewportWidth * this->u;
    Vector3f viewportV = viewportHeight * (-this->v);

    this->pixelDeltaU = viewportU / double(imageResolution.x);
    this->pixelDeltaV = viewportV / double(imageResolution.y);
    // std::cout << pixelDeltaU.x << std::endl;

    // Upper left
    this->upperLeft = from - this->w * this->focusDistance - viewportU / 2.f - viewportV / 2.f;
}

Ray Camera::generateRay(int x, int y)
{
    Vector3f pixelCenter;
    pixelCenter = this->upperLeft + (x+0.5f) * this->pixelDeltaU + (y+0.5f)* this->pixelDeltaV;
    Vector3f direction = Normalize(pixelCenter - this->from);
    return Ray(this->from, direction);
}