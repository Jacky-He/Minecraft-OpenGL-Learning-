#include "SceneRenderer.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"


SceneRenderer::SceneRenderer()
{
    camera = nullptr;
}

SceneRenderer::~SceneRenderer()
{
    
}


void SceneRenderer::Render()
{
    float positions [] = {
        -0.5f, -0.5f, -0.5f, //front
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        
        -0.5f, -0.5f, 0.5f, //back
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        
        0.5f, -0.5f, -0.5f, //right
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        
        -0.5f, -0.5f, 0.5f, //left
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        
        -0.5f, 0.5f, -0.5f, //top
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        
        -0.5f, -0.5f, -0.5f, //bottom
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f
    };
    
    unsigned int indices [] = {
        0, 1, 2,
        2, 3, 0,
        
        4, 5, 6,
        6, 7, 4,
        
        8, 9, 10,
        10, 11, 8,
        
        12, 13, 14,
        14, 15, 12,
        
        16, 17, 18,
        18, 19, 16,
        
        20, 21, 22,
        22, 23, 20
    };
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    VertexArray va;
    VertexBuffer vb (positions, sizeof(positions));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    va.AddBuffer(vb, layout);
    IndexBuffer ib (indices, 36);
    
    Shader shader ("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/shaders/cube.shader");
    shader.SetUniform4f("u_Color", 1, 1, 1, 1);
    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0));
    shader.SetUniformMat4f("u_MVP", camera->GetPVMatrix()*model);
    
    Renderer renderer;
    renderer.Draw(va, ib, shader);
}


void SceneRenderer::SetCamera(Camera* camera)
{
    this -> camera = camera;
}
