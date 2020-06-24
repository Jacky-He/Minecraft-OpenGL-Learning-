#pragma once
#include "Util.hpp"
#include "Timer.hpp"
#include "Light.hpp"

class Constellations
{
public:
    Constellations(double minutes);
    ~Constellations();
    
    Light Update();
    void Start();
    
    glm::vec3 m_SkyColor;
    
private:
    double m_DayNightCycleLength; //20 minutes * 60 seconds/minute;
    Timer m_Timer;
    
    void UpdateSkyColor (glm::vec3 color);
    void DrawSunAndMoon();
};
