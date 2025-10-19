#include "Entity.h"

#include "Renderer/MeshRenderer.h"
#include "Transform.h"

namespace Scene
{
    Entity::Entity(const std::string& name)
        : m_name(name)
    {
        m_transform = std::make_shared<Transform>();
        m_transform->SetEntity(shared_from_this());
        m_renderer = std::make_shared<Renderer::MeshRenderer>();
    }

    std::shared_ptr<Transform> Entity::GetTransform() { return m_transform; }
    std::shared_ptr<Renderer::MeshRenderer> Entity::GetRenderer() { return m_renderer; }

    const std::string& Entity::GetName() const { return m_name; }
    void Entity::SetName(const std::string& name) { m_name = name; }

    void Entity::AddChild(EntityPtr child)
    {
        if (!child) return;
        child->m_parent = shared_from_this();
        m_children.push_back(child);
    }

    const std::vector<std::shared_ptr<Entity>> &Entity::GetChildren() const {
        return m_children;
    }

    std::weak_ptr<Entity> Entity::GetParent() const
    {
        return m_parent;
    }

    void Entity::DrawRecursive()
    {
        if (m_renderer)
            m_renderer->Draw(m_transform->GetWorldMatrix());

        for (auto& child : m_children)
        {
            if (child)
                child->DrawRecursive();
        }
    }
}
