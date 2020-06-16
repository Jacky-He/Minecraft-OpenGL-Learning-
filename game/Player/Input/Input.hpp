#pragma once
#include "Util.hpp"
#include "Event.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

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
    
    //cross hair
    static std::unique_ptr<VertexArray> s_VAO;
    static std::unique_ptr<VertexBuffer> s_VBO;
    static std::unique_ptr<IndexBuffer> s_IBO;
    static std::unique_ptr<Shader> s_Shader;
    static std::unique_ptr<Renderer> s_Renderer;
    
    static float s_CrossHairVertexPositions [24];
    static unsigned int s_CrossHairVertexIndices [6];
    static Texture* s_CrossHairTex;
    
    static void DrawCrossHair();
    
private:
    static void cursorPositionCallback (GLFWwindow* window, double xPos, double yPos);
    static void cursorEnterCallback(GLFWwindow* window, int entered);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
    


