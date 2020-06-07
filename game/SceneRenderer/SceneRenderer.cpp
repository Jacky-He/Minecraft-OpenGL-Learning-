#include "SceneRenderer.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "Block.hpp"


SceneRenderer::SceneRenderer()
{
    camera = nullptr;
}

SceneRenderer::~SceneRenderer()
{
    
}


void SceneRenderer::Render()
{
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Renderer renderer;
    
    Block grassBlock (glm::vec3(0, 0, 0), BlockType::GRASS);
    grassBlock.Draw(renderer, camera);
    for (int i = 1; i < 100; i++)
    {
        grassBlock.m_Position = glm::vec3(0, i, i);
        grassBlock.Draw(renderer, camera);
    }
}


void SceneRenderer::SetCamera(Camera* camera)
{
    this -> camera = camera;
}
