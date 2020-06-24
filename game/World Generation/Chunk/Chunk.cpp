#include "Chunk.hpp"
#include "Constants.hpp"
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
    else position.x = ((int(position.x) + 1)/16 - 1)*16;
    if (position.z >= 0) position.z = int(position.z)/16*16;
    else position.z = ((int(position.z) + 1)/16 - 1)*16;
    return {position.x, position.z};
}

std::vector<std::pair<std::pair<glm::vec3, glm::vec3>, std::pair<glm::vec3, glm::vec3>>> Chunk::GetChunkFaces()
{
    return m_Faces;
}

std::vector <std::pair<glm::vec3, glm::vec3>> Chunk::GetChunkEdges()
{
    return m_Edges;
}
std::vector <glm::vec3> Chunk::GetChunkVertices ()
{
    return m_Vertices;
}

void Chunk::SetLightSources(const std::vector <Light>& lightsources)
{
    for (int i = 0; i < lightsources.size(); i++)
    {
        if (lightsources[i].type == LightSourceType::DIRECTIONAL)
        {
            s_Shader -> SetUniform3f("u_DirLight.direction", lightsources[i].dir.x, lightsources[i].dir.y, lightsources[i].dir.z);
            s_Shader -> SetUniform3f("u_DirLight.ambient", lightsources[i].ambient.x, lightsources[i].ambient.y, lightsources[i].ambient.z);
            s_Shader -> SetUniform3f("u_DirLight.diffuse", lightsources[i].diffuse.x, lightsources[i].diffuse.y, lightsources[i].diffuse.z);
            return;
        }
    }
}

