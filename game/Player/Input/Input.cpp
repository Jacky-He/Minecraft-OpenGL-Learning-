#include "Input.hpp"

bool Input::mouseInside = false;
double Input::xPos = 0;
double Input::yPos = 0;
double Input::xOffset = 0;
double Input::yOffset = 0;

Event <GLFWwindow*, int, int, int, int> Input::keyboardInputEvent = Event <GLFWwindow*, int, int, int, int>();

void Input::Init (GLFWwindow* window)
{
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //HIDDEN, DISABLED
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
    glfwSetCursorEnterCallback(window, cursorEnterCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
}

void Input::cursorPositionCallback (GLFWwindow* window, double xPos, double yPos)
{
    if (!Input::mouseInside) return;
    Input::xPos = xPos;
    Input::yPos = yPos;
}

void Input::cursorEnterCallback(GLFWwindow* window, int entered)
{
    Input::mouseInside = entered;
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    
}

void Input::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    Input::xOffset = xOffset;
    Input::yOffset = yOffset;
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    keyboardInputEvent.Trigger(window, key, scancode, action, mods);
}

Listener<GLFWwindow*, int, int, int, int> Input::addKeyboardInputListener(void* obj, const std::function<void(GLFWwindow*, int, int, int, int)>& listener)
{
    return keyboardInputEvent.AddEventListener(obj, listener);
}
