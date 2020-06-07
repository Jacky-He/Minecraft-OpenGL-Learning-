#pragma once
#include "Util.hpp"
#include "Texture.hpp"

class Textures
{
public:
    static void LoadTextures();
    static Texture* GetTexture(const std::string& name);

    static std::map <std::string, Texture*> m_Textures;
};
