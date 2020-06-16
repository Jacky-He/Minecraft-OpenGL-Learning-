#include "Camera.hpp"
#include "Input.hpp"
#include "Constants.hpp"

Camera::Camera(glm::vec3 position):m_Position(position)
{
    m_Direction = glm::vec3(0.0f, 0.0f, 1.0f);
    m_Updir = glm::vec3(0.0f, 1.0f, 0.0f);
    m_RotationX = glm::radians(0.0f);
    m_RotationY = glm::radians(0.0f);
    
    m_Proj = glm::perspective(glm::radians(45.0f), Constants::screenAspect, 0.1f, 180.0f);
    m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_Updir);
    m_PVMatrix = m_Proj*m_View;
}

Camera::~Camera()
{
   
}

void Camera::Update()
{
    UpdateMatrix();
}

void Camera::Start()
{
    
}

glm::mat4 Camera::GetPVMatrix()
{
    return m_PVMatrix;
}

glm::vec3 Camera::GetPosition ()
{
    return m_Position;
}

glm::vec3 Camera::GetDirection()
{
    return m_Direction;
}

void Camera::SetPosition(glm::vec3 position)
{
    m_Position = position;
    UpdateMatrix();
}

void Camera::UpdateMatrix()
{
    m_RotationY = (float)Input::yPos/600.0f*180.0f-90.0f;
    m_RotationX = (float)-Input::xPos/800.0f*360.0f;
    m_RotationX = glm::radians(m_RotationX);
    m_RotationY = glm::radians(m_RotationY);
    
    m_Direction = glm::rotateX(glm::vec3(0.0f, 0.0f, 1.0f), m_RotationY);
    m_Direction = glm::rotateY(m_Direction, m_RotationX);
    m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_Updir);
    m_PVMatrix = m_Proj*m_View;
}

