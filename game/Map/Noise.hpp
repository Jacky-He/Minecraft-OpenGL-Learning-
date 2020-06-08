#pragma once
#include "Util.hpp"

class Noise
{
private:
    int m_P [512];
    int m_Permutation [256];
    
    double Fade (double t);
    double Lerp (double t, double a, double b);
    double Grad (int hash, double x, double y, double z);
public:
    Noise();
    ~Noise();
    double GenNoise (double x, double y, double z);
};
