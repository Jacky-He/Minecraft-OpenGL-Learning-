#include "Map.hpp"

//Map* Map::CurrMap = nullptr;
std::mutex Map::s_Mutex;
std::map <std::pair <std::pair <int, int>, int>, BlockType> Map::s_Lookup = std::map <std::pair <std::pair <int, int>, int>, BlockType>();
int Map::s_CurrSeed = 1;
Noise* Map::s_NoiseFunction = nullptr;

Map::Map()
{
    if (s_NoiseFunction == nullptr) s_NoiseFunction = new Noise(s_CurrSeed);
}

Map::~Map()
{
    
}

void Map::SetCurrSeed(int seed)
{
    s_CurrSeed = seed;
}
//void Map::SetCurrMap(Map *map)
//{
//    CurrMap = map;
//}
//
//Map* Map::GetCurrMap()
//{
//    return CurrMap;
//}

BlockType Map::GetBlockTypeAtLocation (int x, int y, int z)
{
    
    if (s_Lookup.find({{x, y}, z}) != s_Lookup.end()) return s_Lookup[{{x, y}, z}];
    Biome biome = GetBiome(x, y, z);
    switch (biome)
    {
        case Biome::WATER:
            return GENWater(x, y, z);
        case Biome::PLAIN:
            return GENPlain(x, y, z);
        case Biome::FOREST:
            return GENForest(x, y, z);
        default:
            break;
    }
    
    BlockType res = BlockType::EMPTY;
    double xt = x*0.01;
    double zt = z*0.01;
    double val = s_NoiseFunction -> GenNoise(xt, 0, zt);
    double height = val*256;
    if (y <= height) res = BlockType::GRASS;
    return res;
}

void Map::SetBlockTypeAtLocation (int x, int y, int z, BlockType type)
{
    s_Lookup [{{x, y}, z}] = type;
}

Biome Map::GetBiome(int x, int y, int z)
{
    double xt = x*0.005;
    double zt = z*0.005;
    double val = s_NoiseFunction -> GenNoise(xt, 0, zt);
    if (val < 0.3) return Biome::PLAIN;
    else return Biome::FOREST;
//    if (val < 0.2) return Biome::WATER;
//    else if (val < 0.3) return Biome::PLAIN;
//    else return Biome::FOREST;
}

BlockType Map::GENWater(int x, int y, int z)
{
    return BlockType::EMPTY;
}

BlockType Map::GENPlain(int x, int y, int z)
{
    int baseheight = 63;
    double xt = x*0.005;
    double zt = z*0.005;
    double noise = 2*(s_NoiseFunction -> GenNoise(14*xt, 0, 14*zt)) + 3*(s_NoiseFunction -> GenNoise(xt, 0, zt)) + 4*(s_NoiseFunction -> GenNoise(xt*5, 0, zt*5));
    int height = int(noise) + baseheight;
    double vegyval = s_NoiseFunction -> GenNoise(202.2423*xt, 0, 384.93*zt);
    if (y == height + 1)
    {
        if (vegyval < 0.20) return BlockType::AZUREBLUET;
        if (vegyval < 0.29) return BlockType::TALLGRASS;
        if (vegyval < 0.30) return BlockType::CORNFLOWER;
        if (vegyval < 0.31) return BlockType::OXEYEDAISY;
    }
    if (y <= height) return BlockType::GRASS;
    return BlockType::EMPTY;
}

BlockType Map::GENForest(int x, int y, int z)
{
    int baseheight = 63;
    double xt = x*0.01;
    double zt = z*0.01;
    double noise = 15*(s_NoiseFunction -> GenNoise(3*xt, 0, 3*zt)) + 30*(s_NoiseFunction -> GenNoise(xt, 0, zt)) + 5*(s_NoiseFunction -> GenNoise(xt*5, 0, zt*5)) + 15*(s_NoiseFunction -> GenNoise(2*xt, 0, 2*zt));
    noise = noise/65;
    noise = pow(noise, 3);
    int height = int(noise*70) + baseheight;
    double vegyval = s_NoiseFunction -> GenNoise(202.2423*xt, 0, 384.93*zt);
    double treeextraheight = 4*(s_NoiseFunction -> GenNoise(222.38*xt, 0, 84.72*zt));
    if (vegyval < 0.20)
    {
        int radius = 2;
        if (y >= height + 1 && y >= height + 1 && y <= height + 4 + int(treeextraheight)) return BlockType::OAKLOG;
        s_Mutex.lock();
        for (int dx = -radius; dx <= radius; dx++)
        {
            for (int dz = -radius; dz <= radius; dz++)
            {
                for (int dy = 0; dy <= radius; dy++)
                {
                    if (abs(dx) + abs(dz) + abs(dy) <= radius)
                    {
                        SetBlockTypeAtLocation(x + dx, height + 3 + int(treeextraheight) + dy, z + dz, BlockType::OAKLEAF);
                    }
                }
            }
        }
        s_Mutex.unlock();
    }
    if (y <= height) return BlockType::GRASS;
    return BlockType::EMPTY;
}
