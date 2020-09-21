#ifndef GLFWIMPL_H
#define GLFWIMPL_H

#include <FDGL/BaseOpenGLContext.h>
#include <FDGL/BaseOpenGLWindow.h>

#include <GLFW/glfw3.h>

class Context : public FDGL::BaseOpenGLContext
{
    public:
        Context() : BaseOpenGLContext() {}

        ~Context() override;

        bool init() override;
        bool quit() override;
        void setCurrentContext(FDGL::BaseOpenGLWindow &window) override;
};

class Window : public FDGL::BaseOpenGLWindow
{
    protected:
        std::string m_title;
        GLFWwindow *m_window;

    public:
        Window();
        ~Window() override;

        GLFWwindow *getWindow() { return m_window; }
        const GLFWwindow *getWindow() const { return m_window; }

        bool create(int width, int height, const std::string &title) override;
        void destroy() override;
        bool isOpen() const override;

        void swapBuffer() const override;

        std::string getTitle() const override;
        void setTitle(const std::string &title) override;

        int getWidth() const override;
        void setWidth(int w) override;

        int getHeight() const override;
        void setHeight(int h) override;

        std::pair<int, int> getWindowSize() const;
};

#endif // GLFWIMPL_H
