#define GLEW_STATIC
#include <iostream>
#include "GameControl.hpp"
#include "Util.hpp"
#include "SceneRenderer.hpp"
#include "Input.hpp"
#include "Constants.hpp"
#include "Textures.hpp"
#include "Constellations.hpp"
#include "Map.hpp"
#include "Chunk.hpp"
#include "Player.hpp"

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
    
    //init input and crosshair stuff
    Input::Init(window);
    //init input and crosshair finished


    //load textures
    Textures::LoadTextures();
    //load textures finished
    
    //Generate map
    Map::SetCurrSeed(100);
    //Generate map finished
    
    //set up Chunk
    Chunk::SetUp();
    //set up Chunk finished
    
    GameControl::sharedInstance().Start();
    Player::Init();
    Player* player = new Player("Jacky", glm::vec3(0, 180, 0));
    player -> Start();
    SceneRenderer* renderer = new SceneRenderer(128);
    renderer -> SetCamera(player -> m_FirstPersonCamera);
    player -> SetSceneRenderer (renderer);
    renderer -> SetConstellations (new Constellations(0.2));
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    double timeFromStart = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double timeCurr = glfwGetTime(); //in seconds
        Constants::deltaTime = timeCurr - timeFromStart;
        timeFromStart = timeCurr;
        
//        GameControl::sharedInstance().Update();
        renderer -> Render();
        player -> Update();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    delete player;
    delete renderer;
}
