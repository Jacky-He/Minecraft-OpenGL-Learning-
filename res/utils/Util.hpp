#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <iostream>
#include <functional>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <list>
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
#include "../libs/glm/gtx/rotate_vector.hpp"

#define GLCall(x) GLClearError(); x; assert(GLLogCall(#x, __FILE__, __LINE__))
using std::cout;
using std::endl;

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

enum class BlockType
{
    GRASS = 0, EMPTY = -1
};

enum class Direction
{
    FORWARD = 0, BACKWARD = 1, LEFT = 2, RIGHT = 3, UP = 4, DOWN = 5, UPFORWARD = 6, UPBACKWARD = 7, UPLEFT = 8, UPRIGHT = 9, DOWNFOWARD = 10, DOWNBACKWARD = 11, DOWNLEFT = 12, DOWNRIGHT = 13, LEFTFORWARD = 14, LEFTBACKWARD = 15, RIGHTFORWARD = 16, RIGHTBACKWARD = 17, LEFTUPFORWARD = 18, LEFTUPBACKWARD = 19, RIHGTUPFORWARD = 20, RIGHTUPBACKWARD = 21, LEFTDOWNFORWARD = 22, LEFTDOWNBACKWARD = 23, RIGHTDOWNFORWARD = 24, RIGHTDOWNBACKWARD = 25, CENTER = 26
};

class Util
{
private:
    static std::mt19937 g1;
public:
    static glm::vec3 s_DirectionsUnitVectors [27];
    static int RandInt (int a, int b);
};
