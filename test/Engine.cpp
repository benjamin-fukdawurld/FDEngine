#include "Engine.h"

#include <GLFW/glfw3.h>

#include <FDGL/BufferedMeshComponent.h>
#include <FDGL/OpenGLVertexArray.h>
#include "../FDGL/test/Renderer.h"
#include "../FDGL/test/GLUtils.h"
#include "../FDGL/test/GLFWImpl.h"

#include <FD3D/SceneGraph/SceneLoader.h>
#include <FD3D/Behavior/StrategyBehaviorComponent.h>

#include <FDCore/Binding/Python/FDCorePython.h>
#include <FD3D/Binding/Python/FD3DPython.h>

#include <FDPython/Value.h>

#include <FD3D/Serialization/Json/FD3DJson_fwd.h>
#include <FDJson/JsonSerializer.h>
#include <FD3D/Serialization/Json/FD3DJson.h>

namespace py = pybind11;

template<typename T>
std::enable_if_t<std::is_base_of_v<FD3D::Element, T>, void> serialize_element(FD3D::SceneManager*, FD3D::Element *i, void *user_data)
{
    auto data = reinterpret_cast<std::pair<FDJson::Serializer*, FDJson::Serializer::Value>*>(user_data);
    data->second = data->first->serialize(static_cast<T&>(*i));
}

template<typename T, typename SerializedType = T>
std::enable_if_t<std::is_base_of_v<FD3D::Component, T>, void> unserialize_component(FD3D::SceneManager* mgr, FD3D::Element *, void *user_data)
{
    auto data = reinterpret_cast<std::tuple<FDJson::Serializer*, const FDJson::Serializer::Value&, FD3D::Element **, bool, std::string*>*>(user_data);
    std::unique_ptr<SerializedType> m(new T());
    std::get<3>(*data) = std::get<0>(*data)->unserialize(std::get<1>(*data), *m, std::get<4>(*data));
    if(!std::get<3>(*data))
        return;

    *std::get<2>(*data) = m.get();
    mgr->getScene()->addComponent(m.release());
}

template<typename T, typename SerializedType = T>
std::enable_if_t<std::is_base_of_v<FD3D::SceneNode, T>, void> unserialize_node(FD3D::SceneManager* mgr, FD3D::Element *, void *user_data)
{
    auto data = reinterpret_cast<std::tuple<FDJson::Serializer*, const FDJson::Serializer::Value&, FD3D::Element **, bool, std::string*>*>(user_data);
    std::unique_ptr<SerializedType> m(new T());
    std::get<3>(*data) = std::get<0>(*data)->unserialize(std::get<1>(*data), *m, std::get<4>(*data));
    if(!std::get<3>(*data))
        return;

    *std::get<2>(*data) = m.get();
    mgr->getScene()->addNode(m.release());
}

template<>
void unserialize_node<FD3D::RootNode, FD3D::RootNode>(FD3D::SceneManager* mgr, FD3D::Element *, void *user_data)
{
    auto data = reinterpret_cast<std::tuple<FDJson::Serializer*, const FDJson::Serializer::Value&, FD3D::Element **, bool, std::string*>*>(user_data);
    std::get<3>(*data) = std::get<0>(*data)->unserialize(std::get<1>(*data), mgr->getScene()->getRootNode().getNode(), std::get<4>(*data));
    if(!std::get<3>(*data))
        return;

    *std::get<2>(*data) = mgr->getScene()->getRootNode().getNodeAs<FD3D::SceneNode>();
}


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

    m_pyInterpreter.importModule("PyFD3D");
    m_pyInterpreter.importModule("PyFDCore");
    m_pyInterpreter.importModule("PythonBehavior");

    initSceneManager();
    initScene();

    return true;
}

