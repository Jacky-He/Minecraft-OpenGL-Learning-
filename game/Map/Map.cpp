#include "Map.hpp"

Map* Map::CurrMap = nullptr;

Map::Map():m_NoiseFunction(), m_Lookup()
{
    
}

Map::~Map()
{
    
}

void Map::GenerateTerrain()
{
    
}

void Map::SetCurrMap(Map *map)
{
    CurrMap = map;
}

Map* Map::GetCurrMap()
{
    return CurrMap;
}
BlockType Map::GetBlockTypeAtLocation (int x, int y, int z)
{
    if (m_Lookup.find({{x, y}, z}) != m_Lookup.end()) return m_Lookup[{{x, y}, z}];
    BlockType res = BlockType::EMPTY;
    double xt = x*0.01;
    double zt = z*0.01;
    double val = m_NoiseFunction.GenNoise(xt, 0, zt);
    double height = val*256;
    if (y <= height) res = BlockType::GRASS;
    return res;
}

void Map::SetBlockTypeAtLocation (int x, int y, int z, BlockType type)
{
    m_Lookup [{{x, y}, z}] = type;
}
