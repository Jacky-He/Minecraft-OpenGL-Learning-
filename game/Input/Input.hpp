#pragma once
#include "Util.hpp"
#include "Event.hpp"

class Input
{
public:
    static void Init(GLFWwindow* window);
    static bool mouseInside;
    static double xPos;
    static double yPos;
    static double xOffset;
    static double yOffset;
    static Listener<GLFWwindow*, int, int, int, int> addKeyboardInputListener(void* obj, const std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)>& listener);
    static Event <GLFWwindow*, int, int, int, int> keyboardInputEvent;
private:
    static void cursorPositionCallback (GLFWwindow* window, double xPos, double yPos);
    static void cursorEnterCallback(GLFWwindow* window, int entered);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
    