void Engine::quit()
{
    for(auto &v: m_behaviors)
    {
        for(auto bvrId: v.value)
            m_scene.removeComponent(bvrId);
    }

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

void Engine::initSceneManager()
{
    BaseEngine::initSceneManager();
    initSerialiseHandler();
    initUnserialiseHandler();
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
        std::unique_ptr<FD3D::CameraNode> node(new FD3D::CameraNode);
        std::unique_ptr<FD3D::Camera> cam(new FD3D::Camera);
        node->setEntity(cam.get());
        node->setName("default_camera");
        cam->setPosition(glm::vec3(0.0f, 3.0f, 10.0f));
        cams.push_back(node.get());

        m_scene.addComponent(cam.release());
        m_scene.addNode(node.release());
    }

    static_cast<Renderer&>(m_renderer).setActiveCamera(cams.front()->getEntity());
    static_cast<Renderer&>(m_renderer).initProjection(800, 600);

    std::vector<FD3D::LightNode*> lights = m_scene.getNodesAs<FD3D::LightNode>();
    if(lights.empty())
    {
        std::function<void(FD3D::StrategyBehaviorComponent*)> updateLightFunc = [this](FD3D::StrategyBehaviorComponent *bvr){
            float elapsedTimeInSeconds = m_timeMgr.getElapsedTime() / 1000.0f;

            float radius = 3.0f;
            glm::vec3 &p = bvr->getNode()->as<FD3D::LightNode>()->getEntity()->getPosition();
            p.x = std::sin(elapsedTimeInSeconds) * radius;
            p.z = std::cos(elapsedTimeInSeconds) * radius;
            p.y = std::sin(elapsedTimeInSeconds * 3) * radius;
        };

        {
            std::unique_ptr<FD3D::LightNode> node(new FD3D::LightNode);
            std::unique_ptr<FD3D::Light> light(new FD3D::Light);
            node->setEntity(light.get());
            node->setName("default_light");
            light->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
            light->setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
            light->setType(FD3D::LightType::PointLight);
            light->setDirection(glm::vec3(0.0f, -1.0f, 0.0f));
            light->attenuation.setConstantAttenuation(5.0f);
            lights.push_back(node.get());

            std::unique_ptr<FD3D::StrategyBehaviorComponent> b(new FD3D::StrategyBehaviorComponent());
            b->setUpdateStrategy(updateLightFunc);
            b->setScene(&m_scene);
            b->setNodeId(node->getId());
            m_scene.addComponent(b.get());
            m_scene.bindComponent(node->getId(), b->getId());
            if(!m_behaviors.contains(node->getId()))
                m_behaviors.insert(node->getId(), {});

            m_behaviors[node->getId()]->push_back(b->getId());
            b.release();

            m_scene.addComponent(light.release());
            m_scene.addNode(node.release());
        }


        updateLightFunc = [this](FD3D::StrategyBehaviorComponent *bvr){
            float elapsedTimeInSeconds = m_timeMgr.getElapsedTime() / 1000.0f;

            float radius = 5.0f;
            glm::vec3 &p = bvr->getNode()->as<FD3D::LightNode>()->getEntity()->getPosition();
            p.x = std::sin(elapsedTimeInSeconds) * radius;
            p.z = std::cos(elapsedTimeInSeconds) * radius;
        };

        {
            std::unique_ptr<FD3D::LightNode> node(new FD3D::LightNode);
            std::unique_ptr<FD3D::Light> light(new FD3D::Light);
            node->setEntity(light.get());
            node->setName("spot_light");
            light->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
            light->setDirection(glm::vec3(0.0f, -1.0f, 0.0f));
            light->setType(FD3D::LightType::SpotLight);
            light->setDirection(glm::vec3(1.0f, -1.0f, 0.0f));
            light->attenuation.setConstantAttenuation(5.0f);
            lights.push_back(node.get());

            std::unique_ptr<FD3D::StrategyBehaviorComponent> b(new FD3D::StrategyBehaviorComponent());
            b->setUpdateStrategy(updateLightFunc);
            b->setScene(&m_scene);
            b->setNodeId(node->getId());
            m_scene.addComponent(b.get());
            m_scene.bindComponent(node->getId(), b->getId());
            if(!m_behaviors.contains(node->getId()))
                m_behaviors.insert(node->getId(), {});

            m_behaviors[node->getId()]->push_back(b->getId());
            b.release();

            m_scene.addComponent(light.release());
            m_scene.addNode(node.release());
        }
    }

    static_cast<Renderer&>(m_renderer).setLight(lights.front()->getEntity());

    std::vector<FDGL::BufferedMeshComponent*> meshes = m_scene.getComponentsAs<FDGL::BufferedMeshComponent>();
    FDJson::Serializer s;
    for(auto *m: meshes)
    {
        m->setVAOFunctionToDefault();
        auto v = m_scene.getBoundNodes(m->getId());
        for(auto &n: v)
        {
            auto f = py::globals()["PythonBehavior"].attr("PythonBehavior");
            std::unique_ptr<FD3D::PythonBehaviorWrapper> b(new FD3D::PythonBehaviorWrapper(f(&m_timeMgr, &m_scene, n->getId())));
            m_scene.addComponent(b.get());
            m_scene.bindComponent(n->getId(), b->getId());

            if(!m_behaviors.contains(n->getId()))
                m_behaviors.insert(n->getId(), {});

            m_behaviors[n->getId()]->push_back(b->getId());
            b.release();
        }
    }

    m_sceneManager.prepareIdIndexMap();
    s.setUserData(&m_sceneManager);
    s.save(s.serialize(m_scene), "scene.json");
    m_sceneManager.clearIdIndexMap();

    {
        m_sceneManager.clearIdIndexMap();
        FD3D::Scene scene2;
        std::string err;
        if(!s.parseFile("scene.json", &err))
            std::cerr << err << std::endl;

        m_sceneManager.clearIdIndexMap();
        m_sceneManager.setScene(&scene2);
        const rapidjson::Value &val = s.getCurrentDocument();
        if(!s.unserialize(val, scene2, &err))
            std::cerr << err << std::endl;

        m_sceneManager.clearIdIndexMap();
        m_sceneManager.prepareIdIndexMap();
        s.save(s.serialize(scene2), "scene2.json");
        m_sceneManager.clearIdIndexMap();
        m_sceneManager.setScene(&m_scene);
    }
}

