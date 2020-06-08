#shader vertex
#version 330 core
    
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in float texID;

out float textureID;
out vec2 textureCoords;
uniform mat4 u_MVP;
    
void main()
{
    gl_Position = u_MVP * position;
    textureCoords = texCoords;
    textureID = texID;
}

#shader fragment
#version 330 core

out vec4 color;
in vec2 textureCoords;
in float textureID;
    
uniform sampler2D u_Textures [16];

void main()
{
    int index = int(textureID);
    color = texture (u_Textures[index], textureCoords);
}
