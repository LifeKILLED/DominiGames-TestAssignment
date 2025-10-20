#include "Scene.h"

#include "Entity.h"
#include "Transform.h"
#include "Content/Loader.h"
#include "Renderer/MeshData.h"
#include "Renderer/MeshRenderer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Mesh.h"

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
        // TODO
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

        std::stringstream ss(content);
        while (ss) {
            std::string line;
            std::getline(ss, line);
            if (line.empty() || line[0] == '#') continue;

            if (line.find("[Entity]") != std::string::npos) {
                ParseEntity(ss);
            }
        }

        return true;
    }

    void Scene::ParseEntity(std::stringstream& ss) {
        std::string line;
        std::string name;
        glm::vec3 position(0.0f);
        glm::vec3 rotation(0.0f);
        glm::vec3 scale(1.0f);
        std::string modelFile;
        std::string shaderFile = "basic"; // default shader name
        bool isCamera = false;
        std::string parentName;

        while (std::getline(ss, line)) {
            if (line.empty() || line[0] == '#') continue;
            if (line[0] == '[') break;

            auto pos = line.find('=');
            if (pos == std::string::npos) continue;

            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
            value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());

            if (key == "name") name = value;
            else if (key == "position") position = ParseVec3(value);
            else if (key == "rotation") rotation = ParseVec3(value);
            else if (key == "scale") scale = ParseVec3(value);
            else if (key == "model") modelFile = value;
            else if (key == "shader") shaderFile = value;
            else if (key == "isCamera") isCamera = (value == "1" || value == "true");
            else if (key == "parent") parentName = value;
        }

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
}
