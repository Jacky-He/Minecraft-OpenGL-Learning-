#include "Input.hpp"
#include "Constants.hpp"
#include "Textures.hpp"

bool Input::mouseInside = false;
double Input::xPos = 0;
double Input::yPos = 0;
double Input::xOffset = 0;
double Input::yOffset = 0;

Event <GLFWwindow*, int, int, int, int> Input::keyboardInputEvent = Event <GLFWwindow*, int, int, int, int>();

std::unique_ptr<VertexArray> Input::s_VAO = nullptr;
std::unique_ptr<VertexBuffer> Input::s_VBO = nullptr;
std::unique_ptr<IndexBuffer> Input::s_IBO = nullptr;
std::unique_ptr<Shader> Input::s_Shader = nullptr;
std::unique_ptr<Renderer> Input::s_Renderer = nullptr;

Texture* Input::s_CrossHairTex = nullptr;

float Input::s_CrossHairVertexPositions [] = {
    -10.0f, -10.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -10.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    10.0f, 10.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    10.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f
};

unsigned int Input::s_CrossHairVertexIndices [] = {
    0, 1, 2,
    2, 3, 0
};


void Input::Init (GLFWwindow* window)
{
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //HIDDEN, DISABLED
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
    glfwSetCursorEnterCallback(window, cursorEnterCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPos(window, Constants::screenWidth/2.0f, Constants::screenHeight/2.0f);
    
    s_VAO = std::make_unique<VertexArray> ();
    s_VBO = std::make_unique<VertexBuffer>(s_CrossHairVertexPositions, sizeof(s_CrossHairVertexPositions), GL_STATIC_DRAW);
    s_IBO = std::make_unique<IndexBuffer>(s_CrossHairVertexIndices, 6, GL_STATIC_DRAW);
    s_Shader = std::make_unique<Shader>("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/shaders/cube.shader");
    s_Renderer = std::make_unique<Renderer>();
    
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(1);
    s_VAO -> AddBuffer(*s_VBO, layout);

    int arr [1] = {0};
    s_Shader -> SetUniformiv("u_Textures", arr, 1);
    glm::mat4 proj = glm::ortho(-Constants::screenWidth/2.0f, Constants::screenWidth/2.0f, -Constants::screenHeight/2.0f, Constants::screenHeight/2.0f, -1.0f, 1.0f);
    s_Shader -> SetUniformMat4f("u_MVP", proj);
}

void Input::cursorPositionCallback (GLFWwindow* window, double xPos, double yPos)
{
    if (!Input::mouseInside) return;
    //clamp xpos and ypos
    
    Input::xPos = xPos;
    Input::yPos = std::max(std::min(yPos, double(Constants::screenHeight)), 0.0);
    
    //use only if want to mod this, kinda costly
//    double mod = double(Constants::screenWidth);
//    Input::xPos = std::fmod(std::fmod(xPos, mod) + mod, mod);
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
    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

Listener<GLFWwindow*, int, int, int, int> Input::addKeyboardInputListener(void* obj, const std::function<void(GLFWwindow*, int, int, int, int)>& listener)
{
    return keyboardInputEvent.AddEventListener(obj, listener);
}

void Input::DrawCrossHair ()
{
    if (s_CrossHairTex == nullptr) s_CrossHairTex = Textures::GetTexture("crosshair");
    s_CrossHairTex -> Bind(0);
    s_Renderer -> Draw(*s_VAO, *s_IBO, *s_Shader, 6);
}
