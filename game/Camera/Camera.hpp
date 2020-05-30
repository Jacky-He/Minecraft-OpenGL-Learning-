#pragma once
#include "GameControl.hpp"
#include "Util.hpp"
#include "../../res/libs/glm/glm.hpp"
#include "../../res/libs/glm/gtc/matrix_transform.hpp"


class Camera: public Controller
{
private:
    glm::mat4 proj;
    glm::mat4 view;
    float rotationX;
    float rotationY;
    float aspect;
    
public:
    Camera();
    ~Camera();
    void Start() override;
    void Update() override;
    
    glm::mat4 GetPVMatrix();
};
