#include "Scene.h"



namespace Scene
{
    Scene::Scene()
    {
        m_root = std::make_shared<Entity>("Root");
        m_entityMap.emplace(m_root->GetName(), m_root);
    }

    EntityPtr Scene::GetRoot()
    {
        return m_root;
    }

    EntityPtr Scene::CreateEntity(const std::string& name, EntityPtr parent)
    {
        auto entity = std::make_shared<Entity>();
        entity->SetName(name);

        if (parent)
            parent->AddChild(entity);
        else
            m_root->AddChild(entity);

        m_entityMap.emplace(entity->GetName(), entity);

        return entity;
    }

    EntityPtr Scene::FindEntity(const std::string& name)
    {
        auto it = m_entityMap.find(name);
        if (it != m_entityMap.end())
            return it->second.lock();
        return nullptr;
    }

    void Scene::Update()
    {
        // TODO
    }

    void Scene::Draw()
    {
        m_root->DrawRecursive();
    }
}
