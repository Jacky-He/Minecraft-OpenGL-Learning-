#pragma once
#include "Camera.hpp"
#include "Chunk.hpp"


class SceneRenderer
{
private:
    Camera* camera;
    
    int m_Radius;
    std::set <Chunk*> m_Chunks;
    std::set <std::pair <int, int>> m_ChunkCoords;
    
    void UpdateChunks();
    void DrawChunks();
    void DeleteChunk(Chunk* chunk);
    void InitChunk(std::pair <int, int> position);
    bool OutOfBound(std::pair<int, int> position);
    
    std::mutex m_Mutex;
    std::vector<std::future<void>> m_Futures;

public:
    SceneRenderer(int radius);
    ~SceneRenderer();
    void Render();
    void SetCamera(Camera* camera);
};
