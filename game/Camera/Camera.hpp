#pragma once
#include "Util.hpp"
#include "GameControl.hpp"
#include "Listener.hpp"

class Camera: public Controller
{
private:
    glm::mat4 m_Proj;
    glm::mat4 m_View;
    glm::mat4 m_PVMatrix;
    float m_RotationX;
    float m_RotationY;
    glm::vec3 m_Position;
    glm::vec3 m_Direction;
    glm::vec3 m_Updir;
    float m_Speed;
    bool m_Moving; //whether the camera is moving or not
    std::set <Direction> m_ActiveDirections;
    
    void Move(glm::vec3 dir, float distance);
    void KeyboardInput (GLFWwindow* window, int key, int scancode, int action, int mods);
    void UpdateMovementInfo(Direction dir, bool active);
    glm::vec3 GetMovementDirection ();

    Listener <GLFWwindow*, int, int, int, int> m_KeyboardInputListener;
public:
    Camera();
    ~Camera();
    void Start() override;
    void Update() override;
    glm::mat4 GetPVMatrix();
    glm::vec3 GetPosition ();
};
