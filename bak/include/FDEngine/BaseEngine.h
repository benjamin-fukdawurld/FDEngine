#ifndef BASEENGINE_H
#define BASEENGINE_H

#include <FDCore/TimeManager.h>
#include <FDCore/ResourceManager.h>

#include <FD3D/SceneGraph/SceneManager.h>
#include <FD3D/SceneGraph/Scene.h>

#include <FDGL/BaseOpenGLContext.h>
#include <FDGL/BaseRenderer.h>

namespace FDEngine
{
    class FD_EXPORT BaseEngine
    {
        protected:
            FDGL::BaseOpenGLContext &m_ctx;
            FDGL::BaseOpenGLWindow &m_window;
            FDGL::BaseRenderer &m_renderer;
            FDCore::TimeManager<> m_timeMgr;

            FD3D::Scene m_scene;
            FDCore::ResourceManager m_resourceManager;
            FD3D::SceneManager m_sceneManager;

        public:
            BaseEngine(FDGL::BaseOpenGLContext &ctx,
                       FDGL::BaseOpenGLWindow &window,
                       FDGL::BaseRenderer &renderer);

            virtual ~BaseEngine() = default;

            FDGL::BaseOpenGLContext &getContext();

            const FDGL::BaseOpenGLContext &getContext() const;

            FDGL::BaseOpenGLWindow &getWindow();

            const FDGL::BaseOpenGLWindow &getWindow() const;

            FD3D::Scene &getScene();

            const FD3D::Scene &getScene() const;

            FDGL::BaseRenderer &getRenderer();

            const FDGL::BaseRenderer &getRenderer() const;

            FDCore::TimeManager<> &getTimeManager();

            const FDCore::TimeManager<> &getTimeManager() const;

            FD3D::SceneManager &getSceneManager() { return m_sceneManager; }
            const FD3D::SceneManager &getSceneManager() const { return m_sceneManager; }
            FDCore::ResourceManager &getResourceManager() { return m_resourceManager; }
            const FDCore::ResourceManager &getResourceManager() const { return m_resourceManager; }

            virtual void initSceneManager();

            virtual bool init(int argc, char **argv) = 0;

            virtual void quit() = 0;

            virtual void update() = 0;

            virtual int run() = 0;

            virtual bool isRunning() const = 0;

            virtual void stop() = 0;

    };
}

#endif // BASEENGINE_H
