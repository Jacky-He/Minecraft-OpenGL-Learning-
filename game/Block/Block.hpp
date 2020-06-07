#pragma once
#include "Util.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"

enum BlockType
{
    GRASS = 0
};

struct Vertex
{
    float x; float y; float z; int textureID;
};

class Block
{
public:
    BlockType m_BlockType;
    glm::vec3 m_Position;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer>m_VBO;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<IndexBuffer> m_IBO;
    
    Block (glm::vec3 pos, BlockType type);
    ~Block();
    void Draw(Renderer renderer, Camera* camera);
    void SetTextures();
};
