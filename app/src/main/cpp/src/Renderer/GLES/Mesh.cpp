#include "Renderer/Mesh.h"
#include <GLES2/gl2.h>

namespace Renderer {

    Mesh::~Mesh() {
        Unload();
    }

    void Mesh::SetData(const MeshData& data) {
        m_meshData = data;
        BuildVertexCache();
        m_isDirty = true;
    }

    void Mesh::BuildVertexCache() {
        m_vertexCache.clear();
        m_vertexAttributes.clear();
        m_stride = 0;

        for (auto& attr : m_meshData.attributes) {
            m_vertexAttributes.push_back(attr.size);
            m_stride += attr.size;
        }

        m_vertexCache.reserve(m_meshData.vertices.size() * m_stride);

        for (const auto& v : m_meshData.vertices) {
            for (auto& attr : m_meshData.attributes) {
                switch (attr.type) {
                    case MeshAttribute::Position:
                        for (int i = 0; i < attr.size; ++i)
                            m_vertexCache.push_back(v.position[i]);
                        break;
                    case MeshAttribute::Normal:
                        for (int i = 0; i < attr.size; ++i)
                            m_vertexCache.push_back(v.normal[i]);
                        break;
                    case MeshAttribute::UV:
                        for (int i = 0; i < attr.size; ++i)
                            m_vertexCache.push_back(v.uv[i]);
                        break;
                    case MeshAttribute::Color:
                        for (int i = 0; i < attr.size; ++i)
                            m_vertexCache.push_back(v.color[i]);
                        break;
                }
            }
        }
    }

    void Mesh::Load() {
        if (!m_vbo.has_value()) {
            GLuint vbo {};
            glGenBuffers(1, &vbo);
            m_vbo = vbo;
        }

        if (!m_ibo.has_value()) {
            GLuint ibo {};
            glGenBuffers(1, &ibo);
            m_ibo = ibo;
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
        glBufferData(GL_ARRAY_BUFFER,
                     m_vertexCache.size() * sizeof(float),
                     m_vertexCache.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo.value());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     m_meshData.indices.size() * sizeof(unsigned int),
                     m_meshData.indices.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        m_isDirty = false;
    }

    void Mesh::Unload() {
        if (m_vbo.has_value()) {
            glDeleteBuffers(1, &m_vbo.value());
            m_vbo.reset();
        }
        if (m_ibo.has_value()) {
            glDeleteBuffers(1, &m_ibo.value());
            m_ibo.reset();
        }
    }

    void Mesh::Draw() {
        if (m_isDirty)
            Load();

        if (!m_vbo.has_value() || !m_ibo.has_value())
            return;

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo.value());

        int strideBytes = m_stride * sizeof(float);
        int offset = 0;

        for (GLuint i = 0; i < m_vertexAttributes.size(); ++i) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(
                    i,
                    m_vertexAttributes[i],
                    GL_FLOAT,
                    GL_FALSE,
                    strideBytes,
                    reinterpret_cast<void*>(offset * sizeof(float))
            );
            offset += m_vertexAttributes[i];
        }

        glDrawElements(GL_TRIANGLES, m_meshData.indices.size(), GL_UNSIGNED_INT, 0);

        for (GLuint i = 0; i < m_vertexAttributes.size(); ++i)
            glDisableVertexAttribArray(i);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

} // namespace Renderer
