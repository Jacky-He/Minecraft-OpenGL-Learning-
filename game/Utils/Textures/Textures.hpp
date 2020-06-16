#pragma once
#include "Util.hpp"
#include "Texture.hpp"

class Textures
{
public:
    static void LoadTextures();
    static Texture* GetTexture(const std::string& name);
    static Texture* GetTexture(BlockType type, Direction dir);
    static std::string m_BlockTypeToString [200];
    static std::mutex s_Mutex;

    static std::map <std::string, Texture*> m_Textures;
};
