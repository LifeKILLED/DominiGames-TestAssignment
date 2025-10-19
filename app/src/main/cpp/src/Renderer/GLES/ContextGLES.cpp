#include "Renderer/GLES/ContextGLES.h"

#include "Scene/Scene.h" // TODO: remove

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

        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (m_display == EGL_NO_DISPLAY) { LOGE("Failed to get EGL m_display"); return; }

        if (!eglInitialize(m_display, nullptr, nullptr)) { LOGE("Failed to initialize EGL"); return; }

        if (!eglChooseConfig(m_display, attribs, &config, 1, &numConfigs)) { LOGE("Failed to choose EGL config"); return; }

        eglGetConfigAttrib(m_display, config, EGL_NATIVE_VISUAL_ID, &format);
        ANativeWindow_setBuffersGeometry(window, 0, 0, format);

        m_surface = eglCreateWindowSurface(m_display, config, window, nullptr);
        if (m_surface == EGL_NO_SURFACE) { LOGE("Failed to create EGL m_surface"); return; }

        eglBindAPI(EGL_OPENGL_ES_API);

        const EGLint m_contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
        m_context = eglCreateContext(m_display, config, nullptr, m_contextAttribs);
        if (m_context == EGL_NO_CONTEXT) { LOGE("Failed to create EGL m_context"); return; }

        if (!eglMakeCurrent(m_display, m_surface, m_surface, m_context)) {
            LOGE("eglMakeCurrent failed: %x", eglGetError());
            return;
        }

        eglSwapInterval(m_display, 1);

        eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_width);
        eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_height);
        glViewport(0, 0, m_width, m_height);

        m_initialized = true;
        LOGI("EGL initialized %dx%d", m_width, m_height);
    }

    void ContextGLES::drawFrame() {
        if (!m_initialized) return;
        glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Scene::Scene::get().Draw();

        glFlush();
        eglSwapBuffers(m_display, m_surface);
    }

    void ContextGLES::terminate() {
        terminateGLES();
    }

    void ContextGLES::terminateGLES() {
        if (m_display != EGL_NO_DISPLAY) {
            eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (m_context != EGL_NO_CONTEXT) eglDestroyContext(m_display, m_context);
            if (m_surface != EGL_NO_SURFACE) eglDestroySurface(m_display, m_surface);
            eglTerminate(m_display);
        }

        m_display = EGL_NO_DISPLAY;
        m_context = EGL_NO_CONTEXT;
        m_surface = EGL_NO_SURFACE;
        m_initialized = false;
        m_width = m_height = 0;

        LOGI("EGL terminated");
    }

    void ContextGLES::onPause() { terminate(); }
    void ContextGLES::onResume(void* windowData) { initialize(windowData); }

} // namespace Renderer
