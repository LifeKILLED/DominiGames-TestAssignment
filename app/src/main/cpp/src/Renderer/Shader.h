#pragma once

#include "Resource.h"

#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <utility>

namespace Renderer {

    class Shader : public Resource {
    public:
        Shader() = default;
        ~Shader() override;

        Shader(const std::string& vertSrc, const std::string& fragSrc);

        void SetTransform(const glm::mat4 transform);
        void SetProjection(const glm::mat4 projection);

        void Load() override;
        void Unload() override;
        void Bind();

    protected:
        std::string m_vertSrc;
        std::string m_fragSrc;
        std::optional<unsigned int> m_id;

        int m_transformId {};
        int m_projectionId {};
    };

} // namespace Renderer
