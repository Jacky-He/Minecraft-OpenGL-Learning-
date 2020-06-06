#pragma once
#include "Util.hpp"
#include "GameControl.hpp"
#include "Listener.hpp"

class Camera: public Controller
{
private:
    glm::mat4 proj;
    glm::mat4 view;
    glm::mat4 PVMatrix;
    float rotationX;
    float rotationY;
    float aspect;
    glm::vec3 position;
    void Move(int dir, float distance);
    void keyboardInput (GLFWwindow* window, int key, int scancode, int action, int mods);
    
    Listener <GLFWwindow*, int, int, int, int> keyboardInputListener;
public:
    Camera();
    ~Camera();
    void Start() override;
    void Update() override;
    glm::mat4 GetPVMatrix();
};
