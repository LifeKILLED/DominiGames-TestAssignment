#include <jni.h>
#include <string>
#include <android/native_window.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "NativeGL", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "NativeGL", __VA_ARGS__))

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_dominigames_1testassignment_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

struct Engine {
    ANativeWindow* window = nullptr;
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    bool initialized = false;
};

static bool initDisplay(Engine* engine) {
    if (!engine->window) return false;

    const EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_NONE
    };

    EGLint numConfigs;
    EGLConfig config;
    EGLint format;

    engine->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (engine->display == EGL_NO_DISPLAY) {
        LOGE("Failed to get EGL display");
        return false;
    }

    if (!eglInitialize(engine->display, nullptr, nullptr)) {
        LOGE("Failed to initialize EGL");
        return false;
    }

    if (!eglChooseConfig(engine->display, attribs, &config, 1, &numConfigs)) {
        LOGE("Failed to choose EGL config");
        return false;
    }

    eglGetConfigAttrib(engine->display, config, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(engine->window, 0, 0, format);

    engine->surface = eglCreateWindowSurface(engine->display, config, engine->window, nullptr);
    if (engine->surface == EGL_NO_SURFACE) {
        LOGE("Failed to create EGL surface");
        return false;
    }

    eglBindAPI(EGL_OPENGL_ES_API);

    const EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    engine->context = eglCreateContext(engine->display, config, nullptr, contextAttribs);
    if (engine->context == EGL_NO_CONTEXT) {
        LOGE("Failed to create EGL context");
        return false;
    }

    if (!eglMakeCurrent(engine->display, engine->surface, engine->surface, engine->context)) {
        LOGE("eglMakeCurrent failed: %x", eglGetError());
        return false;
    }
    else {
        LOGI("eglMakeCurrent OK");
    }

    eglSwapInterval(engine->display, 1);

    EGLint w, h;
    eglQuerySurface(engine->display, engine->surface, EGL_WIDTH, &w);
    eglQuerySurface(engine->display, engine->surface, EGL_HEIGHT, &h);
    glViewport(0, 0, w, h);

    engine->initialized = true;
    LOGI("EGL initialized %dx%d", w, h);

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(engine->display, engine->surface);

    return true;
}

// --- Отрисовка кадра ---
static void drawFrame(Engine* engine) {
    if (!engine->initialized) return;

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush(); // TODO: try to remove this

    eglSwapBuffers(engine->display, engine->surface);
}

static void termDisplay(Engine* engine) {
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT)
            eglDestroyContext(engine->display, engine->context);
        if (engine->surface != EGL_NO_SURFACE)
            eglDestroySurface(engine->display, engine->surface);
        eglTerminate(engine->display);
    }

    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
    engine->initialized = false;

    LOGI("EGL terminated");
}

void android_main(struct android_app* app) {
    Engine engine = {};
    app->userData = &engine;

    app->onAppCmd = [](struct android_app* app, int32_t cmd) {
        Engine* engine = (Engine*)app->userData;
        switch (cmd) {
            case APP_CMD_INIT_WINDOW:
                if (app->window && !engine->initialized) {
                    engine->window = app->window;
                    initDisplay(engine);
                }
                break;
            case APP_CMD_TERM_WINDOW:
                termDisplay(engine);
                break;
        }
    };

    int events;
    struct android_poll_source* source;

    while (true) {
        while (ALooper_pollOnce(0, nullptr, &events, (void**)&source) >= 0) {
            if (source)
                source->process(app, source);

            if (app->destroyRequested)
                return;
        }

        if (engine.initialized)
            drawFrame(&engine);
    }
}
