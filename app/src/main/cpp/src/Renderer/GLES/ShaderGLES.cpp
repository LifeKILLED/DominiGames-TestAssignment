#include "ShaderGLES.h"

#include <iostream>

namespace Renderer {

    ShaderGLES::ShaderGLES(const std::string& vertSrc, const std::string& fragSrc)
            : Shader(vertSrc, fragSrc), m_program(0)
    {}

    void ShaderGLES::Load() {
        GLuint vert = glCreateShader(GL_VERTEX_SHADER);
        const char* vSrc = m_vertSrc.c_str();
        glShaderSource(vert, 1, &vSrc, nullptr);
        glCompileShader(vert);

        GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fSrc = m_fragSrc.c_str();
        glShaderSource(frag, 1, &fSrc, nullptr);
        glCompileShader(frag);

        m_program = glCreateProgram();
        glAttachShader(m_program, vert);
        glAttachShader(m_program, frag);
        glLinkProgram(m_program);

        glDeleteShader(vert);
        glDeleteShader(frag);

        GLint success;
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (success != GL_TRUE) {
            char buffer[512];
            glGetProgramInfoLog(m_program, 512, nullptr, buffer);
            std::cerr << "Shader link error: " << buffer << std::endl;
        }
    }

    void ShaderGLES::Destroy() {
        if (m_program != 0) {
            glDeleteProgram(m_program);
            m_program = 0;
        }
    }

    void ShaderGLES::Bind() {
        glUseProgram(m_program);
    }

} // namespace Renderer
