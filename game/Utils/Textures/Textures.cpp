#include "Textures.hpp"


std::map <std::string, Texture*> Textures::m_Textures = std::map <std::string, Texture*>();
std::mutex Textures::s_Mutex;
std::string Textures::m_BlockTypeToString [200];

void Textures::LoadTextures()
{
    m_Textures["grass_top"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Blocks/Grass/grass_top.png");
    m_Textures["grass_side"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Blocks/Grass/grass_side.png");
    m_Textures["grass_bottom"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Blocks/Grass/grass_bottom.png");
    m_Textures["crosshair"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/CrossHair/crosshair.png");
    m_Textures["tall grass"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Vegetations/TallGrass/tallgrass.png");
    m_Textures["dirt_top"] = m_Textures ["dirt_bottom"] = m_Textures ["dirt_side"] = m_Textures["grass_bottom"];
    m_Textures["azure bluet"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Vegetations/AzureBluet/azurebluet.png");
    m_Textures["cornflower"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Vegetations/Cornflower/cornflower.png");
    m_Textures["oxeye daisy"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Vegetations/OxeyeDaisy/oxeyedaisy.png");
    m_Textures["lily of the valley"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Vegetations/Lily of Valley/lilyofvalley.png");
    m_Textures["oak leaf_top"] = m_Textures["oak leaf_side"] = m_Textures["oak leaf_bottom"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Vegetations/Oak Tree/oak_leaf.png");
    m_Textures["oak log_top"] = m_Textures["oak log_bottom"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Vegetations/Oak Tree/oak_log_top.png");
    m_Textures["oak log_side"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Vegetations/Oak Tree/oak_log_side.png");
    m_Textures["birch leaf_top"] = m_Textures["birch leaf_side"] = m_Textures["birch leaf_bottom"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Vegetations/Birch Tree/birch_leaf.png");
    m_Textures["birch log_top"] = m_Textures["birch log_bottom"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Vegetations/Birch Tree/birch_log_top.png");
    m_Textures["birch log_side"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Vegetations/Birch Tree/birch_log_side.png");
    m_Textures["sand_top"] = m_Textures["sand_side"] = m_Textures["sand_bottom"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Blocks/Sand/sand.png");
    m_Textures["sun"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Constellations/Sun/sun.png");
    m_Textures["moon"] = new Texture("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/textures/Constellations/Moon/moon.png");
    m_BlockTypeToString [0] = "grass";
    m_BlockTypeToString [1] = "dirt";
    m_BlockTypeToString [2] = "oak leaf";
    m_BlockTypeToString [3] = "oak log";
    m_BlockTypeToString [4] = "sand";
    m_BlockTypeToString [5] = "birch leaf";
    m_BlockTypeToString [6] = "birch log";
    m_BlockTypeToString [100] = "tall grass";
    m_BlockTypeToString [101] = "azure bluet";
    m_BlockTypeToString [102] = "cornflower";
    m_BlockTypeToString [103] = "oxeye daisy";
    m_BlockTypeToString [104] = "lily of the valley";
}

Texture* Textures::GetTexture(const std::string &name)
{
    return m_Textures[name];
}

Texture* Textures::GetTexture(BlockType type, Direction dir)
{
    int idx = static_cast<int> (type);
    if (idx < 0) return nullptr;
    //cubes
    if (idx < 100)
    {
        std::string str1 = m_BlockTypeToString [idx];
        std::string str2 = "";
        if(dir == Direction::UP) str2 = "top";
        else if (dir == Direction::DOWN) str2 = "bottom";
        else str2 = "side";
        return m_Textures[str1 + "_" + str2];
    }
    //two texes
    return m_Textures[m_BlockTypeToString [idx]];
}
