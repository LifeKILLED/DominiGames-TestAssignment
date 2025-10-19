#pragma once

#include "Renderer/Mesh.h"

#include <GLES2/gl2.h>

namespace Renderer {

    class MeshGLES : public Mesh {
    public:
        MeshGLES();
        ~MeshGLES() override = default;

        void Load() override;
        void Destroy() override;
        void Draw() override;

    private:
        GLuint m_vbo;
    };

} // namespace Renderer
