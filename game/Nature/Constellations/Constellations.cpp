#include "Constellations.hpp"

Constellations::Constellations(double minutes):m_Timer()
{
    m_DayNightCycleLength = minutes*60.0;
    m_SkyColor = glm::vec3 (124.0f/255.05f, 204.0f/255.0f, 239.0f/255.0f);
}

Constellations::~Constellations()
{
    
}

void Constellations::Start()
{
    m_Timer.Reset();
}

void Constellations::UpdateSkyColor(glm::vec3 color)
{
    GLCall(glClearColor(color.x, color.y, color.z, 1.0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));
}

void Constellations::DrawSunAndMoon()
{
    
}

Light Constellations::Update()
{
    DrawSunAndMoon();
    
    double elapsedseconds = m_Timer.Report(false);
    double currpos = std::fmod(elapsedseconds, m_DayNightCycleLength)/m_DayNightCycleLength*2*M_PI;
    double angle = std::fmod(currpos, M_PI);
    Light l (LightSourceType::DIRECTIONAL);
    glm::vec3 westdir = glm::vec3 (-1.0f, 0.0f, 0.0f);
    glm::vec3 raydir = glm::rotateZ(westdir, float(angle));
    
    double daylevel = 15;
    double nightlevel = 7;
    
    float dayfactor = pow(0.8f, 15 - daylevel);
    float nightfactor = pow(0.8f, 15 - nightlevel);
    
    float dayambient = 0.6f*dayfactor;
    float nightambient = 0.6f*nightfactor;
    float daydiffuse = 0.4f*dayfactor;
    float nightdiffuse = 0.4f*nightfactor;
    float dayskystrength = 1.0f*dayfactor;
    float nightskystrength = 1.0f*nightfactor;
    
    float skystrength = (currpos < M_PI ? dayskystrength : nightskystrength);
    float ambientStrength = (currpos < M_PI ? dayambient : nightambient); //day night
    float diffuseStrength = (currpos < M_PI ? daydiffuse : nightdiffuse); //day night
    float angtrans = M_PI/8.0;
    
    if (currpos > 2.0*M_PI - angtrans || currpos < angtrans) //night to day
    {
        float anglepassed = 0.0f;
        if (currpos > 2.0*M_PI - angtrans) anglepassed = currpos - 2.0*M_PI + angtrans;
        else if (currpos < angtrans) anglepassed = currpos + angtrans;
        float ratio = anglepassed/(2.0f*angtrans);
        ambientStrength = nightambient + ratio*(dayambient - nightambient);
        diffuseStrength = nightdiffuse + ratio*(daydiffuse - nightdiffuse);
        skystrength = nightskystrength + ratio*(dayskystrength - nightskystrength);
    }
    else if (currpos > M_PI - angtrans && currpos < M_PI + angtrans) //day to night
    {
        float anglepassed = currpos - (M_PI - angtrans);
        float ratio = anglepassed/(2.0f*angtrans);
        ambientStrength = dayambient + ratio*(nightambient - dayambient);
        diffuseStrength = daydiffuse + ratio*(nightdiffuse - daydiffuse);
        skystrength = dayskystrength + ratio*(nightskystrength - dayskystrength);
    }
    
    UpdateSkyColor (m_SkyColor*skystrength);
    
    glm::vec3 lightColor = glm::vec3 (1.0f, 1.0f, 1.0f);
    l.ambient = ambientStrength*lightColor;
    l.diffuse = diffuseStrength*lightColor;
    l.specular = glm::vec3 (0.0f, 0.0f, 0.0f); //no specular lights
    l.dir = raydir;
    return l;
}
