#pragma once
#include <memory>
#include <glm/glm.hpp>

namespace Scene
{
    class Entity;
    using EntityPtr = std::shared_ptr<Entity>;

    class Transform
    {
    public:
        Transform() = default;

        void SetEntity(EntityPtr entity);
        EntityPtr GetEntity() const;

        void SetPosition(const glm::vec3& pos) { m_position = pos; MarkDirty(); }
        void SetRotation(const glm::vec3& rot) { m_rotation = rot; MarkDirty(); }
        void SetScale(const glm::vec3& scl) { m_scale = scl; MarkDirty(); }

        const glm::mat4& GetWorldMatrix() const { UpdateWorldMatrix(); return m_worldMatrix; }
        const glm::mat4& GetLocalMatrix() const { UpdateLocalMatrix(); return m_localMatrix; }

        void MarkDirty(bool onlyWorldMatrix = false);

    private:
        void UpdateLocalMatrix() const;
        void UpdateWorldMatrix() const;

    private:
        glm::vec3 m_position{0.0f};
        glm::vec3 m_rotation{0.0f};
        glm::vec3 m_scale{1.0f};

        mutable glm::mat4 m_localMatrix{1.0f};
        mutable glm::mat4 m_worldMatrix{1.0f};

        mutable bool m_localIsDirty{true};
        mutable bool m_worldIsDirty{true};

        std::weak_ptr<Entity> m_entity;
    };
}
