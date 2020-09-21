#ifndef ENGINE_H
#define ENGINE_H

#include <FDEngine/BaseEngine.h>

#include <FDCore/AssociativeContainer.h>
#include <FDCore/ThreadPool.h>

#include <FDPython/Interpreter.h>

class Engine : public FDEngine::BaseEngine
{
    protected:
        FDCore::AssociativeContainer<FD3D::SceneNode::id_type, std::vector<FD3D::Component::id_type>> m_behaviors;
        FDPython::Interpreter m_pyInterpreter;
        FDCore::ThreadPool m_threadPool;

    public:
        Engine(FDGL::BaseOpenGLContext &ctx,
               FDGL::BaseOpenGLWindow &window,
               FDGL::BaseRenderer &renderer);
        ~Engine() override;

        FDCore::ThreadPool &getThreadPool();

        const FDCore::ThreadPool &getThreadPool() const;

        bool init(int argc, char **argv) override;

        void quit() override;

        void update() override;

        int run() override;

        bool isRunning() const override;

        void stop() override;

        void processInput(FDGL::BaseOpenGLWindow &window);

        void initSceneManager() override;

        void initScene();

        bool loadScene(const std::string &path);

    private:
        void updateTask(std::vector<FD3D::Component::id_type> &behaviorIds);

        void initSerialiseHandler();
        void initUnserialiseHandler();
};

#endif // ENGINE_H
