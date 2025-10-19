#pragma once

#include "Resource.h"

#include <vector>
#include <cstring>
#include <type_traits>
#include <memory>

namespace Renderer {

    class Mesh : public Resource {
    public:
        Mesh() = default;
        ~Mesh() override;

        template<typename T>
        void LoadVertices(const std::vector<T>& vertices);

        void Load() override;
        void Unload() override;
        void Draw();

    private:
        std::vector<float> m_vertexCache;
        std::vector<int> m_vertexAttributes;
        std::optional<unsigned int> m_id;
        bool m_isDirty {true};
    };

    template<typename T>
    void Mesh::LoadVertices(const std::vector<T>& vertices) {
        if constexpr (std::is_same_v<decltype(T::vertexAttributes), const std::array<int, T::vertexAttributes.size()>&>) {
            m_vertexAttributes = std::vector<int>(T::vertexAttributes.begin(), T::vertexAttributes.end());
        }

        m_vertexCache.resize((vertices.size() * sizeof(T)) / sizeof(float));
        if (!m_vertexCache.empty())
            std::memcpy(m_vertexCache.data(), vertices.data(), sizeof(T) * vertices.size());

        m_isDirty = true;
    }

} // namespace Renderer
