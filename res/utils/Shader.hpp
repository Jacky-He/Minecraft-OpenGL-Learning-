#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include "../libs/glm/glm.hpp"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& filepath);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;
    
    //set uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
    int GetUniformLocation(const std::string& name) const;
    unsigned int CompileShader (unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    ShaderProgramSource ParseShader (const std::string& filepath);
};