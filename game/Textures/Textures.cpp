#include "Textures.hpp"


std::map <std::string, Texture*> Textures::m_Textures = std::map <std::string, Texture*>();

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
