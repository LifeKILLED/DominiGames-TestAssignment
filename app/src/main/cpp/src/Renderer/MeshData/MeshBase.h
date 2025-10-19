#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Renderer {

    struct MeshBase {
        glm::vec3 position;

        virtual std::vector<int>& GetAttributes() const { static auto a = std::vector<int>{3}; return a; }
    };

} // namespace Renderer
