#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

uniform mat3x3 transform;
uniform float aspect;

out vec3 ourColor;

void main()
{
    vec3 pos = transform * vec3(aPos, 1.0f);
    pos.x /= aspect;

    gl_Position = vec4(pos, 1.0);
    ourColor = aColor;
}
