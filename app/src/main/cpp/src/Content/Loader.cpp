#include "Loader.h"

#include <sstream>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>

void Loader::Init(AAssetManager* mgr) {
    assetManager = mgr;
}

std::string Loader::LoadTextFile(const std::string& filename) {
    AAsset* asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_STREAMING);
    if (!asset) {
        LOGE("Can't open file: %s", filename.c_str());
        return {};
    }

    size_t size = AAsset_getLength(asset);
    std::string result(size, '\0');
    AAsset_read(asset, result.data(), size);
    AAsset_close(asset);
    return result;
}
