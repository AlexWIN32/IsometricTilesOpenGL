#pragma once
#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>

class PanAndZoomCamera
{
public:
    void Init();
    const glm::mat3& GetViewTransform() const;
    const glm::mat3& GetWorldTransform() const;
    void SetCursorPos(const glm::vec2& cursorPos);
    void SetScrollDelta(float scrollDelta);
    void SetInMotion(bool inMotion);
    void Update();
private:
    void UpdateTransforms();
    glm::mat3 worldTransform;
    glm::mat3 viewTransform;
    glm::vec2 position = {};
    glm::vec2 cursorPos = {};
    glm::vec2 oldCursorPos = {};
    float zoom = 1.0f;
    float scrollDelta = 0.0f;
    bool inMotion = false;
};
