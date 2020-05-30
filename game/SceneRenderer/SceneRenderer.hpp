#pragma once
#include "Camera.hpp"

class SceneRenderer
{
private:
    Camera camera;
public:
    SceneRenderer();
    ~SceneRenderer();
    void Render();
    void SetCamera(const Camera& camera);
};
