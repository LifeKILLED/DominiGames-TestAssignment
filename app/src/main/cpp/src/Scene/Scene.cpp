#include "Scene.h"

#include "Entity.h"
#include "Transform.h"
#include "Content/Loader.h"
#include "Renderer/MeshData.h"
#include "Renderer/MeshRenderer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Mesh.h"
#include "Input/Input.h"
#include "Util/IniUtils.h"

#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>

namespace {
    glm::vec3 ParseVec3(const std::string& line) {
        glm::vec3 v(0.0f);
        std::stringstream ss(line);
        ss >> v.x >> v.y >> v.z;
        return v;
    }

    void PrintEntityTreeRecursive(const Scene::EntityPtr& entity, const std::string& prefix = "", bool isLast = true) {
        if (!entity)
            return;

        std::cout << prefix;
        std::cout << (isLast ? "└── " : "├── ");
        std::cout << entity->GetName() << "\n";

        auto children = entity->GetChildren();
        for (size_t i = 0; i < children.size(); ++i)
        {
            bool lastChild = (i == children.size() - 1);
            PrintEntityTreeRecursive(children[i], prefix + (isLast ? "    " : "│   "), lastChild);
        }
    }
}

namespace Scene
{
    Scene::Scene()
    {
        m_root = std::make_shared<Entity>("Root");
        m_root->Init();

        m_entities.emplace(m_root->GetName(), m_root);
    }

    EntityPtr Scene::GetRoot()
    {
        return m_root;
    }

    EntityPtr Scene::CreateEntity(const std::string& name, EntityPtr parent)
    {
        auto entity = std::make_shared<Entity>(name);
        entity->Init();

        if (parent)
            parent->AddChild(entity);
        else
            m_root->AddChild(entity);

        m_entities.emplace(entity->GetName(), entity);

        return entity;
    }

    EntityPtr Scene::FindEntity(const std::string& name)
    {
        auto it = m_entities.find(name);
        if (it != m_entities.end())
            return it->second.lock();
        return nullptr;
    }

    void Scene::Update()
    {
        PrintHierarchy();

        auto camera = m_camera.lock();
        if (!camera)
            return;

        auto rotation = camera->GetTransform()->GetRotation();
        auto offset = Input::Input::get().getPointerDelta(0);
        rotation.x += offset.second;
        rotation.y += offset.first;
        camera->GetTransform()->SetRotation(rotation);
    }

    void Scene::Draw()
    {
        auto camera = m_camera.lock();
        if (!camera)
            return;

        m_projection = camera->GetTransform()->GetViewProjection();

        m_root->DrawRecursive();
    }

    bool Scene::LoadScene(const std::string& iniFile) {
        auto content = Loader::get().LoadTextFile("scene/" + iniFile);
        if (content.empty()) {
            std::cerr << "Scene INI empty: " << iniFile << "\n";
            return false;
        }

        auto iniData = IniUtils::LoadIniFromString(content);

        for (const auto& [section, kv] : iniData) {
            ParseEntity(kv);
        }

        return true;
    }

    void Scene::ParseEntity(const std::map<std::string, std::string>& data) {
        std::string name = data.at("name");
        glm::vec3 position(0.0f);
        glm::vec3 rotation(0.0f);
        glm::vec3 scale(1.0f);
        std::string modelFile;
        std::string shaderFile = "basic";
        bool isCamera = false;
        std::string parentName;

        if (auto it = data.find("position"); it != data.end())
            position = ParseVec3(it->second);
        if (auto it = data.find("rotation"); it != data.end())
            rotation = ParseVec3(it->second);
        if (auto it = data.find("scale"); it != data.end())
            scale = ParseVec3(it->second);
        if (auto it = data.find("model"); it != data.end())
            modelFile = it->second;
        if (auto it = data.find("shader"); it != data.end())
            shaderFile = it->second;
        if (auto it = data.find("isCamera"); it != data.end())
            isCamera = (it->second == "1" || it->second == "true");
        if (auto it = data.find("parent"); it != data.end())
            parentName = it->second;

        auto entity = CreateEntity(name, FindEntity(parentName));
        entity->GetTransform()->SetPosition(position);
        entity->GetTransform()->SetRotation(rotation);
        entity->GetTransform()->SetScale(scale);

        if (isCamera) {
            SetCamera(entity);
        }

        if (!modelFile.empty()) {
            Renderer::MeshData meshData;
            meshData.LoadFromString(Loader::get().LoadTextFile("mesh/" + modelFile));
            auto mesh = Renderer::Renderer::get().createMesh();
            mesh->SetData(meshData);
            entity->GetRenderer()->SetMesh(mesh);

            auto vert = Loader::get().LoadTextFile("shaders/" + shaderFile + ".vert");
            auto frag = Loader::get().LoadTextFile("shaders/" + shaderFile + ".frag");
            auto shader = Renderer::Renderer::get().createShader(vert, frag);
            entity->GetRenderer()->SetShader(shader);
        }

        m_entities[name] = entity;
    }

    EntityPtr Scene::GetCamera() const {
        return m_camera.lock();
    }

    void Scene::SetCamera(EntityPtr entity) {
        m_camera = entity;
    }

    void Scene::PrintHierarchy()
    {
        std::cout << "Scene Hierarchy:\n";
        PrintEntityTreeRecursive(m_root, "", true);
    }
}
