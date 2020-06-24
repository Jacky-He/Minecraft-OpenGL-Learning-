#pragma once
#include "Util.hpp"

class Constants
{
public:
    static float screenAspect;
    static float screenHeight;
    static float screenWidth;
    static double deltaTime;
    static float gravity;
    static float terminalVelocity;
    static float jumpVelocity;
    static float EPSILONf;
    static unsigned int cubeIndices [36];
    static unsigned int twoTexIndices [12];
    static float cubeModel [216];
    static float twoTexModel [72];
    static float cubeOutLineModel [384];
    static float quadModel [24];
};
