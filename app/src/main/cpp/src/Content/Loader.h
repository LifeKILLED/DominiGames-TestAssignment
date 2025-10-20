#pragma once

#include "Util/Singleton.h"

#include <string>
#include <vector>
#include <android/asset_manager.h>
#include <glm/glm.hpp>
#include <android/log.h>

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "NativeGL", __VA_ARGS__))

namespace Renderer {
    struct BasicMesh;
}

class Loader : public Singleton<Loader> {
    friend class Singleton<Loader>;

public:
    void Init(AAssetManager* mgr);

    std::string LoadTextFile(const std::string& filename);

private:
    Loader() = default;
    ~Loader() = default;

    AAssetManager* assetManager{nullptr};
};
