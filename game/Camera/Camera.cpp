#include "Camera.hpp"


Camera::Camera()
{
    aspect = 800.0f/600.0f;
    rotationX = 45.0f;
    rotationY = 45.0f;
//    proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -2.0f, 2.0f);
    proj = glm::perspective(180.0f, aspect, 0.0f, 100.0f);
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
}

Camera::~Camera()
{
    
}

void Camera::Update()
{
    
}

void Camera::Start()
{
    
}

glm::mat4 Camera::GetPVMatrix()
{
    return proj*view;
}
