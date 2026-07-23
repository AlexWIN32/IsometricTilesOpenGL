#include <GLFW/glfw3.h>
#include "consoleLog.h"
#include "window.h"

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(window);
}

bool Window::Init(unsigned int width, unsigned int height, std::string title)
{
    if (glfwInit() == false) {
        LOG_ERROR("Unable to initialize glfw");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE); //without this on my machine window appears smaller than it sould

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (window == nullptr) {
        glfwTerminate();
        LOG_ERROR("Unable to create window");
        return false;
    }

    glfwMakeContextCurrent(window);

    glfwGetWindowContentScale(window, &contentScale.x, &contentScale.y);

    SetWindowCallbacks();

    return true;
}

void Window::SetWindowCallbacks()
{
    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window,
        [](GLFWwindow* win, int width, int height)
        {
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(win));

            width /= window->contentScale.x;
            height /= window->contentScale.y;

            for (const auto& handler : window->resizeHandlers) {
                handler(width, height);
            }
        }
    );

    glfwSetCursorPosCallback(window,
        [](GLFWwindow* win, double xpos, double ypos)
        {
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(win));

            xpos /= window->contentScale.x;
            ypos /= window->contentScale.y;

            for (const auto& handler : window->mouseMoveHandlers) {
                handler(xpos, ypos);
            }
        }
    );

    glfwSetMouseButtonCallback(window,
        [](GLFWwindow* win, int button, int action, int mods)
        {
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(win));

            for (const auto& handler : window->mouseClickHandlers) {
                handler(button, action);
            }
        }
    );

    glfwSetScrollCallback(window,
        [](GLFWwindow* win, double xoffset, double yoffset)
        {
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(win));

            for (const auto& handler : window->mouseScrollHandlers) {
                handler(yoffset);
            }
        }
    );
}

void Window::AddMouseMoveHandler(const MouseMoveHandler& handler)
{
    mouseMoveHandlers.push_back(handler);
}

void Window::AddMouseClickHandler(const MouseClickHandler& handler)
{
    mouseClickHandlers.push_back(handler);
}

void Window::AddMouseScrollHandler(const MouseScrollHandler& handler)
{
    mouseScrollHandlers.push_back(handler);
}

void Window::AddResizeHandler(const ResizeHandler& handler)
{
    resizeHandlers.push_back(handler);
}

const glm::vec2& Window::GetContentScale() const
{
    return contentScale;
}

void Window::ProcessEvents()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
