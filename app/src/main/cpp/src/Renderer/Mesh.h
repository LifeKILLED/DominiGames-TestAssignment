#pragma once

#include "Resource.h"

#include <vector>
#include <cstring>
#include <type_traits>

namespace Renderer {

    class Mesh : public Resource {
    public:
        Mesh() : m_isDirty(true) {}
        ~Mesh() override = default;

        template<typename T>
        void LoadVertices(const std::vector<T>& vertices) {
            if constexpr (std::is_same_v<decltype(T::vertexAttributes), const std::array<int, T::vertexAttributes.size()>&>) {
                m_vertexAttributes = std::vector<int>(T::vertexAttributes.begin(), T::vertexAttributes.end());
            }

            m_vertexCache.resize((vertices.size() * sizeof(T)) / sizeof(float));
            if (!m_vertexCache.empty())
                std::memcpy(m_vertexCache.data(), vertices.data(), sizeof(T) * vertices.size());

            m_isDirty = true;
        }

        virtual void Draw() = 0;

    protected:
        std::vector<float> m_vertexCache;
        bool m_isDirty;
        std::vector<int> m_vertexAttributes;
    };

} // namespace Renderer
