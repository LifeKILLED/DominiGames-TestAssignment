#include "Transform.h"
#include "Entity.h"

#define GLM_ENABLE_EXPERIMENTAL 1

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace Scene {
    void Transform::SetEntity(EntityPtr entity) {
        m_entity = entity;
    }

    EntityPtr Transform::GetEntity() const {
        return m_entity.lock();
    }

    void Transform::UpdateLocalMatrix() const {
        if (!m_localIsDirty)
            return;

        glm::mat4 T = glm::translate(glm::mat4(1.0f), m_position);
        glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), {1, 0, 0});
        glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), {0, 1, 0});
        glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), {0, 0, 1});
        glm::mat4 S = glm::scale(glm::mat4(1.0f), m_scale);
        m_localMatrix = T * Rz * Ry * Rx * S;

        m_localIsDirty = false;
        m_worldIsDirty = true;
    }

    void Transform::MarkDirty(bool onlyWorldMatrix) {
        m_localIsDirty = m_worldIsDirty = true;

        if (auto owner = m_entity.lock()) {
            for (auto &child: owner->GetChildren())
                if (child)
                    child->GetTransform()->MarkDirty(true);
        }
    }
    void Transform::UpdateWorldMatrix() const {
        UpdateLocalMatrix();

        if (!m_worldIsDirty)
            return;

        if (auto owner = m_entity.lock()) {
            if (auto parent = owner->GetParent().lock()) {
                auto parentMatrix = parent->GetTransform()->GetWorldMatrix();
                m_worldMatrix = parentMatrix * m_localMatrix;
            } else {
                m_worldMatrix = m_localMatrix;
            }
        }

        m_worldIsDirty = false;
    }

    void Transform::LookAt(const glm::vec3& target, const glm::vec3& up) {
        glm::mat4 view = glm::lookAt(m_position, target, up);
        glm::mat4 invView = glm::inverse(view);

        glm::vec3 scale, translation, skew;
        glm::vec4 perspective;
        glm::quat rotationQuat;
        glm::decompose(invView, scale, rotationQuat, translation, skew, perspective);

        m_rotation = glm::degrees(glm::eulerAngles(rotationQuat));

        MarkDirty();
    }

    glm::mat4 Transform::GetViewProjection(const glm::mat4& projection) const {
        glm::mat4 view = glm::inverse(GetWorldMatrix());
        return projection * view;
    }
}
