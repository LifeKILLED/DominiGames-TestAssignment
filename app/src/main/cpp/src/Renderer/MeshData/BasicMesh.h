#pragma once

#include <glm/glm.hpp>
#include <array>

namespace Renderer {

    struct BasicMesh {
        glm::vec3 position;
        glm::vec2 uv;

        static constexpr std::array<int, 2> vertexAttributes = {3, 2};
    };

} // namespace Renderer
