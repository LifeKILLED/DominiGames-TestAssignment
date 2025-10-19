#include "MeshRenderer.h"
#include "Mesh.h"
#include "Shader.h"

namespace Renderer
{
    void MeshRenderer::Draw(const glm::mat4& transform)
    {
        if (!m_shader || !m_mesh) return;
        m_shader->Bind();
        m_shader->SetTransform(transform);
        m_mesh->Draw();
    }
}
