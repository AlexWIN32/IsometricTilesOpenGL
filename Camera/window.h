#pragma once
#include <string>
#include <functional>
#include <vector>
#include <glm/vec2.hpp>

struct GLFWwindow;

class Window
{
public:
    using MouseScrollHandler = std::function<void(float)>;
    using MouseMoveHandler = std::function<void(float, float)>;
    using MouseClickHandler = std::function<void(int, int)>;
    using ResizeHandler = std::function<void(int, int)>;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window() = default;
    Window(Window&&) = default;
    Window& operator=(Window&&) = default;
    ~Window();
    bool ShouldClose() const;
    bool Init(unsigned int width, unsigned int height, std::string title);
    void SetWindowCallbacks();
    void ProcessEvents();
    void AddMouseMoveHandler(const MouseMoveHandler& handler);
    void AddMouseClickHandler(const MouseClickHandler& handler);
    void AddMouseScrollHandler(const MouseScrollHandler& handler);
    void AddResizeHandler(const ResizeHandler& handler);
    const glm::vec2& GetContentScale() const;
private:
    std::vector<std::function<void(float, float)>> mouseMoveHandlers;
    std::vector<MouseClickHandler> mouseClickHandlers;
    std::vector<MouseScrollHandler> mouseScrollHandlers;
    std::vector<ResizeHandler> resizeHandlers;
    GLFWwindow* window = nullptr;
    glm::vec2 contentScale = {};
};
