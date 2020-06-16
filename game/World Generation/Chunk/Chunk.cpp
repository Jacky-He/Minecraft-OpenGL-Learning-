#include "Chunk.hpp"
#include "Constants.hpp"
#include "Map.hpp"
#include "Timer.hpp"

std::unique_ptr<VertexArray> Chunk::s_VAO = nullptr;
std::unique_ptr<VertexBuffer> Chunk::s_VBO = nullptr;
std::unique_ptr<IndexBuffer> Chunk::s_IBO = nullptr;
std::unique_ptr<Shader> Chunk::s_Shader = nullptr;
std::unique_ptr<Renderer> Chunk::s_Renderer = nullptr;
int Chunk::s_MaxTextureUnits = 0;


std::pair <int, int> Chunk::GetChunkPositionAt (glm::vec3 position)
{
    if (position.x >= 0) position.x = int(position.x)/16*16;
    else position.x = (int(position.x)/17 - 1)*16;
    if (position.z >= 0) position.z = int(position.z)/16*16;
    else position.z = (int(position.z)/17 - 1)*16;
    return {position.x, position.z};
}

void Chunk::SetUp()
{
    s_VAO = std::make_unique<VertexArray>();
    s_VBO = std::make_unique<VertexBuffer>(nullptr, sizeof(float)*144*16*16*256, GL_DYNAMIC_DRAW);
    s_IBO = std::make_unique<IndexBuffer> (nullptr, 36*16*16*256, GL_DYNAMIC_DRAW);
    s_Shader = std::make_unique<Shader>("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/shaders/cube.shader");
    s_Renderer = std::make_unique<Renderer> ();
    
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(1);
    s_VAO -> AddBuffer(*s_VBO, layout);

    GLCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &s_MaxTextureUnits));
    int samplers [32];
    for (int i = 0; i < 32; i++) samplers[i] = i;
    s_Shader -> SetUniformiv("u_Textures", samplers, 32);
    
    //setup indices array
    unsigned int* indices = new unsigned int [36*16*16*256];
    unsigned int cnt = 0;
    for (unsigned int i = 0; i < 36*16*16*256; i++)
    {
        unsigned int a = i%6;
        if (a >= 0 && a <= 2) indices[i] = cnt + a;
        else if (a == 3 || a == 4) indices[i] = cnt + a - 1;
        else if (a == 5)
        {
            indices[i] = cnt;
            cnt += 4;
        }
    }
    s_IBO -> SetData(indices, 36*16*16*256);
}

Chunk::Chunk(std::pair <int, int> position):m_BackwardLeftPosition(position)
{
}

Chunk::~Chunk()
{
    
}

std::pair <int, int> Chunk::GetPosition()
{
    return m_BackwardLeftPosition;
}

void Chunk::Init()
{
    m_Futures.push_back(std::async(std::launch::async, &Chunk::Initialize, this));
}

void Chunk::Initialize()
{
    std::lock_guard<std::mutex> lock1 (Map::s_Mutex);
    std::lock_guard<std::mutex> lock2 (Textures::s_Mutex);

    for (int k = 0; k < 256; k++)
    {
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 16; j++)
            {
                 m_BlockTypes[i][j][k] = Map::CurrMap -> GetBlockTypeAtLocation(m_BackwardLeftPosition.first + i, k, m_BackwardLeftPosition.second + j);
            }
        }
    }
    
    for (int i = m_BackwardLeftPosition.first; i < m_BackwardLeftPosition.first + 16; i++)
    {
        for (int j = m_BackwardLeftPosition.second; j < m_BackwardLeftPosition.second + 16; j++)
        {
            for (int k = 0; k < 256; k++)
            {
                BlockType type = m_BlockTypes[i - m_BackwardLeftPosition.first][j - m_BackwardLeftPosition.second][k];
                if (type == BlockType::EMPTY) continue;
                
                //deal with Two Tex
                if (Util::isTwoTex(type))
                {
                    Texture* texture = Textures::GetTexture (type, Direction::CENTER);

                    int textureidx = -1;
                    if (m_NeededTextures.find (texture) != m_NeededTextures.end()) textureidx = m_NeededTextures[texture];
                    else
                    {
                        m_NeededTextures[texture] = (int)m_TextureVector.size();
                        m_TextureVector.push_back(texture);
                        textureidx = (int)m_TextureVector.size() - 1;
                    }
                    for (int offset1 = 0; offset1 < 2*24; offset1 += 24)
                    {
                        for (int l = 0; l < 4; l++)
                        {
                            int offset2 = 6*l;
                            for (int m = 0; m < 6; m++)
                            {
                                if (m == 5) m_CubeInfo.push_back(textureidx);
                                else if (m == 3 || m == 4) m_CubeInfo.push_back(Constants::cubeModel[offset1 + offset2 + m]);
                                else if (m == 2) m_CubeInfo.push_back(Constants::twoTexModel[offset1 + offset2 + m] + j);
                                else if (m == 1) m_CubeInfo.push_back(Constants::twoTexModel[offset1 + offset2 + m] + k);
                                else m_CubeInfo.push_back(Constants::twoTexModel[offset1 + offset2 + m] + i);
                            }
                        }
                    }
                    continue;
                }
                
                //deal with cube
                std::vector <int> v = GetExposedDirectionsOfCube(glm::vec3(i, k, j));
                for (unsigned int x = 0; x < v.size(); x++)
                {
                    Texture* texture = Textures::GetTexture(type, static_cast<Direction>(v[x]));
                    
                    int textureidx = -1;
                    if (m_NeededTextures.find(texture) != m_NeededTextures.end())
                    {
                        textureidx = m_NeededTextures [texture];
                    }
                    else
                    {
                        m_NeededTextures[texture] = (int)m_TextureVector.size();
                        m_TextureVector.push_back(texture);
                        textureidx = (int)m_TextureVector.size() - 1;
                    }
                    
                    int offset1 = v[x]*24;
                    for (int l = 0; l < 4; l++)
                    {
                        int offset2 = 6*l;
                        for (int m = 0; m < 6; m++)
                        {
                            if (m == 5) m_CubeInfo.push_back(textureidx);
                            else if (m == 3 || m == 4) m_CubeInfo.push_back(Constants::cubeModel[offset1 + offset2 + m]);
                            else if (m == 2) m_CubeInfo.push_back(Constants::cubeModel[offset1 + offset2 + m] + j);
                            else if (m == 1) m_CubeInfo.push_back(Constants::cubeModel[offset1 + offset2 + m] + k);
                            else m_CubeInfo.push_back(Constants::cubeModel[offset1 + offset2 + m] + i);
                        }
                    }
                }
            }
        }
    }
}

