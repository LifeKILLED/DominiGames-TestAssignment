#include "Renderer/Renderer.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Scene/Scene.h"

#if DCONTEXT_GLES
    #include "Renderer/GLES/ContextGLES.h"
    #define CONTEXT_TYPE ContextGLES
#else
    #error need to choose graphic api in CMakeLists.txt by defintion DCONTEXT_GLES
#endif

namespace Renderer {

    void Renderer::beginFrame() {
        if (m_context && m_context->isInitialized())
            m_context->beginFrame();
    }

    void Renderer::endFrame() {
        if (m_context && m_context->isInitialized())
            m_context->endFrame();
    }

    void Renderer::destroyContext() {
        if (!m_context)
            return;

        m_context->onPause();

        if (m_context->destroyResourcesWithContext()) {
            for (auto resource : m_resources) {
                resource->Unload();
            }
        }
    }

    void Renderer::createContext(void* window) {
        if (!m_context)
            m_context = std::make_unique<CONTEXT_TYPE>();

        m_context->onResume(window);

        if (m_context->destroyResourcesWithContext()) {
            for (auto resource : m_resources) {
                resource->Load();
            }
        }
    }

    std::shared_ptr<Mesh> Renderer::createMesh()
    {
        auto mesh = std::make_shared<Mesh>();
        m_resources.emplace(std::static_pointer_cast<Resource>(mesh));
        return mesh;
    }

    std::shared_ptr<Shader> Renderer::createShader(const std::string& vertSrc, const std::string& fragSrc)
    {
        auto shader = std::make_shared<Shader>(vertSrc, fragSrc);
        m_resources.emplace(std::static_pointer_cast<Resource>(shader));
        return shader;
    }

    void Renderer::destroy(std::shared_ptr<Resource> ptr)
    {
        auto resource = m_resources.find(ptr);
        if (resource != m_resources.end())
            m_resources.erase(resource);
    }

} // namespace Renderer
