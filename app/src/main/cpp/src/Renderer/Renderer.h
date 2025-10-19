#pragma once

#include "Renderer/Context.h"
#include "Util/Singleton.h"

#include <memory>
#include <unordered_set>

namespace Renderer {

    class Resource;
    class Mesh;
    class Shader;

    class Renderer : public Singleton<Renderer> {
    public:
        void createContext(void* windowData);
        void destroyContext();
        const std::unique_ptr<Context>& getContext() const { return m_context; };

        void beginFrame();
        void endFrame();

    public:
        std::shared_ptr<Mesh> createMesh();
        std::shared_ptr<Shader> createShader(const std::string& vertSrc, const std::string& fragSrc);
        void destroy(std::shared_ptr<Resource> ptr);

    private:
        friend class Singleton<Renderer>;
        Renderer() = default;

        std::unique_ptr<Context> m_context;

        std::unordered_set<std::shared_ptr<Resource>> m_resources;
    };

} // namespace Renderer
