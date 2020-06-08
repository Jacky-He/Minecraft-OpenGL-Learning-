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


class Chunk //16 x 16 x 256
{
private:
    static std::unique_ptr<VertexArray> s_VAO;
    static std::unique_ptr<VertexBuffer> s_VBO;
    static std::unique_ptr<Shader> s_Shader;
    static std::unique_ptr<IndexBuffer> s_IBO;
    static int s_MaxTextureUnits;
    static glm::vec3 s_DirectionsUnitVectors[6];
    
    std::pair <int, int> m_BackwardLeftPosition; //most negative value 2D
    
    BlockType m_BlockTypes [16][16][256]; //x, z, y offset;
    
    std::vector <float> m_CubeInfo;
    std::map<Texture*, int> m_NeededTextures;
    std::vector<Texture*> m_TextureVector;
    std::vector<std::future<void>> m_Futures;
    
    void Initialize();
    std::vector <int> GetExposedDirectionsOfCube (glm::vec3 position);
    bool OutOfBound (glm::vec3 position);

public:
    static void SetUp();

    void Draw(Camera* camera);
    std::pair <int, int> GetPosition();
    Chunk(std::pair <int, int> position);
    ~Chunk();
};
