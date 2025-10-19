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

        bool isInitialized() const override { return initialized; }

        int getWidth() const override { return width; }
        int getHeight() const override { return height; }

    private:
        void terminateGLES();

    private:
        EGLDisplay display = EGL_NO_DISPLAY;
        EGLSurface surface = EGL_NO_SURFACE;
        EGLContext context = EGL_NO_CONTEXT;
        bool initialized = false;
        int width = 0;
        int height = 0;
    };
} // namespace Renderer
