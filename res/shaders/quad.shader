#shader vertex
#version 330 core
    
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in float texID;
layout (location = 3) in vec3 normalVec;

out float textureID;
out vec2 textureCoords;
out vec3 normal;
out vec3 fragPos;
uniform mat4 u_MVP;
    
void main()
{
    gl_Position = u_MVP * position;
    textureCoords = texCoords;
    textureID = texID;
    fragPos = vec3(position);
    normal = normalVec;
}

#shader fragment
#version 330 core
//#define NUM_POINT_LIGHTS 4

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
};

//struct PointLight {
//    vec3 position;
//    float constant;
//    float linear;
//    float quadratic;
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};


out vec4 fragColor;
in vec2 textureCoords;
in float textureID;
in vec3 fragPos;
in vec3 normal;
    
uniform sampler2D u_Textures [16];
uniform DirLight u_DirLight;
//uniform PointLight pointLights [NUM_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec4 texColor)
{
    vec3 lightDir = normalize (-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 ambient = light.ambient * vec3(texColor);
    vec3 diffuse = light.diffuse * diff * vec3(texColor);
    return (ambient + diffuse);
}

//vec3 CalcPointLight (PointLight light, vec3 normal, vec3 fragPos)
//{
//
//}

void main()
{
    int index = int(textureID);
    vec4 texColor = texture (u_Textures[index], textureCoords);
    if (texColor.a < 1)
        discard;
    vec3 norm = normalize(normal);
    vec3 res = CalcDirLight(u_DirLight, norm, texColor);
    fragColor = vec4(res, 1.0);
}
