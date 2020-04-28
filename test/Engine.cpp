#include "Engine.h"

#include <GLFW/glfw3.h>

#include <FDGL/BufferedMesh.h>
#include <FDGL/OpenGLVertexArray.h>
#include "../FDGL/test/Renderer.h"
#include "../FDGL/test/GLUtils.h"
#include "../FDGL/test/GLFWImpl.h"

#include <FD3D/SceneGraph/SceneLoader.h>
#include <FD3D/Behavior/StrategyBehavior.h>

#include <FDJson/Json_fwd.h>
#include <FD3D/Serialization/Json/FD3D_Json_fwd.h>

#include <FDJson/Json_utils.h>
#include <FD3D/Serialization/Json/FD3D_Json_utils.h>

#include <FDJson/JsonSerializer.h>

#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <FDCore/Binding/Python/FDCorePython.h>
#include <FD3D/Binding/Python/FD3DPython.h>

#include <FDPython/Value.h>

namespace py = pybind11;

//void rotate(FD3D::Behavior *bvr, FDCore::TimeManager<> &t)
//{
//    FD3D::ObjectNode *node = bvr->getNode().getNode().as<FD3D::ObjectNode>();
//    FD3D::Transform &tran = node->getEntity();
//    tran.setRotation(glm::vec3(0.0f, t.getElapsedTime() / -2000.0f, 0.0f));
//}

//PYBIND11_EMBEDDED_MODULE(test_module, m)
//{
//    m.def("rotate", &rotate);
//}


constexpr const char *py_script = R"(
import PyFD3D
import PyFDCore

def rotate(bvr, t):
    node = bvr.node.object
    node.entity.eulerAngles = PyFD3D.vec.vec3(0.0, t.elapsedTime / -2000.0, 0.0)


class TestClass:
    def __init__(self):
        self.message = "the message"

    def the_method(self):
        return "the method";
)";


Engine::Engine(FDGL::BaseOpenGLContext &ctx, FDGL::BaseOpenGLWindow &window, FDGL::BaseRenderer &renderer):
    FDEngine::BaseEngine(ctx, window, renderer)
{
}

Engine::~Engine()
{
    quit();
}

FDCore::ThreadPool &Engine::getThreadPool()
{
    return m_threadPool;
}

const FDCore::ThreadPool &Engine::getThreadPool() const
{
    return m_threadPool;
}

