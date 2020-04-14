#include "include/FDEngine/BaseEngine.h"

FDEngine::BaseEngine::BaseEngine(FDGL::BaseOpenGLContext &ctx, FDGL::BaseOpenGLWindow &window, FDGL::BaseRenderer &renderer) :
    m_ctx(ctx),
    m_window(window),
    m_renderer(renderer)
{}

FDGL::BaseOpenGLContext &FDEngine::BaseEngine::getContext()
{
    return m_ctx;
}

const FDGL::BaseOpenGLContext &FDEngine::BaseEngine::getContext() const
{
    return m_ctx;
}

FDGL::BaseOpenGLWindow &FDEngine::BaseEngine::getWindow()
{
    return m_window;
}

const FDGL::BaseOpenGLWindow &FDEngine::BaseEngine::getWindow() const
{
    return m_window;
}

FD3D::Scene &FDEngine::BaseEngine::getScene()
{
    return m_scene;
}

const FD3D::Scene &FDEngine::BaseEngine::getScene() const
{
    return m_scene;
}

FDGL::BaseRenderer &FDEngine::BaseEngine::getRenderer()
{
    return m_renderer;
}

const FDGL::BaseRenderer &FDEngine::BaseEngine::getRenderer() const
{
    return m_renderer;
}

FDCore::TimeManager<> &FDEngine::BaseEngine::getTimeManager()
{
    return m_timeMgr;
}

const FDCore::TimeManager<> &FDEngine::BaseEngine::getTimeManager() const
{
    return m_timeMgr;
}
