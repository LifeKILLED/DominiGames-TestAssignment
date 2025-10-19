#include "Renderer/Mesh.h"

#include <GLES2/gl2.h>

namespace Renderer {
    Mesh::~Mesh() {
        Unload();
    }

    void Mesh::Load() {
        if (!m_id.has_value()) {
            GLuint buffer {};
            glGenBuffers(1, &buffer);
            m_id = buffer;
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_id.value());
        glBufferData(GL_ARRAY_BUFFER, m_vertexCache.size() * sizeof(float),
                     m_vertexCache.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_isDirty = false;
    }

    void Mesh::Unload() {
        if (m_id.has_value()) {
            glDeleteBuffers(1, &m_id.value());
            m_id.reset();
        }
    }

    void Mesh::Draw() {
        if (m_isDirty)
            Load();

        if (!m_id.has_value())
            return;

        glBindBuffer(GL_ARRAY_BUFFER, m_id.value());

        int stride = 0;
        for (int size: m_vertexAttributes)
            stride += size;
        stride *= sizeof(float);

        int offset = 0;
        for (GLuint i = 0; i < m_vertexAttributes.size(); ++i) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(
                    i,
                    m_vertexAttributes[i],
                    GL_FLOAT,
                    GL_FALSE,
                    stride,
                    reinterpret_cast<void *>(offset * sizeof(float))
            );
            offset += m_vertexAttributes[i];
        }

        int vertexCount = m_vertexCache.size() / (stride / sizeof(float));
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        for (GLuint i = 0; i < m_vertexAttributes.size(); ++i)
            glDisableVertexAttribArray(i);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

} // namespace Renderer
