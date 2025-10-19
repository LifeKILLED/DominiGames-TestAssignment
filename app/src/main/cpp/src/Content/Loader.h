#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <glm/glm.hpp>

namespace Renderer {
    struct BasicMesh;
}

class Loader {
public:
    Loader(AAssetManager* mgr) : assetManager(mgr) {}

    std::string LoadTextFile(const std::string& filename);

    std::vector<Renderer::BasicMesh> LoadMesh(const std::string& filename);

private:
    AAssetManager* assetManager;
};
