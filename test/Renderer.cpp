#include "Renderer.h"

#include <FDGL/BufferedMesh.h>
#include <FDGL/ShaderComponent.h>

#include <FD3D/SceneGraph/SceneLoader.h>

#include <GLFW/glfw3.h>

#include "GLUtils.h"

Renderer::Renderer(FDGL::BaseOpenGLContext &ctx):
    FDGL::BaseRenderer(ctx),
    m_activeCamera(nullptr)
{}

void Renderer::renderScene()
{
    std::stack<FD3D::SceneNode::id_type> todo;
    std::unordered_map<FD3D::SceneNode::id_type, NodeState> nodeStates;
    todo.push(m_scene->getRootId());
    while(!todo.empty())
    {
        if(nodeStates.find(todo.top()) == nodeStates.end())
            nodeStates[todo.top()] = NodeState::New;

        NodeTransitionGuard current(*this, todo.top(), nodeStates[todo.top()]);
        switch(current.getState())
        {
            case NodeState::New:
                addNodeChildren(current.getId(), todo);
            break;

            case NodeState::Explored:
                renderNode(m_scene->getNode(current.getId()));
            break;

            case NodeState::Rendered:
            case NodeState::Done:
            case NodeState::Invalid:
                todo.pop();
            break;
        }
    }
}

void Renderer::init(FDGL::BaseOpenGLWindow &w)
{
    initContext();
    initWindow(w);
}

void Renderer::quit(FDGL::BaseOpenGLWindow &)
{

}

void Renderer::render(FDGL::BaseOpenGLWindow &w)
{
    w.clear();

    drawLight();

    renderScene();

    w.swapBuffer();

    //rotateLight(m_timeMgr.getElapsedTime() / 1000.0f);
}

void Renderer::resize(FDGL::BaseOpenGLWindow &, int width, int height)
{
    getActiveCamera()->projection.setWidth(width);
    getActiveCamera()->projection.setHeight(height);
}

void Renderer::errorHandler(FDGL::ErrorSoure source, FDGL::ErrorType type, uint32_t id, FDGL::ErrorSeverity level, const std::string &msg) const
{
    constexpr const char *format = "GL_DEBUG_MESSAGE:"
                                   "\n{"
                                   "\n    source: %s,"
                                   "\n    type: %s,"
                                   "\n    severity: %s,"
                                   "\n    id: %u"
                                   "\n    message: %s"
                                   "\n}\n";
    fprintf(stderr, format,
            FDGL::errorSourceToString(source).c_str(),
            FDGL::errorTypeToString(type).c_str(),
            FDGL::errorSeverityToString(level).c_str(),
            id, msg.c_str());
}

void Renderer::initContext()
{
    m_ctx.enableDepth();
    m_ctx.enableFaceCulling();
    m_ctx.cullFace(FDGL::CullFace::Front);
    m_ctx.enableDebugOutut();
    glDepthFunc(GL_LESS);
    glDebugMessageCallback(&Renderer::debugCallbackHelper, this);

    FDGL::OpenGLVertexArrayWrapper vao;
    vao.create();
    vao.bind();

    m_ctx.addRessource("default_mesh_shader", createShaderProgram());
    m_ctx.addRessource("default_light_shader", createLightShaderProgram());
    m_ctx.addRessource("attribute_less_vao", vao);
}

void Renderer::initWindow(FDGL::BaseOpenGLWindow &w)
{
    w.setClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
    w.setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::initScene()
{
    assert(loadScene("../../FDGL/test/resources/crate/CrateModel.obj"));
    std::vector<FD3D::CameraNode*> cams = m_scene->getNodesAs<FD3D::CameraNode>();
    if(cams.empty())
    {
        std::unique_ptr<FD3D::CameraNode> cam(new FD3D::CameraNode);
        cam->setName("default_camera");
        cam->getEntity().setPosition(glm::vec3(0.0f, 3.0f, 10.0f));
        cams.push_back(cam.get());
        m_scene->addNode(cam.release());
    }

    m_activeCamera = &cams.front()->getEntity();

    std::vector<FD3D::LightNode*> lights = m_scene->getNodesAs<FD3D::LightNode>();
    if(lights.empty())
    {
        std::unique_ptr<FD3D::LightNode> light(new FD3D::LightNode);
        light->setName("default_light");
        FD3D::Light &l = light->getEntity();
        l.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        l.setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
        l.setType(FD3D::LightType::DirectionalLight);
        l.setDirection(glm::vec3(1.0f, -1.0f, 0.0f));
        l.attenuation.setConstantAttenuation(5.0f);
        lights.push_back(light.get());
        m_scene->addNode(light.release());
    }

    m_light = &lights.front()->getEntity();

    std::vector<FDGL::BufferedMesh*> meshes = m_scene->getComponentsAs<FDGL::BufferedMesh>();
    for(auto *m: meshes)
        m->setVAOFunctionToDefault();
}

void Renderer::initProjection(int width, int height)
{
    FD3D::Projection *proj = getProjection();
    if(proj == nullptr)
        return;

    proj->setFov(glm::radians(45.0f));
    proj->setWidth(width);
    proj->setHeight(height);
    proj->setNear(0.1f);
    proj->setFar(100.0f);
    proj->setType(FD3D::ProjectionType::Perspective);
}

bool Renderer::loadScene(const std::string &path)
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
    return loader.loadScene(*m_scene, path, aiProcess_Triangulate);
}

void Renderer::renderNode(FD3D::SceneNodeProxy node)
{
    if(!node)
        return;

    std::vector<FDGL::BufferedMesh*> meshes = node.getComponentsAs<FDGL::BufferedMesh>();
    if(meshes.empty())
        return;

    for(size_t i = 0, imax = meshes.size(); i < imax; ++i)
    {
        renderMesh(meshes[i]);
    }
}

