#pragma once

#include "Entity.h"
#include "Util/Singleton.h"

#include <memory>
#include <unordered_map>
#include <map>
#include <string>
#include <glm/glm.hpp>

namespace Scene
{
    using EntityPtr = std::shared_ptr<Entity>;

    class Scene : public Singleton<Scene>
    {
    public:
        bool LoadScene(const std::string& iniFile);

        EntityPtr GetRoot();
        EntityPtr CreateEntity(const std::string& name, EntityPtr parent);
        EntityPtr FindEntity(const std::string& name);

        EntityPtr GetCamera() const;
        void SetCamera(EntityPtr entity);
        glm::mat4& GetProjection() { return m_projection; }

        void Update();
        void Draw();

        void PrintHierarchy();

    protected:
        Scene();

    private:
        void ParseEntity(const std::map<std::string, std::string>& data);

    private:
        EntityPtr m_root;
        std::unordered_map<std::string, std::weak_ptr<Entity>> m_entities;
        std::weak_ptr<Entity> m_camera;
        glm::mat4 m_projection{1.0f};
        friend class Singleton<Scene>;
    };
}
