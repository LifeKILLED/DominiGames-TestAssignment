#pragma once

#include "Resource.h"
#include "Renderer/MeshData.h"

#include <vector>
#include <optional>

namespace Renderer {

    class Mesh : public Resource {
    public:
        Mesh() = default;
        ~Mesh() override;

        void SetData(const MeshData& data);

        void Load() override;
        void Unload() override;
        void Draw();

    private:
        void BuildVertexCache();

    private:
        MeshData m_meshData;

        std::optional<unsigned int> m_vbo;   // Vertex buffer
        std::optional<unsigned int> m_ibo;   // Index buffer
        bool m_isDirty {true};

        std::vector<float> m_vertexCache;
        std::vector<int> m_vertexAttributes;
        int m_stride {0};
    };

} // namespace Renderer