void Renderer::addNodeChildren(FD3D::SceneNode::id_type id,
                               std::stack<FD3D::SceneNode::id_type> &todo)
{
    FD3D::SceneNodeProxy node = m_scene->getNode(id);
    std::vector<FD3D::SceneNode::id_type> toAdd = node->getChildIds();
    for(size_t i = 0, imax = toAdd.size(); i < imax; ++i)
        todo.push(toAdd[i]);
}

void Renderer::drawLight()
{
    FDGL::OpenGLShaderProgramWrapper program = FDGL::as<FDGL::OpenGLShaderProgramWrapper>(m_ctx.getRessource("default_light_shader"));
    FDGL::OpenGLVertexArrayWrapper vao = FDGL::as<FDGL::OpenGLVertexArrayWrapper>(m_ctx.getRessource("attribute_less_vao"));
    program.bind();
    program.setUniform("lightPosition", m_light->getPosition());
    program.setUniform(1, getActiveCamera()->getMatrix());
    program.setUniform(2, getProjection()->getMatrix());
    program.setUniform("scale", 0.5f);
    program.setUniform("lightColor", m_light->color.diffuse);
    vao.bind();
    FDGL::drawArrays(FDGL::DrawMode::Triangles, 0, 36);
}

void Renderer::rotateCamera(float elapsedTime)
{
    float radius = 10.0f;
    float camX = std::sin(elapsedTime) * radius;
    float camZ = std::cos(elapsedTime) * radius;
    getActiveCamera()->setPosition(glm::vec3(camX, 0.0f, camZ));
    getActiveCamera()->setRotation(glm::vec3(0.0f, elapsedTime, 0.0f));
}

void Renderer::rotateLight(float elapsedTime)
{
    float radius = 3.0f;
    glm::vec3 &p = m_light->getPosition();
    p.x = std::sin(elapsedTime) * radius;
    p.z = std::cos(elapsedTime) * radius;
}

void Renderer::renderMesh(FDGL::BufferedMesh *mesh)
{
    FDGL::OpenGLShaderProgramWrapper program;
    FDGL::ShaderComponent *shadComp = nullptr;
    if(mesh->hasShader())
    {
        FD3D::Component *comp = m_scene->getComponent(mesh->getShaderId());
        if(!comp)
            shadComp = comp->as<FDGL::ShaderComponent>();
    }

    if(shadComp)
        program = shadComp->getShaderProgram();
    else
        program = m_ctx.getRessource("default_mesh_shader");

    FD3D::Material *mat = nullptr;
    if(mesh->hasMaterial())
    {

        FD3D::Component *comp = m_scene->getComponent(mesh->getMaterialId());
        if(comp)
            mat = comp->as<FD3D::Material>();
    }

    program.bind();
    program.setUniform("texture", 0);
    program.setUniform(0, m_transformStack.getCurrentMatrix());
    program.setUniform(1, getActiveCamera()->getMatrix());
    program.setUniform(2, getProjection()->getMatrix());
    program.setUniform(3, m_activeCamera->getPosition());
    program.setUniform(4, m_light->getPosition());
    program.setUniform(5, m_light->color.ambient);
    program.setUniform(6, m_light->color.diffuse);
    program.setUniform(7, m_light->color.specular);
    program.setUniform(8, m_light->attenuation.getConstantAttenuation());
    program.setUniform(9, m_light->attenuation.getLinearAttenuation());
    program.setUniform(10, m_light->attenuation.getQuadraticAttenuation());

    program.setUniform(11, mat->getAmbientColor());
    program.setUniform(12, mat->getDiffuseColor());
    program.setUniform(13, mat->getSpecularColor());
    program.setUniform(14, mat->getShininess());

    FDGL::OpenGLTextureWrapper tex(mat->getTextures()[FD3D::TextureType::Ambient][0]);

    tex.activateTexture(0);
    tex.bind(FDGL::TextureTarget::Texture2D);

    mesh->getVAO().bind();
    FDGL::drawElements<uint32_t>(FDGL::DrawMode::Triangles, mesh->getNumberOfIndices(), nullptr);
}

Renderer::NodeTransitionGuard::NodeTransitionGuard(Renderer &r, FD3D::SceneNode::id_type id, Renderer::NodeState &state):
    m_renderer(r),
    m_id(id),
    m_state(state)
{}

Renderer::NodeTransitionGuard::~NodeTransitionGuard()
{
    applyTransition();
}

FD3D::SceneNode::id_type Renderer::NodeTransitionGuard::getId()
{
    return m_id;
}

Renderer::NodeState Renderer::NodeTransitionGuard::getState() const
{
    return m_state;
}

void Renderer::NodeTransitionGuard::applyTransition()
{
    switch (m_state)
    {
        case NodeState::New:
        {
            FD3D::ObjectNode *obj = m_renderer.getScene()->getNode(m_id)->as<FD3D::ObjectNode>();
            if(obj != nullptr)
                m_renderer.pushTransform(obj->getEntity());

            m_state = NodeState::Explored;
        }
        break;

        case NodeState::Explored:
        m_state =  NodeState::Rendered;
        break;

        case NodeState::Rendered:
        {
            FD3D::ObjectNode *obj = m_renderer.getScene()->getNode(m_id)->as<FD3D::ObjectNode>();
            if(obj != nullptr)
                m_renderer.popTransform();

            m_state = NodeState::Done;
        }
        break;

        default:
        break;
    }
}
