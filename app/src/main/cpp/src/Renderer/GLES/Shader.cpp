#include "Renderer/Shader.h"

#include <GLES2/gl2.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace Renderer {

    Shader::Shader(const std::string& vertSrc, const std::string& fragSrc)
            : m_vertSrc(vertSrc), m_fragSrc(fragSrc)
    {}

    Shader::~Shader() {
        Unload();
    }

    void Shader::Load() {
        GLuint vert = glCreateShader(GL_VERTEX_SHADER);
        const char* vSrc = m_vertSrc.c_str();
        glShaderSource(vert, 1, &vSrc, nullptr);
        glCompileShader(vert);

        GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fSrc = m_fragSrc.c_str();
        glShaderSource(frag, 1, &fSrc, nullptr);
        glCompileShader(frag);

        m_id = glCreateProgram();
        glAttachShader(m_id.value(), vert);
        glAttachShader(m_id.value(), frag);
        glLinkProgram(m_id.value());

        glDeleteShader(vert);
        glDeleteShader(frag);

        GLint success;
        glGetProgramiv(m_id.value(), GL_LINK_STATUS, &success);
        if (success != GL_TRUE) {
            char buffer[512];
            glGetProgramInfoLog(m_id.value(), 512, nullptr, buffer);
            std::cerr << "Shader link error: " << buffer << std::endl;
            return;
        }

        m_transformId = glGetUniformLocation(m_id.value(), "transform");
        m_projectionId = glGetUniformLocation(m_id.value(), "projection");
    }

    void Shader::Unload() {
        if (m_id.has_value()) {
            glDeleteProgram(m_id.value());
            m_id.reset();
        }
    }

    void Shader::Bind() {
        if (m_id.has_value())
            glUseProgram(m_id.value());
    }

    void Shader::SetTransform(const glm::mat4 transform) {
        glUniformMatrix4fv(m_transformId, 1, GL_FALSE, glm::value_ptr(transform));
    }

} // namespace Renderer
