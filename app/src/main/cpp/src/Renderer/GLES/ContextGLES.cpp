#include "Renderer/GLES/ContextGLES.h"

#include <android/native_window.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "NativeGL", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "NativeGL", __VA_ARGS__))

namespace Renderer {

    void ContextGLES::initialize(void* windowData) {
        ANativeWindow* window = reinterpret_cast<ANativeWindow*>(windowData);

        const EGLint attribs[] = {
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_DEPTH_SIZE, 16,
                EGL_NONE
        };

        EGLint numConfigs;
        EGLConfig config;
        EGLint format;

        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) { LOGE("Failed to get EGL display"); return; }

        if (!eglInitialize(display, nullptr, nullptr)) { LOGE("Failed to initialize EGL"); return; }

        if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) { LOGE("Failed to choose EGL config"); return; }

        eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
        ANativeWindow_setBuffersGeometry(window, 0, 0, format);

        surface = eglCreateWindowSurface(display, config, window, nullptr);
        if (surface == EGL_NO_SURFACE) { LOGE("Failed to create EGL surface"); return; }

        eglBindAPI(EGL_OPENGL_ES_API);

        const EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
        context = eglCreateContext(display, config, nullptr, contextAttribs);
        if (context == EGL_NO_CONTEXT) { LOGE("Failed to create EGL context"); return; }

        if (!eglMakeCurrent(display, surface, surface, context)) {
            LOGE("eglMakeCurrent failed: %x", eglGetError());
            return;
        }

        eglSwapInterval(display, 1);

        eglQuerySurface(display, surface, EGL_WIDTH, &width);
        eglQuerySurface(display, surface, EGL_HEIGHT, &height);
        glViewport(0, 0, width, height);

        initialized = true;
        LOGI("EGL initialized %dx%d", width, height);
    }

    void ContextGLES::drawFrame() {
        if (!initialized) return;
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        eglSwapBuffers(display, surface);
    }

    void ContextGLES::terminate() {
        terminateGLES();
    }

    void ContextGLES::terminateGLES() {
        if (display != EGL_NO_DISPLAY) {
            eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (context != EGL_NO_CONTEXT) eglDestroyContext(display, context);
            if (surface != EGL_NO_SURFACE) eglDestroySurface(display, surface);
            eglTerminate(display);
        }

        display = EGL_NO_DISPLAY;
        context = EGL_NO_CONTEXT;
        surface = EGL_NO_SURFACE;
        initialized = false;
        width = height = 0;

        LOGI("EGL terminated");
    }

    void ContextGLES::onPause() { terminate(); }
    void ContextGLES::onResume(void* windowData) { initialize(windowData); }

} // namespace Renderer
