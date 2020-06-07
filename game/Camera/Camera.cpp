#include "Camera.hpp"
#include "Input.hpp"
#include "Constants.hpp"

Camera::Camera():m_ActiveDirections()
{
    using namespace std::placeholders;
    std::function<void(GLFWwindow*, int, int, int, int)> f = std::bind(&Camera::KeyboardInput, this, _1, _2, _3, _4, _5);
    m_KeyboardInputListener = Input::addKeyboardInputListener(this, f);
    
    m_Speed = 5.0f; // m/s
    m_Position = glm::vec3(0.0f, 0.0f, -2.0f);
    m_Direction = glm::vec3(0.0f, 0.0f, 1.0f);
    m_Updir = glm::vec3(0.0f, 1.0f, 0.0f);
    m_RotationX = glm::radians(45.0f);
    m_RotationY = glm::radians(45.0f);
    
    m_Proj = glm::perspective(glm::radians(45.0f), Constants::screenAspect, 0.1f, 100.0f);
    m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_Updir);
    m_PVMatrix = m_Proj*m_View;
}

Camera::~Camera()
{
    m_KeyboardInputListener.Remove();
    m_ActiveDirections.clear();
}

void Camera::Update()
{
    if (m_Moving) Move(GetMovementDirection(), m_Speed*Constants::deltaTime);

    m_RotationY = (float)Input::yPos/600.0f*180.0f-90.0f;
    m_RotationX = (float)-Input::xPos/800.0f*360.0f;
    m_RotationX = glm::radians(m_RotationX);
    m_RotationY = glm::radians(m_RotationY);
    
    m_Direction = glm::rotateX(glm::vec3(0.0f, 0.0f, 1.0f), m_RotationY);
    m_Direction = glm::rotateY(m_Direction, m_RotationX);
    m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_Updir);
    m_PVMatrix = m_Proj*m_View;
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

//Movements
void Camera::Move (glm::vec3 dir, float distance)
{
    m_Position += distance*dir;
}

void Camera::KeyboardInput (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    bool active = true;
    if (action == GLFW_RELEASE) active = false;
    if (key == GLFW_KEY_W || key == GLFW_KEY_UP) UpdateMovementInfo(Direction::FORWARD, active);
    if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) UpdateMovementInfo(Direction::BACKWARD, active);
    if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) UpdateMovementInfo(Direction::LEFT, active);
    if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) UpdateMovementInfo(Direction::RIGHT, active);
    if (key == GLFW_KEY_SPACE) UpdateMovementInfo(Direction::UP, active);
    if (key == GLFW_KEY_LEFT_SHIFT) UpdateMovementInfo(Direction::DOWN, active);
}

void Camera::UpdateMovementInfo(Direction dir, bool active)
{
    if (active) m_ActiveDirections.insert(dir);
    else m_ActiveDirections.erase(dir);
    if (m_ActiveDirections.empty()) m_Moving = false;
    else m_Moving = true;
}

glm::vec3 Camera::GetMovementDirection ()
{
    glm::vec3 curr = glm::vec3(0, 0, 0);
    for (Direction each: m_ActiveDirections)
    {
        if (each == Direction::FORWARD) curr += glm::normalize(glm::vec3(m_Direction.x, 0.0f, m_Direction.z));
        else if (each == Direction::BACKWARD) curr -= glm::normalize(glm::vec3(m_Direction.x, 0.0f, m_Direction.z));
        else if (each == Direction::LEFT) curr += glm::normalize(glm::cross(m_Updir, m_Direction));
        else if (each == Direction::RIGHT) curr -= glm::normalize(glm::cross(m_Updir, m_Direction));
        else if (each == Direction::UP) curr += glm::normalize(m_Updir);
        else if (each == Direction::DOWN) curr -= glm::normalize(m_Updir);
    }
    if (abs(glm::length(curr)) < Constants::EPSILONf) return glm::zero<glm::vec3>();
    return glm::normalize(curr);
}
