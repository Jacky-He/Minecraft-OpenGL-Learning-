#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <iostream>
#include <functional>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <string>
#include <algorithm>
#include <memory>
#include <list>
#include <queue>
#include <stdio.h>
#include <future>
#include <ctime>
#include <mutex>
#include <numeric>
#include <time.h>
#include <random>
#include <unordered_map>
#include <map>
#include <set>
#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/matrix_transform.hpp"
#include "../libs/glm/gtc/matrix_access.hpp"
#include "../libs/glm/gtx/rotate_vector.hpp"

#define GLCall(x) GLClearError(); x; assert(GLLogCall(#x, __FILE__, __LINE__))
using std::cout;
using std::endl;

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

enum class BlockType
{
    EMPTY = -1, GRASS = 0, DIRT = 1, OAKLEAF = 2, OAKLOG = 3, SAND = 4,
    
    TALLGRASS = 100, AZUREBLUET = 101, CORNFLOWER = 102, OXEYEDAISY = 103
};

enum class Direction
{
    FORWARD = 0, BACKWARD = 1, LEFT = 2, RIGHT = 3, UP = 4, DOWN = 5, UPFORWARD = 6, UPBACKWARD = 7, UPLEFT = 8, UPRIGHT = 9, DOWNFOWARD = 10, DOWNBACKWARD = 11, DOWNLEFT = 12, DOWNRIGHT = 13, LEFTFORWARD = 14, LEFTBACKWARD = 15, RIGHTFORWARD = 16, RIGHTBACKWARD = 17, LEFTUPFORWARD = 18, LEFTUPBACKWARD = 19, RIHGTUPFORWARD = 20, RIGHTUPBACKWARD = 21, LEFTDOWNFORWARD = 22, LEFTDOWNBACKWARD = 23, RIGHTDOWNFORWARD = 24, RIGHTDOWNBACKWARD = 25, CENTER = 26
};

enum class ItemType
{
    Block, Tool, Entity
};

enum class Biome
{
    WATER, PLAIN, FOREST
};

class Util
{
private:
    static std::mt19937 g1;
public:
    static glm::vec3 s_DirectionsUnitVectors [27];
    static int RandInt (int a, int b);
    static bool isEmptyWhenRendering (BlockType type);
    static bool isTwoTex (BlockType type);
    static bool isCollidable (BlockType type);
    static bool isBreakable (BlockType type);
};
