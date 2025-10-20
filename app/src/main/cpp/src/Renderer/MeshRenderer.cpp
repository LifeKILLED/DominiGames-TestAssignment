#include "MeshRenderer.h"
#include "Mesh.h"
#include "Shader.h"

namespace Renderer
{
    void MeshRenderer::Draw(const glm::mat4& transform, const glm::mat4& projection)
    {
        if (!m_shader || !m_mesh) return;

        m_shader->Bind();
        m_shader->SetTransform(transform);
        m_shader->SetProjection(projection);

        m_mesh->Draw();
    }

    void MeshRenderer::SetShader(std::shared_ptr<Shader> shader)
    {
        m_shader = shader;
    }

    std::shared_ptr<Shader> MeshRenderer::GetShader()
    {
        return m_shader;
    }

    void MeshRenderer::SetMesh(std::shared_ptr<Mesh> mesh)
    {
        m_mesh = mesh;
    }

}
