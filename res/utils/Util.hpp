#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <iostream>
#include <functional>
#include <stdlib.h>
#include <vector>
#include <string>
#include <memory>
#include <list>
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
