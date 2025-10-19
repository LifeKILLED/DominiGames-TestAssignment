#pragma once

#include <glm/glm.hpp>
#include <array>

namespace Renderer {

    struct BasicMesh {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 normal;
        glm::vec4 color;

        static constexpr std::array<int, 4> vertexAttributes = {3, 2, 3, 4};
    };

} // namespace Renderer
