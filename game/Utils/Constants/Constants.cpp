#include "Constants.hpp"

float Constants::screenAspect = 800.0f/600.0f;
double Constants::deltaTime = 0;

float Constants::EPSILONf = 0.0001f;

float Constants::gravity = 9.81f*2.9f;
float Constants::terminalVelocity = 22.0f;
float Constants::jumpVelocity = 10.0f;

unsigned int Constants::cubeIndices [] =
{
    0, 1, 2,
    2, 3, 0,
    
    4, 5, 6,
    6, 7, 4,
    
    8, 9, 10,
    10, 11, 8,
    
    12, 13, 14,
    14, 15, 12,
    
    16, 17, 18,
    18, 19, 16,
    
    20, 21, 22,
    22, 23, 20
};

float Constants::cubeModel [] = {
    
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,//forward
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
    
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,//back
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 3.0f,//left
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 3.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 3.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 3.0f,
    
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 2.0f,//right
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 2.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 2.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 2.0f,
    
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 4.0f, //top
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 4.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 4.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 4.0f,
    
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 5.0f, //bottom
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 5.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 5.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 5.0f
};
