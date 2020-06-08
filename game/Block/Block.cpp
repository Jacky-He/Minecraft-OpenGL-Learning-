#include "Block.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"
#include "Textures.hpp"
#include "Constants.hpp"



Block::Block(glm::vec3 pos, BlockType type):m_BlockType(type), m_Position(pos)
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(Constants::cubeModel, sizeof(Constants::cubeModel));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(1);
    m_VAO -> AddBuffer(*m_VBO, layout);
    m_IBO = std::make_unique<IndexBuffer>(Constants::cubeIndices, 36);
    m_Shader = std::make_unique<Shader>("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/shaders/cube.shader");
    int samplers [6] = {0, 1, 2, 3, 4, 5};
    m_Shader -> SetUniformiv("u_Textures", samplers, 6);
}

Block::~Block()
{
    
}

void Block::Draw(Renderer renderer, Camera* camera)
{
    Texture* front = Textures::GetTexture("grass_side");
    Texture* back = Textures::GetTexture("grass_side");
    Texture* right = Textures::GetTexture("grass_side");
    Texture* left = Textures::GetTexture("grass_side");
    Texture* top = Textures::GetTexture("grass_top");
    Texture* bottom = Textures::GetTexture("grass_bottom");
    front->Bind(0);
    back->Bind(1);
    right->Bind(2);
    left->Bind(3);
    top->Bind(4);
    bottom->Bind(5);

    glm::mat4 model = glm::translate(glm::mat4(1.0), m_Position);
    m_Shader -> SetUniformMat4f("u_MVP", camera->GetPVMatrix()*model);
    
    renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
}
