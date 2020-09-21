#include "GLFWImpl.h"

#include <iostream>

Context::~Context()
{
    quit();
}

bool Context::init()
{
    if(glfwInit() == GLFW_FALSE)
    {
        std::cerr << __func__ << ": Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

bool Context::quit()
{
    glfwTerminate();
    return true;
}

void Context::setCurrentContext(FDGL::BaseOpenGLWindow &window)
{
    glfwMakeContextCurrent(static_cast<Window&>(window).getWindow());
}





Window::Window() : BaseOpenGLWindow(), m_window(nullptr) {}

Window::~Window()
{
    if(!m_window)
        return;

    destroy();
    quit();
}

bool Window::create(int width, int height, const std::string &title)
{
    m_title = title;
    m_window = glfwCreateWindow(width, height, m_title.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        std::cout << __func__ << ": Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_window);

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *w, int width, int height)
    {
        reinterpret_cast<Window*>(w)->onResize(width, height);
        glViewport(0, 0, width, height);
    });

    return true;
}

void Window::destroy()
{
    glfwDestroyWindow(m_window);
}

bool Window::isOpen() const
{
    return glfwWindowShouldClose(m_window) == 0;
}

void Window::swapBuffer() const
{
    glfwSwapBuffers(m_window);
}

std::string Window::getTitle() const
{
    return m_title;
}

void Window::setTitle(const std::string &title)
{
    m_title = title;
    glfwSetWindowTitle(m_window, m_title.c_str());
}

int Window::getWidth() const
{
    int w, h;
    glfwGetWindowSize(m_window, &w, &h);
    return w;
}

void Window::setWidth(int w)
{
    glfwSetWindowSize(m_window, w, getHeight());
}

int Window::getHeight() const
{
    int w, h;
    glfwGetWindowSize(m_window, &w, &h);
    return h;
}

void Window::setHeight(int h)
{
    glfwSetWindowSize(m_window, getWidth(), h);
}

std::pair<int, int> Window::getWindowSize() const
{
    std::pair<int, int> result;
    glfwGetWindowSize(m_window, &(result.first), &(result.second));
    return result;
}
