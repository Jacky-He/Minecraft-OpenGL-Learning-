#pragma once
#include "Util.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"
#include "IndexBuffer.hpp"
#include "Textures.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "Map.hpp"

class Chunk //16 x 16 x 256
{
private:
    static std::unique_ptr<VertexArray> s_VAO;
    static std::unique_ptr<VertexBuffer> s_VBO;
    static std::unique_ptr<Shader> s_Shader;
    static std::unique_ptr<IndexBuffer> s_IBO;
    static std::unique_ptr<Renderer> s_Renderer;
    static int s_MaxTextureUnits;
    
    std::pair <int, int> m_BackwardLeftPosition; //most negative value 2D
    
    BlockType m_BlockTypes [16][16][256]; //x, z, y offset;
    
    std::vector <float> m_CubeInfo;
    std::map<Texture*, int> m_NeededTextures;
    std::vector<Texture*> m_TextureVector;
    std::vector<std::future<void>> m_Futures;
    
    std::vector<std::pair<std::pair<glm::vec3, glm::vec3>, std::pair<glm::vec3, glm::vec3>>> m_Faces;
    std::vector<std::pair <glm::vec3, glm::vec3>> m_Edges;
    std::vector<glm::vec3> m_Vertices;
    
    Map* m_Map;
    
//    VertexArray m_VAO;
//    VertexBuffer m_VBO;
//    Shader m_Shader;
//    IndexBuffer m_IBO;
//    Renderer m_Renderer;

    void Initialize();
    std::vector <int> GetExposedDirectionsOfCube (glm::vec3 position);
    bool OutOfBound (glm::vec3 position);

public:
    static void SetUp();
    static std::pair <int, int> GetChunkPositionAt (glm::vec3 position);
    
    void Init();
    void Draw(Camera* camera);
    std::vector <glm::vec3> GetChunkVertices();
    std::vector<std::pair<glm::vec3, glm::vec3>> GetChunkEdges();
    std::vector<std::pair<std::pair<glm::vec3, glm::vec3>, std::pair<glm::vec3, glm::vec3>>> GetChunkFaces();
    std::pair <int, int> GetPosition();
    Chunk(std::pair <int, int> position);
    ~Chunk();
};
