#define GLEW_STATIC
#include <iostream>
#include "GameControl.hpp"
#include "Util.hpp"
#include "SceneRenderer.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "Constants.hpp"
#include "Textures.hpp"
#include "Map.hpp"
#include "Chunk.hpp"

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
    
    Input::Init(window);

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
    
    
    //load textures
    Textures::LoadTextures();
    //load textures finished
    
    //Generate map
    Map* worldMap = new Map();
    Map::SetCurrMap(worldMap);
    //Generate map finished
    
    //set up Chunk
    Chunk::SetUp();
    //set up Chunk finished
    
    GameControl::sharedInstance().Start();
    Camera* camera = new Camera();
    camera -> Start();
    SceneRenderer* renderer = new SceneRenderer(112);
    renderer -> SetCamera(camera);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    double timeFromStart = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double timeCurr = glfwGetTime(); //in seconds
        Constants::deltaTime = timeCurr - timeFromStart;
        timeFromStart = timeCurr;
        GLCall(glClearColor(124/255.05,204/255.0,239/255.0, 1.0));
        GLCall(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));
        
//        GameControl::sharedInstance().Update();
        camera -> Update();
        renderer -> Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    delete camera;
    delete renderer;
}
