#pragma once
#include "Util.hpp"
#include "Timer.hpp"
#include "Light.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Renderer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Constellations
{
public:
    static void SetUp();
    
    Constellations(double minutes);
    ~Constellations();
    
    Light Update(glm::vec3 cameraDirection);
    void Start();
    
    glm::vec3 m_SkyColor;
    
private:
    static std::unique_ptr<VertexBuffer> s_VBO;
    static std::unique_ptr<VertexArray> s_VAO;
    static std::unique_ptr<IndexBuffer> s_IBO;
    static std::unique_ptr<Shader> s_Shader;
    static std::unique_ptr<Renderer> s_Renderer;
    
    double m_DayNightCycleLength; //20 minutes * 60 seconds/minute;
    Timer m_Timer;
    
    void UpdateSkyColor (glm::vec3 color);
    void DrawSunAndMoon(glm::vec3 cameraDirection, double angle);
};
