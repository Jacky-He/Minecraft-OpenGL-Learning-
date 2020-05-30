#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GameControl.hpp"
#include "Util.hpp"
#include "Camera.hpp"
#include "SceneRenderer.hpp"


const GLint WIDTH = 800, HEIGHT = 600;

int main ()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //needed for mac
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //prevent from resizing
    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learn Open GL", nullptr, nullptr);
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    //if window is not successfully created, terminate
    if (window == nullptr)
    {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    glfwSwapInterval(1);
    glViewport(0, 0, screenWidth, screenHeight);
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    GameControl::sharedInstance().Start();
    Camera* camera = new Camera();
    SceneRenderer* renderer = new SceneRenderer();
    renderer -> SetCamera(*camera);
    
    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
//        GameControl::sharedInstance().Update();
        camera -> Update();
        renderer -> Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    delete camera;
    delete renderer;
}
