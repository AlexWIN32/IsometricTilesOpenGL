#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

uniform mat3x3 transform;
uniform float aspect;

out vec3 ourColor;

void main()
{
    vec3 pos = transform * vec3(aPos, 1.0f);

    /*
    pos is in isometric space which is unit square centered in screen

           |
         --+------------->
        |  |0      |  |
        |  |       |  |
        |  |       |  |
        |__|_______|__|
           |       1,1
          \|/

    we need to convert pos from isometric space to NDC space
    */

    pos.x = (pos.x * 2.0f - 1.0f) / aspect;
    pos.y = (1.0f - pos.y) * 2.0f - 1.0f;

    gl_Position = vec4(pos, 1.0);
    ourColor = aColor;
}
