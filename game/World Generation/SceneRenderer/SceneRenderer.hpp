#pragma once
#include "Camera.hpp"
#include "Chunk.hpp"


class SceneRenderer
{
private:
    Camera* camera;
    
    int m_Radius;
    
    std::map <std::pair <int, int>, Chunk*> m_Chunks;
    
    std::queue <Chunk*> m_DeleteQueue;
    std::mutex m_DeleteQueueMutex;
    
    void UpdateChunks();
    void DrawChunks();
    void DeleteChunks();
    void InitChunk(std::pair <int, int> position);
    bool OutOfBound(std::pair<int, int> position);
    bool InView (Chunk* c);
    bool SameSideAsPlaneNormal (glm::vec4 plane, glm::vec4 point);
    
    std::mutex m_Mutex;
    std::vector<std::future<void>> m_Futures;

public:
    SceneRenderer(int radius);
    ~SceneRenderer();
    void Render();
    void SetCamera(Camera* camera);
    void UpdateBlockAtLocation (glm::vec3 pos, BlockType type);
};
