#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "Entity.h"
#include "Util/Singleton.h"

namespace Scene
{
    using EntityPtr = std::shared_ptr<Entity>;

    class Scene : public Singleton<Scene>
    {
    public:
        EntityPtr GetRoot();
        EntityPtr CreateEntity(const std::string& name, EntityPtr parent);
        EntityPtr FindEntity(const std::string& name);

        void Update();
        void Draw();

    protected:
        Scene();

    private:
        EntityPtr m_root;
        std::unordered_map<std::string, std::weak_ptr<Entity>> m_entityMap;

        friend class Singleton<Scene>;
    };
}
