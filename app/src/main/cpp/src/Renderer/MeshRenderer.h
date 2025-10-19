#pragma once
#include <memory>
#include <glm/glm.hpp>

namespace Renderer
{
    class Shader;
    class Mesh;

    class MeshRenderer
    {
    public:
        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<Mesh> m_mesh;

        void Draw(const glm::mat4& transform);
    };
}
