#include "MeshGLES.h"

namespace Renderer {

    MeshGLES::MeshGLES() : m_vbo(0) {}

    void MeshGLES::Load() {
        if (m_vbo == 0) {
            glGenBuffers(1, &m_vbo);
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_vertexCache.size() * sizeof(float),
                     m_vertexCache.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_isDirty = false;
    }

    void MeshGLES::Destroy() {
        if (m_vbo != 0) {
            glDeleteBuffers(1, &m_vbo);
            m_vbo = 0;
        }
    }

    void MeshGLES::Draw() {
        if (m_isDirty) {
            Load();
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        int stride = 0;
        for (int size : m_vertexAttributes) stride += size;
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
                    (void*)(offset * sizeof(float))
            );
            offset += m_vertexAttributes[i];
        }

        glDrawArrays(GL_TRIANGLES, 0, m_vertexCache.size() / (stride / sizeof(float)));

        for (GLuint i = 0; i < m_vertexAttributes.size(); ++i) {
            glDisableVertexAttribArray(i);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

} // namespace Renderer
