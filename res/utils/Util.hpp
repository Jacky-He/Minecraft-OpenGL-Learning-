#pragma once

#include <GL/glew.h>
#include <assert.h>
#include <iostream>
#include <functional>
#include <stdlib.h>
#include <vector>
#include <string>
#include <memory>

#define GLCall(x) GLClearError(); x; assert(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);
