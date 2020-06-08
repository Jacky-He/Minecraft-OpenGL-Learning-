#pragma once
#include "Util.hpp"
#include "Texture.hpp"

class Textures
{
public:
    static void LoadTextures();
    static Texture* GetTexture(const std::string& name);
    static Texture* GetTexture(BlockType type, Direction dir);
    static std::string m_BlockTypeToString [1];

    static std::map <std::string, Texture*> m_Textures;
};
