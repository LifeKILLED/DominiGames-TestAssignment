#pragma once

#include "Resource.h"

#include <string>
#include <utility>

namespace Renderer {

    class Shader : public Resource {
    public:
        Shader() = default;
        ~Shader() override;

        Shader(const std::string& vertSrc, const std::string& fragSrc);

        void Load() override;
        void Unload() override;
        void Bind();

    protected:
        std::string m_vertSrc;
        std::string m_fragSrc;
        std::optional<unsigned int> m_id;
    };

} // namespace Renderer
