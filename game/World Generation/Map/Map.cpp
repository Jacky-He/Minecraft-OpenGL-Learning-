#include "Map.hpp"

//Map* Map::CurrMap = nullptr;
std::mutex Map::s_Mutex;
std::map <std::pair <std::pair <int, int>, int>, BlockType> Map::s_Lookup = std::map <std::pair <std::pair <int, int>, int>, BlockType>();
int Map::s_CurrSeed = 1;
Noise* Map::s_NoiseFunction = nullptr;

Map::Map():m_HeightMapLimits(2000)
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

double Map::GetHeightAtLocation (int x, int y, int z)
{
    if (m_HeightMap.find({x, z}) != m_HeightMap.end()) return m_HeightMap[{x, z}];
    Biome biome = GetBiome(x, y, z);
    double res = 0;
    switch (biome)
    {
        case Biome::PLAIN:
        {
            int baseheight = 63;
            double xt = x*0.005;
            double zt = z*0.005;
            double noise = 5*(s_NoiseFunction -> GenNoise(14*xt, 0, 14*zt)) + 4*(s_NoiseFunction -> GenNoise(xt*5, 0, zt*5));
            res = noise + baseheight;
            break;
        }
        case Biome::FOREST:
        {
            int baseheight = 63;
            double xt = x*0.01;
            double zt = z*0.01;
            double noise = 20*(s_NoiseFunction -> GenNoise(3*xt, 0, 3*zt)) + 45*(s_NoiseFunction -> GenNoise(2*xt, 0, 2*zt));
            noise = noise/65;
            noise = pow(noise, 3);
            res = noise*70 + baseheight;
            break;
        }
        default:
            res = 0;
            break;
    }
    if (m_HeightMapOrder.size() >= m_HeightMapLimits)
    {
        m_HeightMap.erase(m_HeightMapOrder.front());
        m_HeightMapOrder.pop_front();
    }
    m_HeightMap [{x, z}] = res;
    m_HeightMapOrder.push_back({x, z});
    return res;
}

std::pair <int, int> Map::GetChunkPositionAt (glm::vec3 position)
{
    if (position.x >= 0) position.x = int(position.x)/16*16;
    else position.x = ((int(position.x) + 1)/16 - 1)*16;
    if (position.z >= 0) position.z = int(position.z)/16*16;
    else position.z = ((int(position.z) + 1)/16 - 1)*16;
    return {position.x, position.z};
}

int Map::InterpolateHeightAtLocation (int x, int y, int z)
{
    std::pair <int, int> p1 = GetChunkPositionAt(glm::vec3 (x, y, z));
    std::pair <int, int> p2 = {p1.first + 15, p1.second};
    std::pair <int, int> p3 = {p1.first + 15, p1.second + 15};
    std::pair <int, int> p4 = {p1.first, p1.second + 15};
    double v1 = GetHeightAtLocation(p1.first, 0, p1.second);
    double v2 = GetHeightAtLocation(p2.first, 0, p2.second);
    double v3 = GetHeightAtLocation(p3.first, 0, p3.second);
    double v4 = GetHeightAtLocation(p4.first, 0, p4.second);
    double a1 = v1*(p2.first - x)/15.0 + v2*(x - p1.first)/15.0;
    double a2 = v4*(p3.first - x)/15.0 + v3*(x - p4.first)/15.0;
    double res = a1*(p3.second - z)/15.0 + a2*(z - p2.second)/15.0;
    return res;
}

BlockType Map::GetBlockTypeAtLocation (int x, int y, int z)
{
    if (s_Lookup.find({{x, y}, z}) != s_Lookup.end()) return s_Lookup[{{x, y}, z}];
    int height = (int)round(InterpolateHeightAtLocation(x, y, z));
    Biome biome = GetBiome(x, y, z);
    switch (biome)
    {
        case Biome::WATER:
            return GENWater(x, y, z, height);
        case Biome::PLAIN:
            return GENPlain(x, y, z, height);
        case Biome::FOREST:
            return GENForest(x, y, z, height);
        default:
            break;
    }
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
}

BlockType Map::GENWater(int x, int y, int z, int height)
{
    return BlockType::EMPTY;
}

BlockType Map::GENPlain(int x, int y, int z, int height)
{
    double vegyval = s_NoiseFunction -> GenNoise(202.2423*x, 0, 384.93*z);
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

BlockType Map::GENForest(int x, int y, int z, int height)
{
    double vegyval = s_NoiseFunction -> GenNoise(202.2423*x, 0, 384.93*z);
    double treeextraheight = 4*(s_NoiseFunction -> GenNoise(222.38*x, 0, 84.72*z));
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
