#include "Loader.h"

#include "Renderer/MeshData/BasicMesh.h"

#include <sstream>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>

void Loader::Init(AAssetManager* mgr) {
    assetManager = mgr;
}

std::string Loader::LoadTextFile(const std::string& filename) {
    std::string fullPath = "shaders/" + filename;
    AAsset* asset = AAssetManager_open(assetManager, fullPath.c_str(), AASSET_MODE_STREAMING);
    if (!asset) {
        LOGE("Can't open file: %s", fullPath.c_str());
        return {};
    }

    size_t size = AAsset_getLength(asset);
    std::string result(size, '\0');
    AAsset_read(asset, result.data(), size);
    AAsset_close(asset);
    return result;
}

std::vector<Renderer::BasicMesh> Loader::LoadMesh(const std::string& filename) {
    std::vector<Renderer::BasicMesh> vertices;
    std::string fullPath = "mesh/" + filename;
    AAsset* asset = AAssetManager_open(assetManager, fullPath.c_str(), AASSET_MODE_STREAMING);
    if (!asset) {
        LOGE("Can't open mesh: %s", fullPath.c_str());
        return vertices;
    }

    size_t size = AAsset_getLength(asset);
    std::string content(size, '\0');
    AAsset_read(asset, content.data(), size);
    AAsset_close(asset);

    std::stringstream ss(content);
    std::string line;
    while (std::getline(ss, line)) {
        if (line.empty() || line[0] == '#') continue;
        Renderer::BasicMesh v{};
        std::stringstream ls(line);
        ls >> v.position.x >> v.position.y >> v.position.z
           >> v.normal.x >> v.normal.y >> v.normal.z
           >> v.uv.x >> v.uv.y
           >> v.color.r >> v.color.g >> v.color.b >> v.color.a;
        vertices.push_back(v);
    }

    return vertices;
}