bool Chunk::OutOfBound (glm::vec3 position)
{
    return !(position.y >= 0 && position.y <= 255 && position.x >= m_BackwardLeftPosition.first && position.x <= m_BackwardLeftPosition.first + 15 && position.z >= m_BackwardLeftPosition.second && position.z <= m_BackwardLeftPosition.second + 15);
}

std::vector <int> Chunk::GetExposedDirectionsOfCube(glm::vec3 position)
{
    std::vector <int> res;
    for (int i = 0; i < 6; i++)
    {
        glm::vec3 temp = position + Util::s_DirectionsUnitVectors[i];
        BlockType type;
        if (OutOfBound(temp))
        {
            type = Map::CurrMap -> GetBlockTypeAtLocation((int)std::round(temp.x), (int)std::round(temp.y), (int)std::round(temp.z));
        }
        else type = m_BlockTypes [(int)std::round(temp.x) - m_BackwardLeftPosition.first][(int)std::round(temp.z) - m_BackwardLeftPosition.second][(int)std::round(temp.y)];
        if (Util::isEmptyWhenRendering(type)) res.push_back(i);
    }
    return res;
}

void Chunk::Draw(Camera* camera)
{
    if (m_NeededTextures.size() <= s_MaxTextureUnits)
    {
        for (int i = 0; i < m_TextureVector.size(); i++) m_TextureVector[i] -> Bind(i);
        s_VBO -> SetData(&m_CubeInfo[0], sizeof(float)*(int)m_CubeInfo.size());
        
        glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0));
        s_Shader -> SetUniformMat4f("u_MVP", camera->GetPVMatrix()*model);

        s_Renderer -> Draw(*s_VAO, *s_IBO, *s_Shader, 6*(int)m_CubeInfo.size()/24);
        return;
    }
    
    int curr = 0;
    int lookup [100];
       
    while (curr < (int)m_CubeInfo.size())
    {
        std::vector <float> loadInfo;
        std::set <int> selectedTextures;
        std::vector <Texture*> textures;
        
        int texturecnt = 0;
        while (texturecnt < s_MaxTextureUnits && curr < (int)m_CubeInfo.size())
        {
            int texid = (int)std::round(m_CubeInfo[curr + 23]);
            if (selectedTextures.find(texid) == selectedTextures.end())
            {
                texturecnt++;
                selectedTextures.insert(texid);
                lookup [texid] = (int)selectedTextures.size() - 1;
                Texture* t = m_TextureVector[texid];
                textures.push_back(t);
            }
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 6; j++)
                {
                    if (j == 6) loadInfo.push_back(lookup[texid]);
                    else loadInfo.push_back(m_CubeInfo[curr + i*6 + j]);
                }
            }
            curr += 24;
        }
        for (int i = 0; i < textures.size(); i++) textures[i] -> Bind(i);
        s_VBO -> SetData(&loadInfo[0], sizeof(float)*(int)loadInfo.size());
        
        glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0));
        s_Shader -> SetUniformMat4f("u_MVP", camera->GetPVMatrix()*model);

        s_Renderer -> Draw(*s_VAO, *s_IBO, *s_Shader, 6*(int)loadInfo.size()/24);
    }
}