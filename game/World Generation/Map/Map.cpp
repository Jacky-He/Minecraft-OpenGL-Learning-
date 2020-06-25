#include "Map.hpp"

//Map* Map::CurrMap = nullptr;
std::mutex Map::s_Mutex;
std::map <std::pair <std::pair <int, int>, int>, BlockType> Map::s_Lookup = std::map <std::pair <std::pair <int, int>, int>, BlockType>();
int Map::s_CurrSeed = 1;
Noise* Map::s_NoiseFunction = nullptr;

Map::Map():m_HeightMapLimits(2000), m_InterHeightMapLimits(300)
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

std::pair <BlockType, int> Map::GetVegetationAtLocation (int x, int y, int z)
{
    Biome biome = GetBiome(x, y, z);
    switch (biome)
    {
        case Biome::PLAIN:
        {
            double vegyval = s_NoiseFunction -> GenNoise(202.2423*x, 0, 384.93*z);
            if (vegyval < 0.20) return {BlockType::AZUREBLUET, 1};
            if (vegyval < 0.29) return {BlockType::TALLGRASS, 1};
            if (vegyval < 0.30) return {BlockType::CORNFLOWER, 1};
            if (vegyval < 0.31) return {BlockType::OXEYEDAISY, 1};
            break;
        }
        case Biome::FOREST:
        {
            double vegyval = s_NoiseFunction -> GenNoise(202.2423*x, 0, 384.93*z);
            double treeextraheight = 4*(s_NoiseFunction -> GenNoise(222.38*x, 0, 84.72*z));
            if (vegyval < 0.18) return {BlockType::OAKLOG, 3 + (int)round(treeextraheight)};
            if (vegyval < 0.21) return {BlockType::BIRCHLOG, 3 + (int)round(treeextraheight)};
            if (vegyval < 0.25) return {BlockType::LILYOFVALLEY, 1};
            break;
        }
        default:
            break;
    }
    return {BlockType::EMPTY, 0};
}

double Map::GetHeightAtLocation (int x, int y, int z)
{
    if (m_HeightMap.find({x, z}) != m_HeightMap.end()) return m_HeightMap[{x, z}];
    double res = 0;
    Biome biome = GetBiome(x, y, z);
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
    if (m_InterHeightMap.find({x, z}) != m_InterHeightMap.end()) return m_InterHeightMap[{x, z}];
    Biome biome = GetBiome(x, y, z);
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
    if (m_InterHeightMapOrder.size() >= m_InterHeightMapLimits)
    {
        m_InterHeightMap.erase(m_InterHeightMapOrder.front());
        m_InterHeightMapOrder.pop_front();
    }
    m_InterHeightMap [{x, z}] = res;
    m_InterHeightMapOrder.push_back({x, z});
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
    std::pair <BlockType, int> vegyandheight = GetVegetationAtLocation(x, y, z);
    if (y > height && y <= height + vegyandheight.second) return vegyandheight.first;
    if (y <= height) return BlockType::GRASS;
    return BlockType::EMPTY;
}

BlockType Map::GENForest(int x, int y, int z, int height)
{
    if (y > height + 10) return BlockType::EMPTY;
    std::pair <BlockType, int> vegyandheight = GetVegetationAtLocation(x, y, z);
    if (y > height && y <= height + vegyandheight.second) return vegyandheight.first;
    if (y > height)
    {
        //check surroundings for tree
        int radius = 2;
        for (int dx = -radius; dx <= radius; dx++)
        {
            for (int dz = -radius; dz <= radius; dz++)
            {
                std::pair <BlockType, int> veg = GetVegetationAtLocation(x + dx, 0, z + dz);
                if (veg.first == BlockType::OAKLOG)
                {
                    int height = (int)round(InterpolateHeightAtLocation(x + dx, 0, z + dz));
                    int tip = height + veg.second;
                    if (y >= tip && abs(dx) + abs(dz) + abs(y - tip) <= radius) return BlockType::OAKLEAF;
                }
                else if (veg.first == BlockType::BIRCHLOG)
                {
                    int height = (int)round(InterpolateHeightAtLocation(x + dx, 0, z + dz));
                    int tip = height + veg.second;
                    if (y >= tip && abs(dx) + abs(dz) + abs(y - tip) <= radius) return BlockType::BIRCHLEAF;
                }
            }
        }
    }
    if (y <= height) return BlockType::GRASS;
    return BlockType::EMPTY;
}
