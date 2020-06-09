#include "Textures.hpp"


std::map <std::string, Texture*> Textures::m_Textures = std::map <std::string, Texture*>();
std::mutex Textures::s_Mutex;
std::string Textures::m_BlockTypeToString [] =
{
    "grass"
};

void Textures::LoadTextures()
{
    m_Textures["grass_top"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Grass/grass_top.png");
    m_Textures["grass_side"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Grass/grass_side.png");
    m_Textures["grass_bottom"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Grass/grass_bottom.png");
}

Texture* Textures::GetTexture(const std::string &name)
{
    return m_Textures[name];
}

Texture* Textures::GetTexture(BlockType type, Direction dir)
{
    if (static_cast <int> (type) > 0) return nullptr;
    
    std::string str1 = m_BlockTypeToString [static_cast<int>(type)];
    std::string str2 = "";
    if(dir == Direction::UP) str2 = "top";
    else if (dir == Direction::DOWN) str2 = "bottom";
    else str2 = "side";
    return m_Textures[str1 + "_" + str2];
}
