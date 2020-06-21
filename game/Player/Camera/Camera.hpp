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
    glm::mat4 m_VPMatrix;
    float m_RotationX;
    float m_RotationY;
    glm::vec3 m_Position;
    glm::vec3 m_Updir;
    
    void UpdateMatrix();
public:
    
    float m_DeltaPosition;
    
    Camera(glm::vec3 position);
    ~Camera();
    void Start() override;
    void Update() override;
    glm::mat4 GetPVMatrix();
    glm::mat4 GetVPMatrix();
    glm::vec3 GetPosition ();
    glm::vec3 GetDirection();
    glm::vec3 m_Direction;
    glm::vec3 m_RightDirection;
    void SetPosition(glm::vec3 position);
    
    
    void SetSpeed(float speed);
};
