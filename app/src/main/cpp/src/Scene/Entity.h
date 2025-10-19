#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Renderer {
    class MeshRenderer;
}

namespace Scene
{
    class Transform;

    class Entity : public std::enable_shared_from_this<Entity>
    {
    public:
        explicit Entity(const std::string& name = "Entity");

        void Init();

        const std::string& GetName() const;
        void SetName(const std::string& name);

        std::shared_ptr<Transform> GetTransform();
        std::shared_ptr<Renderer::MeshRenderer> GetRenderer();

        void AddChild(std::shared_ptr<Entity> child);
        const std::vector<std::shared_ptr<Entity>>& GetChildren() const;

        void SetParent(std::shared_ptr<Entity> parent);
        std::shared_ptr<Entity> GetParent() const;

        void DrawRecursive();

        bool IsRoot() const { return m_parent.expired(); }

    private:
        std::string m_name;
        std::shared_ptr<Transform> m_transform;
        std::shared_ptr<Renderer::MeshRenderer> m_renderer;

        std::vector<std::shared_ptr<Entity>> m_children;
        std::weak_ptr<Entity> m_parent;
    };
}