void Chunk::SetUp()
{
    s_VAO = std::make_unique<VertexArray>();
    s_VBO = std::make_unique<VertexBuffer>(nullptr, sizeof(float)*216*16*16*256, GL_DYNAMIC_DRAW);
    s_IBO = std::make_unique<IndexBuffer> (nullptr, 36*16*16*256, GL_DYNAMIC_DRAW);
    s_Shader = std::make_unique<Shader>("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/shaders/quad.shader");
    s_Renderer = std::make_unique<Renderer> ();
    
    VertexBufferLayout layout;
    layout.Push<float>(3); //x, y, z
    layout.Push<float>(2); //tex coords
    layout.Push<float>(1); //tex idx
    layout.Push<float>(3); //nx, ny, nz
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
    m_Map = new Map();
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

glm::vec3 Chunk::GetPVertex(glm::vec4 plane)
{
    std::pair <int, int> p = m_BackwardLeftPosition;
    glm::vec3 res = glm::vec3 (float(p.first) - 0.5f, -0.5f, float(p.second - 0.5f));
    if (plane.x >= 0) res.x = float(p.first + 15) + 0.5f;
    if (plane.y >= 0) res.y = 255.5f;
    if (plane.z >= 0) res.z = float(p.second + 15) + 0.5f;
    return res;
}

void Chunk::Initialize()
{
    std::pair <int, int> p = m_BackwardLeftPosition;
    m_Vertices.push_back(glm::vec3 (float(p.first) - 0.5f, -0.5f, float(p.second) - 0.5f));
    m_Vertices.push_back(glm::vec3 (float(p.first + 15) + 0.5f, -0.5f, float(p.second) - 0.5f));
    m_Vertices.push_back(glm::vec3 (float(p.first + 15) + 0.5f, -0.5f, float(p.second + 15) + 0.5f));
    m_Vertices.push_back(glm::vec3 (float(p.first) - 0.5f, -0.5f, float(p.second + 15) + 0.5f));
    
    m_Vertices.push_back(glm::vec3 (float(p.first) - 0.5f, 255.5f, float(p.second) - 0.5f));
    m_Vertices.push_back(glm::vec3 (float(p.first + 15) + 0.5f, 255.5f, float(p.second) - 0.5f));
    m_Vertices.push_back(glm::vec3 (float(p.first + 15) + 0.5f, 255.5f, float(p.second + 15) + 0.5f));
    m_Vertices.push_back(glm::vec3 (float(p.first) - 0.5f, 255.5f, float(p.second + 15) + 0.5f));
    
    m_Edges.push_back({m_Vertices[0], m_Vertices[1]});
    m_Edges.push_back({m_Vertices[1], m_Vertices[2]});
    m_Edges.push_back({m_Vertices[2], m_Vertices[3]});
    m_Edges.push_back({m_Vertices[3], m_Vertices[0]});
    
    m_Edges.push_back({m_Vertices[4], m_Vertices[5]});
    m_Edges.push_back({m_Vertices[5], m_Vertices[6]});
    m_Edges.push_back({m_Vertices[6], m_Vertices[7]});
    m_Edges.push_back({m_Vertices[7], m_Vertices[4]});
    
    m_Edges.push_back({m_Vertices[0], m_Vertices[4]});
    m_Edges.push_back({m_Vertices[1], m_Vertices[5]});
    m_Edges.push_back({m_Vertices[2], m_Vertices[6]});
    m_Edges.push_back({m_Vertices[3], m_Vertices[7]});
    
    m_Faces.push_back({m_Edges[0], m_Edges[2]});
    m_Faces.push_back({m_Edges[4], m_Edges[6]});
    m_Faces.push_back({m_Edges[0], m_Edges[4]});
    m_Faces.push_back({m_Edges[1], m_Edges[5]});
    m_Faces.push_back({m_Edges[2], m_Edges[6]});
    m_Faces.push_back({m_Edges[3], m_Edges[7]});
    
    std::lock_guard<std::mutex> lock (m_BlockTypeCubeInfoMutex);
    for (int k = 0; k < 256; k++)
        for (int i = 0; i < 16; i++)
            for (int j = 0; j < 16; j++)
                 m_BlockTypes[i][j][k] = m_Map -> GetBlockTypeAtLocation(m_BackwardLeftPosition.first + i, k, m_BackwardLeftPosition.second + j);
    
    for (int i = m_BackwardLeftPosition.first; i < m_BackwardLeftPosition.first + 16; i++)
        for (int j = m_BackwardLeftPosition.second; j < m_BackwardLeftPosition.second + 16; j++)
            for (int k = 0; k < 256; k++)
                CalculateNewMesh(glm::vec3(i, k, j));
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
            type = m_Map -> GetBlockTypeAtLocation((int)std::round(temp.x), (int)std::round(temp.y), (int)std::round(temp.z));
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
        s_VBO -> SetDataMap (&m_CubeInfo[0], sizeof(float)*(int)m_CubeInfo.size());
        
        glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0));
        s_Shader -> SetUniformMat4f("u_MVP", camera->GetPVMatrix()*model);

        s_Renderer -> Draw(*s_VAO, *s_IBO, *s_Shader, 6*(int)m_CubeInfo.size()/36);
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
            int texid = (int)std::round(m_CubeInfo[curr + 35]);
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
                for (int j = 0; j < 9; j++)
                {
                    if (j == 6) loadInfo.push_back(lookup[texid]);
                    else loadInfo.push_back(m_CubeInfo[curr + i*6 + j]);
                }
            }
            curr += 36;
        }
        for (int i = 0; i < textures.size(); i++) textures[i] -> Bind(i);
        s_VBO -> SetData(&loadInfo[0], sizeof(float)*(int)loadInfo.size());
        
        glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0));
        s_Shader -> SetUniformMat4f("u_MVP", camera->GetPVMatrix()*model);

        s_Renderer -> Draw(*s_VAO, *s_IBO, *s_Shader, 6*(int)loadInfo.size()/36);
    }
}

void Chunk::UpdateBlockType(glm::vec3 pos, BlockType type)
{
    m_BlockTypeCubeInfoMutex.lock();
    int x = (int)round(pos.x) - m_BackwardLeftPosition.first;
    int y = (int)round(pos.y);
    int z = (int)round(pos.z) - m_BackwardLeftPosition.second;
    m_BlockTypes[x][z][y] = type;
    m_BlockTypeCubeInfoMutex.unlock();
    RecalculateMeshes(pos);
}

