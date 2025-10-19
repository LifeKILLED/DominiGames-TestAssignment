#pragma once

#include "Renderer/Context.h"

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <android/log.h>

namespace Renderer {

    class ContextGLES : public Context {
    public:
        ContextGLES() = default;
        ~ContextGLES() override { terminateGLES(); }

        void initialize(void* windowData) override;
        void drawFrame() override;
        void terminate() override;

        void onPause() override;
        void onResume(void* windowData) override;

        bool isInitialized() const override { return m_initialized; }

        int getWidth() const override { return m_width; }
        int getHeight() const override { return m_height; }

    private:
        void terminateGLES();

    private:
        EGLDisplay m_display = EGL_NO_DISPLAY;
        EGLSurface m_surface = EGL_NO_SURFACE;
        EGLContext m_context = EGL_NO_CONTEXT;
        bool m_initialized = false;
        int m_width = 0;
        int m_height = 0;
    };
} // namespace Renderer
