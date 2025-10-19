#pragma once

#include "MeshBase.h"

namespace Renderer {

    struct BasicMesh : public MeshBase {
        glm::vec2 uv;
        glm::vec3 normal;
        glm::vec4 color;

        std::vector<int> & GetAttributes() const override { static auto a = std::vector<int>{3, 2, 3, 4}; return a; }
    };

} // namespace Renderer
