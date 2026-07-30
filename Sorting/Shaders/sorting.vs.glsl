#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform float aspect;

out vec2 texCoord;

void main()
{
    vec3 pos = aPos;

    pos.x = pos.x * 2.0f - 1.0f;
    pos.y = (1.0f - pos.y) * 2.0f - 1.0f;


    gl_Position = vec4(pos.x / aspect, pos.y, 0.0f, 1.0);
    texCoord = aTexCoord;
}