void Chunk::RecalculateMeshes (glm::vec3 pos)
{
    m_BlockTypeCubeInfoMutex.lock();
    DeleteOldMesh(pos);
    CalculateNewMesh(pos);
    m_BlockTypeCubeInfoMutex.unlock();
}

void Chunk::DeleteOldMesh (glm::vec3 pos)
{
    std::vector <float> temp;
    int x = (int)round(pos.x) - m_BackwardLeftPosition.first; int y = (int)round(pos.y); int z = (int)round(pos.z) - m_BackwardLeftPosition.second;
    if (m_CubeInfoLookup.find({{x, y}, z}) == m_CubeInfoLookup.end()) return;
    std::pair <int, int> info = m_CubeInfoLookup[{{x, y}, z}];
    for (int i = 0; i < info.first; i++) temp.push_back(m_CubeInfo[i]);
    for (int i = info.first + info.second; i < m_CubeInfo.size(); i++) temp.push_back(m_CubeInfo[i]);
    m_CubeInfo.clear();
    for (int i = 0; i < (int)temp.size(); i++) m_CubeInfo.push_back(temp[i]);
    m_CubeInfoLookup.erase({{x, y}, z});
    for (auto& each : m_CubeInfoLookup)
    {
        if (each.second.first > info.first) each.second.first -= info.second;
    }
}

void Chunk::CalculateNewMesh (glm::vec3 pos)
{
    int i = (int)round(pos.x);
    int k = (int)round(pos.y);
    int j = (int)round(pos.z);
    BlockType type = m_BlockTypes[i - m_BackwardLeftPosition.first][j - m_BackwardLeftPosition.second][k];
    if (type == BlockType::EMPTY) return;
    
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
        
        int startidx = (int)m_CubeInfo.size();
        int length = 72;
        m_CubeInfoLookup [{{i - m_BackwardLeftPosition.first, k}, j - m_BackwardLeftPosition.second}] = {startidx, length};
        for (int offset1 = 0; offset1 < 2*36; offset1 += 36)
        {
            for (int l = 0; l < 4; l++)
            {
                int offset2 = 9*l;
                for (int m = 0; m < 9; m++)
                {
                    if (m >= 6) m_CubeInfo.push_back(Constants::twoTexModel[offset1 + offset2 + m]);
                    else if (m == 5) m_CubeInfo.push_back(textureidx);
                    else if (m == 3 || m == 4) m_CubeInfo.push_back(Constants::twoTexModel[offset1 + offset2 + m]);
                    else if (m == 2) m_CubeInfo.push_back(Constants::twoTexModel[offset1 + offset2 + m] + j);
                    else if (m == 1) m_CubeInfo.push_back(Constants::twoTexModel[offset1 + offset2 + m] + k);
                    else m_CubeInfo.push_back(Constants::twoTexModel[offset1 + offset2 + m] + i);
                }
            }
        }
        return;
    }
    
    //deal with cube
    std::vector <int> v = GetExposedDirectionsOfCube(glm::vec3(i, k, j));
    int startidx = (int)m_CubeInfo.size();
    int length = int(v.size())*36;
    m_CubeInfoLookup [{{i - m_BackwardLeftPosition.first, k}, j - m_BackwardLeftPosition.second}] = {startidx, length};
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
        
        int offset1 = v[x]*36;
        for (int l = 0; l < 4; l++)
        {
            int offset2 = 9*l;
            for (int m = 0; m < 9; m++)
            {
                if (m >= 6) m_CubeInfo.push_back(Constants::cubeModel[offset1 + offset2 + m]);
                else if (m == 5) m_CubeInfo.push_back(textureidx);
                else if (m == 3 || m == 4) m_CubeInfo.push_back(Constants::cubeModel[offset1 + offset2 + m]);
                else if (m == 2) m_CubeInfo.push_back(Constants::cubeModel[offset1 + offset2 + m] + j);
                else if (m == 1) m_CubeInfo.push_back(Constants::cubeModel[offset1 + offset2 + m] + k);
                else m_CubeInfo.push_back(Constants::cubeModel[offset1 + offset2 + m] + i);
            }
        }
    }
}
