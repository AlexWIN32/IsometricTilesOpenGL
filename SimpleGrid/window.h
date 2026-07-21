#pragma once
#include <string>
#include <functional>
#include <vector>
#include <glm/vec2.hpp>

struct GLFWwindow;

class Window
{
public:
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window() = default;
    Window(Window&&) = default;
    Window& operator=(Window&&) = default;
    ~Window();
    bool ShouldClose() const;
    bool Init(unsigned int width, unsigned int height, std::string title);
    void ProcessEvents();
    void AddMouseMoveHandler(const std::function<void(float, float)>& handler);
    void AddResizeHandler(const std::function<void(int, int)>& handler);
    const glm::vec2& GetContentScale() const;
private:
    std::vector<std::function<void(float, float)>> mouseMoveHandlers;
    std::vector<std::function<void(int, int)>> resizeHandlers;
    GLFWwindow* window = nullptr;
    glm::vec2 contentScale;
};
