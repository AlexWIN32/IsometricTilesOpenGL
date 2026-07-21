#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "consoleLog.h"
#include "rendering/shader.h"
#include "rendering/vertexBuffer.h"
#include "window.h"
#include "IsometricGrid.h"

int windowWidth = 1024;
int windowHeight = 768;

Window window;
IsometricGrid isometricGrid;
Shader shader;

float aspect = 0.0f;

int main(int argc, char** argv)
{
    aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    window.Init(windowWidth, windowHeight, "simple isometrig grid");
    window.AddMouseMoveHandler(
        [] (float x, float y)
        {
            LOG_INFO("cursor screen %f %f", x, y);

            x = ((x / windowWidth) * 2.0f - 1.0f) * aspect;
            y = -y / windowHeight * 2.0f + 1.0f;

            LOG_INFO("cursor cartesian %f %f", x, y);

            isometricGrid.PickCell({x, y});
        }
    );
    window.AddResizeHandler(
        [] (int width, int height)
        {
            windowWidth = width;
            windowHeight = height;

            aspect = static_cast<float>(width) / static_cast<float>(height);

            const auto& contentScale = window.GetContentScale();
            glViewport(0, 0, width * contentScale.x, height * contentScale.y);
        }
    );

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false) {
        LOG_ERROR("Unable to initialize GLAD");
        return -1;
    }

    bool res = shader.LoadFromFiles("./shaders/simple_grid.vs.glsl", "./shaders/simple_grid.fs.glsl");
    if (res == false) {
        LOG_ERROR("Unable to load shaders");
        return -1;
    }

    isometricGrid.Init({4,3}, {0.2f, 0.1f});

    while (window.ShouldClose() == false) {

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        shader.SetUniform("aspect", aspect);
        isometricGrid.Draw();

        window.ProcessEvents();
    }

    return 0;
}
