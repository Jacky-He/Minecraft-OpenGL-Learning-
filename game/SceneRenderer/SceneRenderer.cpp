#include "SceneRenderer.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "Block.hpp"


SceneRenderer::SceneRenderer(int radius):m_Radius(radius), m_Chunks(), m_ChunkCoords()
{
    camera = nullptr;
}

SceneRenderer::~SceneRenderer()
{
    
}

void SceneRenderer::InitChunk(std::pair <int, int> position)
{
    m_Chunks.insert(new Chunk(position));
    m_ChunkCoords.insert(position);
}

void SceneRenderer::DeleteChunk(Chunk* chunk)
{
    std::pair <int, int> temppos = chunk -> GetPosition();
    m_Chunks.erase(chunk);
    delete chunk;
    m_ChunkCoords.erase(temppos);
}

void SceneRenderer::UpdateChunks()
{
    glm::vec3 campos = camera -> GetPosition();
    //add chunks
    for (int x = -m_Radius; x < m_Radius; x += 16)
    {
        for (int z = -m_Radius; z < m_Radius; z += 16)
        {
            int targetx = campos.x + x;
            int targetz = campos.z + z;
            targetx = targetx/16*16;
            targetz = targetz/16*16;
            if (m_ChunkCoords.find({targetx, targetz}) == m_ChunkCoords.end())
            {
                std::pair <int, int> pp = {targetx, targetz};
                InitChunk(pp);
            }
        }
    }
    //delete chunks
    std::vector <Chunk*> trash;
    for (Chunk* each: m_Chunks)
    {
        std::pair <int, int> pos = each -> GetPosition();
        if (OutOfBound(pos)) trash.push_back(each);
    }
    while (!trash.empty())
    {
        Chunk* temp = trash.back();
        trash.pop_back();
        DeleteChunk(temp);
    }
}

bool SceneRenderer::OutOfBound(std::pair <int, int> position)
{
    glm::vec3 campos = camera -> GetPosition();
    return !(position.first >= campos.x - m_Radius && position.first < campos.x + m_Radius && position.second >= campos.z - m_Radius && position.second < campos.z + m_Radius);
}

void SceneRenderer::DrawChunks()
{
    for (Chunk* each : m_Chunks) each -> Draw(camera);
}

void SceneRenderer::Render()
{
    UpdateChunks();
    DrawChunks();
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
//    Renderer renderer;
//
//    Block grassBlock (glm::vec3(0, 0, 0), BlockType::GRASS);
//    grassBlock.Draw(renderer, camera);
}


void SceneRenderer::SetCamera(Camera* camera)
{
    this -> camera = camera;
}
