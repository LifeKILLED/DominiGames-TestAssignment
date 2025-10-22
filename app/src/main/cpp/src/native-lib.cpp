#include "Renderer/Renderer.h"
#include "Content/Loader.h"
#include "Input/Input.h"
#include "Scene/Scene.h"

#include <jni.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "NativeGL", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "NativeGL", __VA_ARGS__))

extern "C" {

extern "C" JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_initLoader(
        JNIEnv* env, jobject /* this */, jobject assetManager) {
    Loader::get().Init(AAssetManager_fromJava(env, assetManager));
}

JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_SceneLoadFromFile(
        JNIEnv* env, jobject /* this */, jstring filename) {
    const char* path = env->GetStringUTFChars(filename, nullptr);
    Scene::Scene::get().LoadScene(path);
    env->ReleaseStringUTFChars(filename, path);
}

JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_SceneUpdate(
        JNIEnv* env, jobject /* this */) {
    Scene::Scene::get().Update();
}

JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_SceneDraw(
        JNIEnv* env, jobject /* this */) {
    Scene::Scene::get().Draw();
}

JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_RendererBeginFrame(
        JNIEnv* env, jobject /* this */) {
    Renderer::Renderer::get().beginFrame();
}

JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_RendererEndFrame(
        JNIEnv* env, jobject /* this */) {
    Renderer::Renderer::get().endFrame();
}

JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_RendererSetViewport(
        JNIEnv* env, jobject /* this */, jint width, jint height) {
    Renderer::Renderer::get().setViewport(width, height);
}

JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_onPointerStart(
        JNIEnv* env, jobject /* this */, jint id, jfloat x, jfloat y) {
    Input::Input::get().onPointerDown(id, x, y);
    LOGI("PointerStart id=%d x=%f y=%f", id, x, y);
}

JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_onPointerMove(
        JNIEnv* env, jobject /* this */, jint id, jfloat x, jfloat y) {
    Input::Input::get().onPointerMove(id, x, y);
    LOGI("PointerMove id=%d x=%f y=%f", id, x, y);
}

JNIEXPORT void JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_onPointerEnd(
        JNIEnv* env, jobject /* this */, jint id) {
    Input::Input::get().onPointerUp(id);
    LOGI("PointerEnd id=%d", id);
}

JNIEXPORT jstring JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_stringFromJNI(
        JNIEnv* env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

} // extern "C"
