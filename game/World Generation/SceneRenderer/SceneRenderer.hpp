#pragma once
#include "Camera.hpp"
#include "Chunk.hpp"
#include "Constellations.hpp"
#include "Light.hpp"

class SceneRenderer
{
private:
    Camera* camera;
    Constellations* m_Constellations;
    
    int m_Radius;
    
    std::map <std::pair <int, int>, Chunk*> m_Chunks;
    
    std::queue <Chunk*> m_DeleteQueue;
    std::mutex m_DeleteQueueMutex;
    
    void UpdateChunks();
    void DrawChunks();
    void DeleteChunks();
    void UpdateConstellations();
    void InitChunk(std::pair <int, int> position);
    bool OutOfBound(std::pair<int, int> position);
    bool InView (Chunk* c);
    bool SameSideAsPlaneNormal (glm::vec4 plane, glm::vec4 point);
    
    std::mutex m_Mutex;
    std::vector<std::future<void>> m_Futures;
    
    std::vector<Light> m_LightSources;

public:
    SceneRenderer(int radius);
    ~SceneRenderer();
    void Render();
    void SetCamera(Camera* camera);
    void SetConstellations (Constellations* constellations);
    void UpdateBlockAtLocation (glm::vec3 pos, BlockType type);
};
