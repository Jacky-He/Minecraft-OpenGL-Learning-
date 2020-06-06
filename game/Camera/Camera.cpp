#include "Camera.hpp"
#include "Input.hpp"

Camera::Camera()
{
    using namespace std::placeholders;
    std::function<void(GLFWwindow*, int, int, int, int)> f = std::bind(&Camera::keyboardInput, *this, _1, _2, _3, _4, _5);
    keyboardInputListener = Input::addKeyboardInputListener(this, f);
    keyboardInputListener.Remove();
    position = glm::vec3(-2, -2, -2);
    aspect = 800.0f/600.0f;
    rotationX = glm::radians(45.0f);
    rotationY = 45.0f;
    proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    view = glm::translate(glm::mat4(1.0f), position);
    PVMatrix = proj*view;
}

Camera::~Camera()
{
    
}

void Camera::Update()
{
    rotationY = (float)-Input::yPos/600*360;
    rotationX = (float)Input::xPos/800*360;
    rotationX = glm::radians(rotationX);
    rotationY = glm::radians(rotationY);
//    proj = glm::perspective(rotationY, aspect, 0.0f, 100.0f);
    view = glm::translate(glm::mat4(1.0f), position);
    view = glm::rotate(view, rotationX, glm::vec3(0, 1, 0));
    view = glm::rotate(view, rotationY, glm::vec3(1, 0, 0 ));
    PVMatrix = proj*view;
}

void Camera::Start()
{
    
}

glm::mat4 Camera::GetPVMatrix()
{
    return PVMatrix;
}

void Camera::Move(int dir, float distance)
{
    
}


void Camera::keyboardInput (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    cout << "camera pressed" << endl;
    if (action != GLFW_PRESS && action != GLFW_RELEASE) return;
    if (key == GLFW_KEY_W || key == GLFW_KEY_UP) Move(1, 0.1f);
    if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) Move(2, 0.1f);
    if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) Move(3, 0.1f);
    if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) Move(4, 0.1f);
}

