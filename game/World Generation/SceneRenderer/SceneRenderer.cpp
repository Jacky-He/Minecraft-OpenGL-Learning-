#include "SceneRenderer.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "Timer.hpp"
#include "Input.hpp"
#include "Map.hpp"

SceneRenderer::SceneRenderer(int radius):m_Radius(radius), m_Chunks(), m_DeleteQueue()
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
    m_Chunks.insert(std::pair <std::pair <int, int>, Chunk*> (position, c));
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

void SceneRenderer::UpdateConstellations()
{
    m_LightSources.push_back(m_Constellations -> Update(camera -> GetDirection()));
}

void SceneRenderer::UpdateChunks()
{
    std::pair <int, int> campos = Chunk::GetChunkPositionAt(camera -> GetPosition());
    
    //delete chunks
    std::vector <Chunk*> trash;
    for (auto const& each : m_Chunks)
    {
        if (OutOfBound(each.first)) trash.push_back(each.second);
    }
    
    while (!trash.empty())
    {
        Chunk* temp = trash.back();
        trash.pop_back();
        std::pair <int, int> temppos = temp -> GetPosition();
        m_Chunks.erase(temppos);
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
            if (m_Chunks.find({targetx, targetz}) == m_Chunks.end())
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
    Chunk::SetLightSources(m_LightSources);
    for (auto const& each : m_Chunks)
    {
        //check if in view, better solution uses bounding volume hierarchy (checking intersection kinda expensive) (maybe later)
        if (InView(each.second)) each.second -> Draw(camera);
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
    m_LightSources.clear();
    UpdateConstellations();
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

void SceneRenderer::SetConstellations (Constellations* constellations)
{
    m_Constellations = constellations;
    Constellations::SetUp();
    m_Constellations -> Start();
}

void SceneRenderer::UpdateBlockAtLocation(glm::vec3 pos, BlockType type)
{
    std::pair <int, int> chunkpos = Chunk::GetChunkPositionAt(pos);
    if (m_Chunks.find(chunkpos) == m_Chunks.end()) return; //don't really need to update it since it's not loaded
    Chunk* c = m_Chunks[chunkpos];
    c -> UpdateBlockType(pos, type);
    for (int i = 0; i < 6; i++)
    {
        glm::vec3 check = Util::s_DirectionsUnitVectors [i] + pos;
        std::pair <int, int> checkcpos = Chunk::GetChunkPositionAt(check);
        if (m_Chunks.find(checkcpos) == m_Chunks.end()) continue; //again, not loaded
        Chunk* checkc = m_Chunks[checkcpos];
        checkc -> RecalculateMeshes(check);
    }
}
