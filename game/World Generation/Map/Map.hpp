#pragma once
#include "Util.hpp"
#include "Noise.hpp"

class Map
{
    
private:
    Noise m_NoiseFunction;

public:
    static Map* CurrMap;
    static void SetCurrMap (Map* map);
    static Map* GetCurrMap ();
    static std::mutex s_Mutex;

    
    Map();
    ~Map();
    std::map <std::pair <std::pair <int, int>, int>, BlockType> m_Lookup;
    void GenerateTerrain ();
    BlockType GetBlockTypeAtLocation(int x, int y, int z);
    void SetBlockTypeAtLocation (int x, int y, int z, BlockType type);
    Biome GetBiome (int x, int y, int z);

    BlockType GENWater(int x, int y, int z);
    BlockType GENPlain(int x, int y, int z);
    BlockType GENForest(int x, int y, int z);
};

/*
https://mrl.nyu.edu/~perlin/noise/
*/
