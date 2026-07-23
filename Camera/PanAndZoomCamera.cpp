//based on https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/SmallerProjects/OneLoneCoder_PanAndZoom.cpp

#include "PanAndZoomCamera.h"

void PanAndZoomCamera::Init()
{
    viewTransform = glm::mat3(1.0f);
}

const glm::mat3& PanAndZoomCamera::GetViewTransform() const
{
    return viewTransform;
}

const glm::mat3& PanAndZoomCamera::GetWorldTransform() const
{
    return worldTransform;
}

void PanAndZoomCamera::SetCursorPos(const glm::vec2& cursorPos)
{
    this->cursorPos = cursorPos;
}

void PanAndZoomCamera::SetScrollDelta(float scrollDelta)
{
    this->scrollDelta = scrollDelta;
}

void PanAndZoomCamera::SetInMotion(bool inMotion)
{
    this->inMotion = inMotion;
}

void PanAndZoomCamera::Update()
{
    if (inMotion == true) {
        //since cursor position is in view space it means that
        //zooming is applied. We need position in world space so
        //undo zooming

        position -= (cursorPos - oldCursorPos) / zoom;
    }

    oldCursorPos = cursorPos;

    glm::vec2 cursorPosBeforeZoomW = cursorPos / zoom + position;

    if (scrollDelta > 0.0f) {
        zoom *= 1.1f;
    } else if (scrollDelta < 0.0f) {
        zoom = std::max(zoom * 0.9f, 0.01f);
    }

    glm::vec2 cursorPosAfrerZoomW = cursorPos / zoom + position;

    position -= cursorPosAfrerZoomW - cursorPosBeforeZoomW;

    UpdateTransforms();
}

void PanAndZoomCamera::UpdateTransforms()
{
    /*
    we need to first apply negative offset and then zooming
    in matrix form this transform will look like this

        |1  0  0|   |z 0 0|   |z      0  0|
        |0  1  0| * |0 z 0| = |0      z  0|
        |-x -y 1|   |0 0 1|   |-x*z -y*z 1|
    */

    viewTransform =
    {
        zoom, 0.0f, 0.0f,
        0.0f, zoom, 0.0f,
        -position.x*zoom, -position.y*zoom, 1.0f,
    };

    /*
    world transform is view transform's inverse

        W = V' = (T*Z)'= Z'*T'

    since

            |1  0  0|      |z 0 0|
        T = |0  1  0|  Z = |0 z 0|
            |-x -y 1|      |0 0 1|

    we have

                |1/z  0  0|   |1 0 0|   |1/z  0  0|
        Z'*T' = | 0  1/z 0| * |0 1 0| = | 0  1/z 0|
                | 0   0  1|   |x y 1|   | x   y  1|
    */

    worldTransform =
    {
        1/zoom, 0.0f, 0.0f,
        0.0f, 1/zoom, 0.0f,
        position.x, position.y, 1.0f,
    };
}

