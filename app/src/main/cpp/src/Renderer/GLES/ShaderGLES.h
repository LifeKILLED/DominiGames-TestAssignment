#pragma once

#include "Renderer/Shader.h"

#include <GLES2/gl2.h>

namespace Renderer {

    class ShaderGLES : public Shader {
    public:
        ShaderGLES(const std::string& vertSrc, const std::string& fragSrc);
        ~ShaderGLES() override = default;

        void Load() override;
        void Destroy() override;
        void Bind() override;

    private:
        GLuint m_program;
    };

} // namespace Renderer
