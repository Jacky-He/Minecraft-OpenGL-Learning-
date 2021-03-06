#include "Constellations.hpp"
#include "Constants.hpp"
#include "Texture.hpp"
#include "Textures.hpp"

std::unique_ptr<VertexBuffer> Constellations::s_VBO = nullptr;
std::unique_ptr<VertexArray> Constellations::s_VAO = nullptr;
std::unique_ptr<IndexBuffer> Constellations::s_IBO = nullptr;
std::unique_ptr<Shader> Constellations::s_Shader = nullptr;
std::unique_ptr<Renderer> Constellations::s_Renderer = nullptr;

Constellations::Constellations(double minutes):m_Timer()
{
    m_DayNightCycleLength = minutes*60.0;
    m_SkyColor = glm::vec3 (124.0f/255.05f, 204.0f/255.0f, 239.0f/255.0f);
}

void Constellations::SetUp()
{
    s_VAO = std::make_unique<VertexArray>();
    s_VBO = std::make_unique<VertexBuffer>(Constants::quadModel, sizeof(Constants::quadModel), GL_STATIC_DRAW);
    unsigned int indices [] = {0, 1, 2, 2, 3, 0};
    s_IBO = std::make_unique<IndexBuffer>(indices, 6, GL_STATIC_DRAW);
    s_Shader = std::make_unique<Shader>("/Users/jackyhe/Desktop/DEV/Open GL/Minecraft/Minecraft/res/shaders/nolightingquad.shader");
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(1);
    s_VAO -> AddBuffer(*s_VBO, layout);
    int texid [16]; for (int i = 0; i < 16; i++) texid[i] = i;
    s_Shader -> SetUniformiv("u_Textures", texid, 16);
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

void Constellations::DrawSunAndMoon(glm::vec3 cameraDirection, double angle)
{
    Texture* suntex = Textures::GetTexture("sun");
    Texture* moontex = Textures::GetTexture("moon");
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), Constants::screenAspect, 0.1f, 300.0f);
    glm::mat4 view = glm::lookAt(glm::vec3 (2.0f, 0.0f, 0.0f), glm::vec3 (2.0f, 0.0f, 0.0f)+ cameraDirection, glm::vec3 (0.0f, 1.0f, 0.0f));
    glm::mat4 modelSun = glm::mat4(1.0f);
    glm::mat4 modelMoon = glm::mat4(1.0f);
    float distance = 256.0f;
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3 (0, 50, 50));
    
    //sun
    glm::mat4 rotationSun = glm::rotate(float(angle), glm::vec3 (0, 0, 1));
    glm::mat4 translationSun = glm::translate(glm::mat4(1.0f), glm::vec3 (distance*cos(angle), distance*sin(angle), 0));
    modelSun = translationSun*rotationSun*scale;
    //moon
    glm::mat4 rotationMoon = glm::rotate(float(M_PI + angle), glm::vec3 (0, 0, 1));
    glm::mat4 translationMoon = glm::translate(glm::mat4(1.0f), glm::vec3(-distance*cos(angle), -distance*sin(angle), 0));
    modelMoon = translationMoon*rotationMoon*scale;

    //draw sun
    suntex -> Bind(0);
    s_Shader -> SetUniformMat4f("u_MVP", proj*view*modelSun);
    s_Renderer -> Draw(*s_VAO, *s_IBO, *s_Shader);

    //draw moon
    moontex -> Bind(0);
    s_Shader -> SetUniformMat4f("u_MVP", proj*view*modelMoon);
    s_Renderer -> Draw(*s_VAO, *s_IBO, *s_Shader);
}

Light Constellations::Update(glm::vec3 cameraDirection)
{
    
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
    DrawSunAndMoon(cameraDirection, currpos);
    
    glm::vec3 lightColor = glm::vec3 (1.0f, 1.0f, 1.0f);
    l.ambient = ambientStrength*lightColor;
    l.diffuse = diffuseStrength*lightColor;
    l.specular = glm::vec3 (0.0f, 0.0f, 0.0f); //no specular lights
    l.dir = raydir;
    return l;
}
