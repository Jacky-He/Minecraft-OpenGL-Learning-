#include "Util.hpp"

std::mt19937 Util::g1((unsigned int)time(0));
int Util::RandInt(int a, int b){return std::uniform_int_distribution<int>(a, b)(g1);}

glm::vec3 Util::s_DirectionsUnitVectors [] =
{
    glm::vec3 (0, 0, 1),
    glm::vec3 (0, 0, -1),
    glm::vec3 (-1, 0, 0),
    glm::vec3 (1, 0, 0),
    glm::vec3 (0, 1, 0),
    glm::vec3 (0, -1, 0),
    
    glm::vec3 (0, 1, 1),
    glm::vec3 (0, 1, -1),
    glm::vec3 (-1, 1, 0),
    glm::vec3 (1, 1, 0),
    glm::vec3 (0, -1, 1),
    glm::vec3 (0, -1, -1),
    glm::vec3 (-1, -1, 0),
    glm::vec3 (1, -1, 0),
    glm::vec3 (-1, 0, 1),
    glm::vec3 (-1, 0, -1),
    glm::vec3 (1, 0, 1),
    glm::vec3 (1, 0, -1),
    
    glm::vec3 (-1, 1, 1),
    glm::vec3 (-1, 1, -1),
    glm::vec3 (1, 1, 1),
    glm::vec3 (1, 1, -1),
    glm::vec3 (-1, -1, 1),
    glm::vec3 (-1, -1, -1),
    glm::vec3 (1, -1, 1),
    glm::vec3 (1, -1, -1),
    
    glm::vec3 (0, 0, 0)
};

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    bool res = true;
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        res = false;
    }
    return res;
}


