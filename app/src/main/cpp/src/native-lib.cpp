#include "Renderer/Renderer.h"
#include "Content/Loader.h"
#include "Input/Input.h"
#include "Scene/Scene.h"

#include <jni.h>
#include <string>
#include <android/native_window.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager_jni.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "NativeGL", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "NativeGL", __VA_ARGS__))

extern "C" JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_initLoader(
        JNIEnv* env, jobject /* this */, jobject assetManager) {
    Loader::get().Init(AAssetManager_fromJava(env, assetManager));
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_stringFromJNI(
        JNIEnv* env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_onPointerStart(
        JNIEnv* env, jobject thiz, jint id, jfloat x, jfloat y) {
    Input::Input::get().onPointerDown(id, x, y);
    LOGI("PointerStart id=%d x=%f y=%f", id, x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_onPointerMove(
        JNIEnv* env, jobject thiz, jint id, jfloat x, jfloat y) {
    Input::Input::get().onPointerMove(id, x, y);
    LOGI("PointerMove id=%d x=%f y=%f", id, x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_onPointerEnd(
        JNIEnv* env, jobject thiz, jint id) {
    Input::Input::get().onPointerUp(id);
    LOGI("PointerEnd id=%d", id);
}

#include <android/input.h>

[[maybe_unused]] void android_main(struct android_app* app) {
    app->onAppCmd = [](struct android_app* app, int32_t cmd) {
        switch (cmd) {
            case APP_CMD_INIT_WINDOW:
                if (app->window) {
                    Renderer::Renderer::get().createContext(reinterpret_cast<void*>(app->window));
                    Loader::get().Init(app->activity->assetManager);
                    Scene::Scene::get().LoadScene("cube_scene.ini");
                }
                break;

            case APP_CMD_TERM_WINDOW:
                Renderer::Renderer::get().destroyContext();
                break;
        }
    };

    app->onInputEvent = [](struct android_app* app, AInputEvent* event) -> int32_t {
        if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
            int32_t action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
            int32_t pointerIndex = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            int32_t pointerId = AMotionEvent_getPointerId(event, pointerIndex);
            float x = AMotionEvent_getX(event, pointerIndex);
            float y = AMotionEvent_getY(event, pointerIndex);

            switch (action) {
                case AMOTION_EVENT_ACTION_DOWN:
                case AMOTION_EVENT_ACTION_POINTER_DOWN:
                    Input::Input::get().onPointerDown(pointerId, x, y);
                    break;

                case AMOTION_EVENT_ACTION_MOVE:
                    for (int i = 0; i < AMotionEvent_getPointerCount(event); ++i) {
                        int id = AMotionEvent_getPointerId(event, i);
                        float mx = AMotionEvent_getX(event, i);
                        float my = AMotionEvent_getY(event, i);
                        Input::Input::get().onPointerMove(id, mx, my);
                    }
                    break;

                case AMOTION_EVENT_ACTION_UP:
                case AMOTION_EVENT_ACTION_POINTER_UP:
                case AMOTION_EVENT_ACTION_CANCEL:
                    Input::Input::get().onPointerUp(pointerId);
                    break;
            }

            return 1;
        }
        return 0;
    };

    int events;
    struct android_poll_source* source;

    while (true) {
        while (ALooper_pollOnce(0, nullptr, &events, (void**)&source) >= 0) {
            if (source) source->process(app, source);
            if (app->destroyRequested) return;
        }

        Renderer::Renderer::get().beginFrame();
        Scene::Scene::get().Update();
        Scene::Scene::get().Draw();
        Renderer::Renderer::get().endFrame();
    }
}
