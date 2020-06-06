#ifndef RENDERER_H
#define RENDERER_H

#include <FDCore/TimeManager.h>

#include <FDGL/BaseOpenGLContext.h>
#include <FDGL/BaseOpenGLWindow.h>
#include <FDGL/OpenGLShaderProgram.h>
#include <FDGL/OpenGLTexture.h>
#include <FDGL/BaseRenderer.h>

#include <FDGL/OpenGLVertexArray.h>

#include <FD3D/Camera/Camera.h>
#include <FD3D/Light/Light.h>
#include <FD3D/SceneGraph/Scene.h>
#include <FD3D/Utils/TransformStack.h>

#include <stack>
#include <unordered_map>
#include <unordered_set>

namespace FDGL
{
    class BufferedMeshComponent;
}

class Renderer final : public FDGL::BaseRenderer
{
    private:
        enum class NodeState : int8_t
        {
            Invalid  =-1,
            New = 0,
            Explored,
            Rendered,
            Done
        };

        class NodeTransitionGuard
        {
            private:
                Renderer &m_renderer;
                const FD3D::SceneNode::id_type m_id;
                NodeState &m_state;

            public:
                NodeTransitionGuard(Renderer &r, FD3D::SceneNode::id_type id, NodeState &state);
                ~NodeTransitionGuard();

                FD3D::SceneNode::id_type getId();
                NodeState getState() const;

            private:
                void applyTransition();
        };

    protected:
        FD3D::TransformStack m_transformStack;
        FD3D::Camera *m_activeCamera;
        FD3D::Light *m_light;

    public:
        Renderer(FDGL::BaseOpenGLContext &ctx);
        ~Renderer() override = default;

        FD3D::TransformStack &getTransformStack()
        {
            return m_transformStack;
        }

        const FD3D::TransformStack &getTransformStack() const
        {
            return m_transformStack;
        }

        void pushTransform(const FD3D::Transform &trans)
        {
            m_transformStack.push(trans);
        }

        void popTransform()
        {
            m_transformStack.pop();
        }

        bool hasActiveCamera() const
        {
            return m_activeCamera == nullptr;
        }

        FD3D::Camera *getActiveCamera()
        {
            return m_activeCamera;
        }

        const FD3D::Camera *getActiveCamera() const
        {
            return m_activeCamera;
        }

        void setActiveCamera(FD3D::Camera *cam)
        {
            m_activeCamera = cam;
        }

        FD3D::Projection *getProjection()
        {
            return (m_activeCamera == nullptr ?
                        nullptr
                      : &m_activeCamera->projection);
        }

        const FD3D::Projection *getProjection() const
        {
            return (m_activeCamera == nullptr ?
                        nullptr
                      : &m_activeCamera->projection);
        }

        FD3D::Light *getLight()
        {
            return m_light;
        }

        const FD3D::Light *getLight() const
        {
            return m_light;
        }

        void setLight(FD3D::Light *light)
        {
            m_light = light;
        }

        void renderScene();
        void renderNode(FD3D::SceneNodeProxy node);
        void addNodeChildren(FD3D::SceneNode::id_type id,
                             std::stack<FD3D::SceneNode::id_type> &todo);

        void initProjection(int width, int height);

    protected:

        void init(FDGL::BaseOpenGLWindow &w) override;

        void quit(FDGL::BaseOpenGLWindow &) override;

        void render(FDGL::BaseOpenGLWindow &w) override;

        void resize(FDGL::BaseOpenGLWindow &, int width, int height) override;

        void errorHandler(FDGL::ErrorSoure source, FDGL::ErrorType type,
                          uint32_t id, FDGL::ErrorSeverity level,
                          const std::string &msg) const override;


    private:

        void initContext();

        void initWindow(FDGL::BaseOpenGLWindow &w);

        void initScene();


        bool loadScene(const std::string &path);

        void drawLight();

        void rotateCamera(float elapsedTime);
        void rotateLight(float elapsedTime);

        void renderMesh(FDGL::BufferedMeshComponent *mesh);
};


#endif // RENDERER_H
