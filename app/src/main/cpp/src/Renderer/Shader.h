#pragma once

#include "Resource.h"

#include <string>
#include <utility>

namespace Renderer {

    class Shader : public Resource {
    public:
        Shader(std::string  vertSrc, std::string  fragSrc)
                : m_vertSrc(std::move(vertSrc)), m_fragSrc(std::move(fragSrc)) {}

        virtual void Bind() = 0;

    protected:
        std::string m_vertSrc;
        std::string m_fragSrc;
    };

} // namespace Renderer
