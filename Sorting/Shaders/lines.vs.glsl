#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

uniform float aspect;

out vec3 ourColor;

void main()
{
    //vec3 pos = vec3(aPos, 1.0f);
    //pos.x /= aspect;
    //gl_Position = vec4(pos, 1.0);

    vec3 pos = vec3(aPos, 1.0f);
    pos.x = pos.x * 2.0f - 1.0f;
    pos.y = (1.0f - pos.y) * 2.0f - 1.0f;

    pos.x /= aspect;

    gl_Position = vec4(pos.x, pos.y, 0.0f, 1.0f);

    ourColor = aColor;
}
