#pragma once
#include "Util.hpp"

enum class LightSourceType
{
    POINT, DIRECTIONAL
};

class Light
{
public:
    Light(LightSourceType type);
    ~Light();
    
    LightSourceType type;
    
    //all
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    //directional
    glm::vec3 dir;
    
    //point
    glm::vec3 pos;
    float constant;
    float linear;
    float quadratic;
private:
};
