#include <jni.h>
#include <string>
#include <android/native_window.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "Renderer/Renderer.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "NativeGL", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "NativeGL", __VA_ARGS__))

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

[[maybe_unused]] void android_main(struct android_app* app) {
    app->onAppCmd = [](struct android_app* app, int32_t cmd) {
        switch (cmd) {
            case APP_CMD_INIT_WINDOW:
                if (app->window)
                    Renderer::Renderer::get().createContext(reinterpret_cast<void*>(app->window));
                break;
            case APP_CMD_TERM_WINDOW:
                Renderer::Renderer::get().destroyContext();
                break;
        }
    };

    int events;
    struct android_poll_source* source;

    while (true) {
        while (ALooper_pollOnce(0, nullptr, &events, (void**)&source) >= 0) {
            if (source) source->process(app, source);
            if (app->destroyRequested) return;
        }

        Renderer::Renderer::get().draw();
    }
}