bool Engine::loadScene(const std::string &path)
{
    FDGL::BufferedMeshComponent::setDefaultVAOFunction([](FDGL::BufferedMeshComponent &mesh)
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
        return loadTexture(path).getId();
    });
    loader.setEmbeddedTextureLoader([](const aiTexture *texture){
        return loadTexture(texture).getId();
    });
    loader.setMeshAllocator([](){
        return new FDGL::BufferedMeshComponent();
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
            FD3D::AbstractBehaviorComponent *bvr = comp->as<FD3D::AbstractBehaviorComponent>();
            if(bvr)
                bvr->update();
        }
    }
}

void Engine::initSerialiseHandler()
{
    m_sceneManager.registerEvent("json-serialize");
    auto &handlers = m_sceneManager["json-serialize"];
    handlers.insert(FDCore::TypeCodeHelper<FD3D::AbstractMeshComponent>::code, &serialize_element<FD3D::AbstractMeshComponent>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::MeshComponent>::code, &serialize_element<FD3D::MeshComponent>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::MaterialComponent>::code, &serialize_element<FD3D::MaterialComponent>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::TextureComponent>::code, &serialize_element<FD3D::TextureComponent>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::Camera>::code, &serialize_element<FD3D::Camera>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::Transform>::code, &serialize_element<FD3D::Transform>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::Light>::code, &serialize_element<FD3D::Light>);
    handlers.insert(FDCore::TypeCodeHelper<FDGL::BufferedMeshComponent>::code, &serialize_element<FD3D::AbstractMeshComponent>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::ObjectNode>::code, &serialize_element<FD3D::ObjectNode>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::LightNode>::code, &serialize_element<FD3D::LightNode>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::CameraNode>::code, &serialize_element<FD3D::CameraNode>);
}

void Engine::initUnserialiseHandler()
{
    m_sceneManager.registerEvent("json-unserialize");
    auto &handlers = m_sceneManager["json-unserialize"];
    handlers.insert(FDCore::TypeCodeHelper<FD3D::MeshComponent>::code, &unserialize_component<FD3D::MeshComponent>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::MaterialComponent>::code, &unserialize_component<FD3D::MaterialComponent>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::TextureComponent>::code, &unserialize_component<FD3D::TextureComponent>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::Camera>::code, &unserialize_component<FD3D::Camera>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::Transform>::code, &unserialize_component<FD3D::Transform>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::Light>::code, &unserialize_component<FD3D::Light>);
    handlers.insert(FDCore::TypeCodeHelper<FDGL::BufferedMeshComponent>::code, &unserialize_component<FDGL::BufferedMeshComponent, FD3D::AbstractMeshComponent>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::StrategyBehaviorComponent>::code, &unserialize_component<FD3D::StrategyBehaviorComponent, FD3D::Component>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::PythonBehaviorWrapper>::code, &unserialize_component<FD3D::PythonBehaviorWrapper, FD3D::Component>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::SceneNode>::code, &unserialize_node<FD3D::SceneNode>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::RootNode>::code, &unserialize_node<FD3D::RootNode>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::ObjectNode>::code, &unserialize_node<FD3D::ObjectNode>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::LightNode>::code, &unserialize_node<FD3D::LightNode>);
    handlers.insert(FDCore::TypeCodeHelper<FD3D::CameraNode>::code, &unserialize_node<FD3D::CameraNode>);
}
