#include "Renderer/Shader.h"

#include "Renderer/Renderer.h"
#include "Renderer/Context.h"

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
        const auto& context = Renderer::Renderer::get().getContext();
        if (context == nullptr || !context->isInitialized())
            return;

        m_isLoaded = true;

        GLuint vert = glCreateShader(GL_VERTEX_SHADER);
        const char* vSrc = m_vertSrc.c_str();
        glShaderSource(vert, 1, &vSrc, nullptr);
        glCompileShader(vert);

        GLint success = GL_FALSE;
        glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
        if (success != GL_TRUE) {
            char buffer[512] = {};
            glGetShaderInfoLog(vert, sizeof(buffer), nullptr, buffer);
            std::cerr << "Vertex shader compile error:\n" << buffer << std::endl;
            glDeleteShader(vert);
            return;
        }

        GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fSrc = m_fragSrc.c_str();
        glShaderSource(frag, 1, &fSrc, nullptr);
        glCompileShader(frag);

        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if (success != GL_TRUE) {
            char buffer[512] = {};
            glGetShaderInfoLog(frag, sizeof(buffer), nullptr, buffer);
            std::cerr << "Fragment shader compile error:\n" << buffer << std::endl;
            glDeleteShader(vert);
            glDeleteShader(frag);
            return;
        }

        m_id = glCreateProgram();
        glAttachShader(m_id.value(), vert);
        glAttachShader(m_id.value(), frag);

        glBindAttribLocation(m_id.value(), 0, "position");
        glBindAttribLocation(m_id.value(), 1, "normal");
        glBindAttribLocation(m_id.value(), 2, "uv");
        glBindAttribLocation(m_id.value(), 3, "color");

        glLinkProgram(m_id.value());

        glDeleteShader(vert);
        glDeleteShader(frag);

        glGetProgramiv(m_id.value(), GL_LINK_STATUS, &success);
        if (success != GL_TRUE) {
            char buffer[512] = {};
            glGetProgramInfoLog(m_id.value(), sizeof(buffer), nullptr, buffer);
            std::cerr << "Shader link error:\n" << buffer << std::endl;
            glDeleteProgram(m_id.value());
            m_id.reset();
            return;
        }

        Bind();
        m_transformId = glGetUniformLocation(m_id.value(), "transform");
        m_projectionId = glGetUniformLocation(m_id.value(), "projection");
    }

    void Shader::Unload() {
        if (m_id.has_value()) {
            glDeleteProgram(m_id.value());
            m_id.reset();
        }

        m_isLoaded = false;
    }

    void Shader::Bind() {
        if (!m_isLoaded)
            Load();

        if (m_id.has_value())
            glUseProgram(m_id.value());
    }

    void Shader::SetTransform(const glm::mat4 transform) {
        glUniformMatrix4fv(m_transformId, 1, GL_FALSE, glm::value_ptr(transform));
    }

    void Shader::SetProjection(const glm::mat4 projection) {
        glUniformMatrix4fv(m_projectionId, 1, GL_FALSE, glm::value_ptr(projection));
    }

} // namespace Renderer
