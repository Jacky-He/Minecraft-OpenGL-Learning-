#include "SceneRenderer.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "Timer.hpp"
#include "Input.hpp"
#include "Map.hpp"

SceneRenderer::SceneRenderer(int radius):m_Radius(radius), m_Chunks(), m_ChunkCoords(), m_DeleteQueue()
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

void SceneRenderer::DeleteChunks()
{
    std::lock_guard<std::mutex> lock(m_DeleteQueueMutex);
    while (!m_DeleteQueue.empty())
    {
        Chunk* chunk = m_DeleteQueue.front();
        m_DeleteQueue.pop();
        delete chunk;
    }
}

void SceneRenderer::UpdateChunks()
{
    std::pair <int, int> campos = Chunk::GetChunkPositionAt(camera -> GetPosition());
    
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
        std::pair <int, int> temppos = temp -> GetPosition();
        m_ChunkCoords.erase(temppos);
        m_Chunks.erase(temp);
        m_DeleteQueue.push(temp);
    }
    
    m_Futures.push_back(std::async(std::launch::async, &SceneRenderer::DeleteChunks, this));
    
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
        //check if in view, better solution uses bounding volume hierarchy (checking intersection kinda expensive) (maybe later)
        if (InView (each)) each -> Draw(camera);
    }
}

// nvm p-vertex is actually op
bool SceneRenderer::InView(Chunk* c)
{
    glm::mat4 mat = camera -> GetPVMatrix();
    glm::vec4 r4 = glm::row(mat, 3);
    std::vector <glm::vec4> planes;
    
    //get frustum planes
    for (int row = 0; row < 3; row++)
    {
        glm::vec4 curr = glm::row(mat, row);
        planes.push_back(r4 - curr);
        planes.push_back(r4 + curr);
    }
    
    for (glm::vec4 plane : planes)
    {
        //only need p-vertex
        if (!SameSideAsPlaneNormal(plane, glm::vec4 (c -> GetPVertex(plane), 1))) return false;
    }
    return true;
}

bool SceneRenderer::SameSideAsPlaneNormal (glm::vec4 plane, glm::vec4 point)
{
    return glm::dot(plane, point) >= 0;
}

void SceneRenderer::Render()
{
    UpdateChunks();
    DrawChunks();
    Input::DrawCrossHair();
    
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
//    Renderer renderer;
}

void SceneRenderer::SetCamera(Camera* camera)
{
    this -> camera = camera;
}
