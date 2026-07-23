#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include "consoleLog.h"
#include "IsometricGrid.h"
#include "main.h"
#include "PanAndZoomCamera.h"
#include "rendering/shader.h"
#include "rendering/vertexBuffer.h"
#include "window.h"
#include <set>

int windowWidth = 1024;
int windowHeight = 768;

Window window;
IsometricGrid isometricGrid;
Shader shader;
PanAndZoomCamera panAndZoomCamera;

float aspect = 0.0f;
glm::vec2 cursorV;
bool processMotion = false;
float scrollFactor = 0.0f;

void CreateWindow()
{
    window.Init(windowWidth, windowHeight, "pan and zoom camera with isometric grid");

    window.AddMouseMoveHandler(
        [](float x, float y)
        {
            LOG_INFO("cursor screen %f %f", x, y);

            cursorV.x = ((x / windowWidth) * 2.0f - 1.0f) * aspect;
            cursorV.y = -y / windowHeight * 2.0f + 1.0f;

            auto cursorWorld = panAndZoomCamera.GetWorldTransform() * glm::vec3(cursorV, 1.0f);

            LOG_INFO("cursor cartesian %f %f", cursorV.x, cursorV.y);

            isometricGrid.PickCell(cursorWorld);
        }
    );

    window.AddMouseClickHandler(
        [](int button, int action)
        {
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
                processMotion = true;
            }

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
                processMotion = false;
            }
        }
    );

    window.AddMouseScrollHandler(
        [](float sf)
        {
            scrollFactor = sf;
        }
    );

    window.AddResizeHandler(
        [](int width, int height)
        {
            windowWidth = width;
            windowHeight = height;

            aspect = static_cast<float>(width) / static_cast<float>(height);

            const auto& contentScale = window.GetContentScale();
            glViewport(0, 0, width * contentScale.x, height * contentScale.y);
        }
    );
}

int main(int argc, char** argv)
{
    aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    CreateWindow();

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false) {
        LOG_ERROR("Unable to initialize GLAD");
        return -1;
    }

    bool res = shader.LoadFromFiles("./shaders/camera.vs.glsl", "./shaders/camera.fs.glsl");
    if (res == false) {
        LOG_ERROR("Unable to load shaders");
        return -1;
    }

    /**/
    VertexBuffer fillBuffer;
    fillBuffer.Init();

    std::vector<VertexBuffer::Vertex> fillVertices;
    fillVertices.push_back({{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}});
    fillVertices.push_back({{-0.5f, 0.5f}, {0.0f, 0.5f, 0.0f}});
    fillVertices.push_back({{0.5f, 0.5f}, {0.0f, 0.0f, 0.5f}});
    fillVertices.push_back({{0.5f, 0.5f}, {0.0f, 0.0f, 0.5f}});
    fillVertices.push_back({{0.5f, -0.5f}, {0.0f, 0.5f, 0.5f}});
    fillVertices.push_back({{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}});

    fillBuffer.Update(fillVertices);

    isometricGrid.Init({4,3}, {0.2f, 0.1f}, {});
    panAndZoomCamera.Init();

    while (window.ShouldClose() == false) {

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        panAndZoomCamera.SetInMotion(processMotion);
        panAndZoomCamera.SetCursorPos(cursorV);
        panAndZoomCamera.SetScrollDelta(scrollFactor);
        panAndZoomCamera.Update();

        scrollFactor = 0.0f;

        shader.Use();
        shader.SetUniform("transform", panAndZoomCamera.GetViewTransform());
        shader.SetUniform("aspect", aspect);
        //fillBuffer.Draw(GL_TRIANGLES, 6);
        isometricGrid.Draw();

        window.ProcessEvents();
    }

    return 0;
}
