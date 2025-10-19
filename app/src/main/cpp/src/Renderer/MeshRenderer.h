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
        void Draw(const glm::mat4& transform, const glm::mat4& projection);

        void SetShader(std::shared_ptr<Shader> shader);
        std::shared_ptr<Shader> GetShader();

        void SetMesh(std::shared_ptr<Mesh> mesh);

    private:
        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<Mesh> m_mesh;
    };
}
