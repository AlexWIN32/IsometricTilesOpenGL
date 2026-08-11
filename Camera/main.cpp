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

glm::vec2 squareCenter(0.7f, 0.5f);
glm::vec2 squareHalfSize(0.25f, 0.25f);

static void ProcessMouseMove(float x, float y)
{
    LOG_INFO("cursor screen %f %f", x, y);

    /*
    we have cursor coords in cursor space

        ---------------->
        |0            |
        |             |
        |             |
        |_____________|
        |             W,H
        \|/

    isometric space is unit square centered in screen

           |
         --+------------->
        |  |0      |  |
        |  |       |  |
        |  |       |  |
        |__|_______|__|
           |       1,1
          \|/

    we need to convert cusror space to isometric space
    first we need to normalize cursor coords

        Nx = x / windowWidth
        Ny = y / windowHeight

    then we need to convert normalized cursor coords to aspect space
    where x in [-A, A] and y in [-1, 1], A is W/H

               /|\
         _______|_______
        |A,1    |       |
        |       |       |
        --------+--------->
        |       |       |
        |_______|_______|
                |      -A,-1

        Ax = (Nx * 2 - 1) * A;
        Ay = (1 - Ny) * 2 - 1;

    then we need to convert aspect space to isometric space

        Ix = (Ax + 1) / 2
        Iy = 1 - (Ay + 1) / 2

    if we plug Ax and Ay into Ix and Iy we get

        Ix = ((Nx * 2 - 1) * A + 1) / 2
        Ix = (Nx*2*A - A + 1) / 2
        Ix = Nx*A - A/2 + 1/2

        Ix = 1 - (((1 - Ny) * 2 - 1) + 1) / 2
        Ix = 1 - ((2 - Ny*2 - 1) + 1) / 2
        Ix = 1 - (1 - Ny*2 + 1) / 2
        Ix = 1 - (2 - Ny*2) / 2
        Ix = 1 - (1 - Ny)
        Ix = 1 - 1 + Ny
        Ix = Ny
    */

    glm::vec2 cursorS(x / windowWidth, y / windowHeight);

    cursorV.x = cursorS.x * aspect - aspect * 0.5f + 0.5f;
    cursorV.y = cursorS.y;

    LOG_INFO("cursor world %f %f", cursorV.x, cursorV.y);
    auto cursorWorld = panAndZoomCamera.GetWorldTransform() * glm::vec3(cursorV, 1.0f);

    isometricGrid.PickCell(cursorWorld);
}

void CreateWindow()
{
    window.Init(windowWidth, windowHeight, "pan and zoom camera with isometric grid");

    window.AddMouseMoveHandler(ProcessMouseMove);

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

    isometricGrid.Init({4,3}, {0.2f, 0.1f}, {0.5f, 0.5f});
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
        isometricGrid.Draw();

        window.ProcessEvents();
    }

    return 0;
}
