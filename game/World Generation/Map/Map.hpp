#pragma once
#include "Util.hpp"
#include "Noise.hpp"

class Map
{
    
private:
//    Noise m_NoiseFunction;

public:
    static int s_CurrSeed;
    static void SetCurrSeed (int seed);
    static Noise* s_NoiseFunction;
//    static Map* CurrMap;
//    static void SetCurrMap (Map* map);
//    static Map* GetCurrMap ();
    static std::mutex s_Mutex;
    static std::map <std::pair <std::pair <int, int>, int>, BlockType> s_Lookup;
    
    Map();
    ~Map();
    BlockType GetBlockTypeAtLocation(int x, int y, int z);
    double GetHeightAtLocation(int x, int y, int z);
    int InterpolateHeightAtLocation (int x, int y, int z);
    void SetBlockTypeAtLocation (int x, int y, int z, BlockType type);
    std::pair <int, int> GetChunkPositionAt(glm::vec3 position);
    Biome GetBiome (int x, int y, int z);

    BlockType GENWater(int x, int y, int z, int height);
    BlockType GENPlain(int x, int y, int z, int height);
    BlockType GENForest(int x, int y, int z, int height);
};

/*
https://mrl.nyu.edu/~perlin/noise/
*/
