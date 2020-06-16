#include "SceneRenderer.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "Timer.hpp"
#include "Input.hpp"
#include "Map.hpp"

SceneRenderer::SceneRenderer(int radius):m_Radius(radius), m_Chunks(), m_ChunkCoords()
{
    camera = nullptr;
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

SceneRenderer::~SceneRenderer()
{
    
}

void SceneRenderer::InitChunk(std::pair <int, int> position)
{
    Chunk* c = new Chunk(position);
    m_Chunks.insert(c);
    m_ChunkCoords.insert(position);
    c -> Init();
}

void SceneRenderer::DeleteChunk(Chunk* chunk)
{
    std::pair <int, int> temppos = chunk -> GetPosition();
    m_ChunkCoords.erase(temppos);
    m_Chunks.erase(chunk);
    delete chunk;
}

void SceneRenderer::UpdateChunks()
{
    
    std::pair <int, int> campos = Chunk::GetChunkPositionAt(camera -> GetPosition());
    
    //add chunks
    std::vector<std::pair <int, int>> v;
    for (int x = -m_Radius; x < m_Radius; x += 16)
    {
        for (int z = -m_Radius; z < m_Radius; z += 16)
        {
            int targetx = campos.first + x;
            int targetz = campos.second + z;
            targetx = targetx/16*16;
            targetz = targetz/16*16;
            if (m_ChunkCoords.find({targetx, targetz}) == m_ChunkCoords.end())
            {
                std::pair <int, int> pp = {targetx, targetz};
                v.push_back(pp);
            }
        }
    }
    
    for (int i = 0; i < (int)v.size(); i++) std::swap(v[i], v[Util::RandInt(i, (int)v.size() - 1)]);
    for (int i = 0; i < (int)v.size(); i++) InitChunk(v[i]);
    
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
    std::pair <int, int> campos = Chunk::GetChunkPositionAt(camera -> GetPosition());
    return !(position.first >= int(campos.first - m_Radius)/16*16 && position.first < int(campos.first + m_Radius)/16*16 && position.second >= int(campos.second - m_Radius)/16*16 && position.second < int(campos.second + m_Radius)/16*16);
}

void SceneRenderer::DrawChunks()
{
    for (Chunk* each : m_Chunks)
    {
        //check if in view, better solution uses bounding volume hierarchy
        each -> Draw(camera);
    }
}

void SceneRenderer::Render()
{
    UpdateChunks();
    DrawChunks();
    Input::DrawCrossHair();
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