bool Engine::init(int, char **)
{

    if(!m_ctx.init())
        return false;

    if(!m_window.create(800, 600, "Test window"))
        return false;

    m_ctx.setCurrentContext(m_window);
    if (!m_ctx.loadOpenGLFunctions(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        return false;

    m_renderer.setScene(&m_scene);
    m_window.setRenderer(m_renderer);
    m_window.setInputStrategy(&Engine::processInput, this);
    m_window.init();

//    m_pyInterpreter.importModule("test_module");
    m_pyInterpreter.importModule("PyFD3D");
    m_pyInterpreter.importModule("PyFDCore");
    m_pyInterpreter.importModule("PythonBehavior");

//    py::dict d;
//    m_pyInterpreter.setVariable("locals", FDPython::Value(d));
//    m_pyInterpreter.exec(py_script, *m_pyInterpreter.getVariable("locals"));

    initScene();

    return true;
}

void Engine::quit()
{
    m_window.quit();
    m_window.destroy();
    m_ctx.quit();
}

void Engine::processInput(FDGL::BaseOpenGLWindow &)
{
    Window &w = static_cast<Window&>(m_window);
    if(glfwGetKey(w.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        stop();

    glfwPollEvents();
}

void Engine::update()
{
    pybind11::gil_scoped_release rel;
    std::vector<std::future<void>> taskResults;
    taskResults.reserve(m_behaviors.size());
    for(auto it = m_behaviors.begin(), end = m_behaviors.end(); it != end; ++it)
        taskResults.push_back(m_threadPool.enqueue(&Engine::updateTask, this, std::ref(it->value)));

    for(auto &result: taskResults)
        result.wait();

    pybind11::gil_scoped_acquire acq;
}

int Engine::run()
{
    while (isRunning())
    {
        m_window.processInput();
        update();
        m_window.render();
    }

    return 0;
}

bool Engine::isRunning() const
{
    return m_window.isOpen();
}

void Engine::stop()
{
    glfwSetWindowShouldClose(static_cast<Window&>(m_window).getWindow(), true);
}

void Engine::initScene()
{
    assert(loadScene("../../FDGL/test/resources/crate/CrateModel.obj"));
    std::vector<FD3D::CameraNode*> cams = m_scene.getNodesAs<FD3D::CameraNode>();
    if(cams.empty())
    {
        std::unique_ptr<FD3D::CameraNode> cam(new FD3D::CameraNode);
        cam->setName("default_camera");
        cam->getEntity().setPosition(glm::vec3(0.0f, 3.0f, 10.0f));
        cams.push_back(cam.get());
        m_scene.addNode(cam.release());
    }

    static_cast<Renderer&>(m_renderer).setActiveCamera(&cams.front()->getEntity());
    static_cast<Renderer&>(m_renderer).initProjection(800, 600);

    std::vector<FD3D::LightNode*> lights = m_scene.getNodesAs<FD3D::LightNode>();
    if(lights.empty())
    {
        std::function<void(FD3D::StrategyBehavior*)> updateLightFunc = [this](FD3D::StrategyBehavior *bvr){
            float elapsedTimeInSeconds = m_timeMgr.getElapsedTime() / 1000.0f;

            float radius = 3.0f;
            glm::vec3 &p = bvr->getNode()->as<FD3D::LightNode>()->getEntity().getPosition();
            p.x = std::sin(elapsedTimeInSeconds) * radius;
            p.z = std::cos(elapsedTimeInSeconds) * radius;
            p.y = std::sin(elapsedTimeInSeconds * 3) * radius;
        };

        std::unique_ptr<FD3D::LightNode> light(new FD3D::LightNode);
        light->setName("default_light");
        FD3D::Light &l = light->getEntity();
        l.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        l.setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
        l.setType(FD3D::LightType::DirectionalLight);
        l.setDirection(glm::vec3(1.0f, -1.0f, 0.0f));
        l.attenuation.setConstantAttenuation(5.0f);
        lights.push_back(light.get());

        std::unique_ptr<FD3D::StrategyBehavior> b(new FD3D::StrategyBehavior());
        b->setUpdateStrategy(updateLightFunc);
        b->setScene(&m_scene);
        b->setNodeId(light->getId());
        m_scene.addComponent(b.get());
        m_scene.bindComponent(light->getId(), b->getId());
        if(!m_behaviors.contains(light->getId()))
            m_behaviors.insert(light->getId(), {});

        m_behaviors[light->getId()]->push_back(b->getId());
        b.release();

        m_scene.addNode(light.release());
    }

    /*m_pyInterpreter.setVariable("test_module.rotate",
                                FDPython::Value(m_pyInterpreter.getVariable("test_module")->getObject<pybind11::module>().attr("rotate")));*/
    //m_pyInterpreter.setVariable("rotate", FDPython::Value(m_pyInterpreter.getVariable("locals")->getObject<pybind11::dict>()["rotate"]));

    static_cast<Renderer&>(m_renderer).setLight(&lights.front()->getEntity());

    std::vector<FDGL::BufferedMesh*> meshes = m_scene.getComponentsAs<FDGL::BufferedMesh>();
    /*std::function<void(FD3D::StrategyBehavior*)> updateFunc = [this](FD3D::StrategyBehavior *bvr){
        pybind11::gil_scoped_acquire acquire;
        this->m_pyInterpreter.getVariable("rotate")->getObject<pybind11::function>()(
            static_cast<FD3D::Behavior*>(bvr), m_timeMgr
        );
    };*/

    for(auto *m: meshes)
    {
        m->setVAOFunctionToDefault();
        auto v = m_scene.getBoundNodes(m->getId());
        for(auto &n: v)
        {
            auto f = py::globals()["PythonBehavior"].attr("PythonBehavior");
            py::print(f);
            py::object obj = f(&m_timeMgr, &m_scene, n->getId());
            py::print(obj);

            std::unique_ptr<FD3D::PythonBehaviorWrapper> b(new FD3D::PythonBehaviorWrapper(obj));

            //std::unique_ptr<FD3D::StrategyBehavior> b(new FD3D::StrategyBehavior());
            //b->setUpdateStrategy(updateFunc);
//            b->setScene(&m_scene);
//            b->setNodeId(n->getId());
            m_scene.addComponent(b.get());
            m_scene.bindComponent(n->getId(), b->getId());

            if(!m_behaviors.contains(n->getId()))
                m_behaviors.insert(n->getId(), {});

            m_behaviors[n->getId()]->push_back(b->getId());
            //b->update();
            b.release();
        }
    }
}

bool Engine::loadScene(const std::string &path)
{
    FDGL::BufferedMesh::setDefaultVAOFunction([](FDGL::BufferedMesh &mesh)
    {
        FDGL::OpenGLBufferWrapper vbo = mesh.getVBO();
        FDGL::OpenGLBufferWrapper ebo = mesh.getEBO();
        vbo.bind(FDGL::BufferTarget::VertexAttribute);
        ebo.bind(FDGL::BufferTarget::VertexIndex);
        size_t s = mesh.getStride() * sizeof(float);
        FDGL::setAttribFromBuffer<GL_FLOAT, 3, false>(0, s,
                                                      static_cast<size_t>(mesh.getComponentOffset(FD3D::VertexComponentType::Position)) * sizeof(float));
        FDGL::enableAttrib(0);
        FDGL::setAttribFromBuffer<GL_FLOAT, 3, false>(1, s,
                                                      static_cast<size_t>(mesh.getComponentOffset(FD3D::VertexComponentType::Normal)) * sizeof(float));
        FDGL::enableAttrib(1);
        FDGL::setAttribFromBuffer<GL_FLOAT, 2, false>(2, s,
                                                      static_cast<size_t>(mesh.getComponentOffset(FD3D::VertexComponentType::Texture)) * sizeof(float));
        FDGL::enableAttrib(2);
    });
    FD3D::SceneLoader loader;
    loader.setTextureLoader([](const std::string &path){
        return loadTexture(path);
    });
    loader.setEmbeddedTextureLoader([](const aiTexture *texture){
        return loadTexture(texture);
    });
    loader.setMeshAllocator([](){
        return new FDGL::BufferedMesh();
    });
    return loader.loadScene(m_scene, path, aiProcess_Triangulate);
}

void Engine::updateTask(std::vector<FD3D::Component::id_type> &behaviorIds)
{
    for(auto id: behaviorIds)
    {
        FD3D::Component *comp = m_scene.getComponent(id);
        if(comp)
        {
            FD3D::Behavior *bvr = comp->as<FD3D::Behavior>();
            if(bvr)
                bvr->update();
        }
    }
}
