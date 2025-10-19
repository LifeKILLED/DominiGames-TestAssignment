#include "Renderer/GLES/ContextGLES.h"

#include "Scene/Scene.h" // TODO: remove

#include <android/native_window.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "NativeGL", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "NativeGL", __VA_ARGS__))

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <android/log.h>

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

    void ContextGLES::beginFrame() {
        if (!m_initialized)
            return;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        glClearDepthf(1.0f);

        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        DrawTestTriangle();
    }

    void ContextGLES::endFrame() {
        if (!m_initialized)
            return;

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

    void ContextGLES::DrawTestTriangle()
    {
        // --- Вершины (позиция + цвет) ---
        GLfloat vertices[] = {
                //    x      y     z      r     g     b     a
                0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f, // верх
                -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f, // левый
                0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f  // правый
        };

        // --- Шейдеры ---
        const char* vertSrc =
                "attribute vec3 aPos;"
                "attribute vec4 aColor;"
                "uniform mat4 uTransform;"
                "uniform mat4 uProjection;"
                "varying vec4 vColor;"
                "void main() {"
                "   gl_Position = uProjection * uTransform * vec4(aPos, 1.0);"
                "   vColor = aColor;"
                "}";

        const char* fragSrc =
                "precision mediump float;"
                "varying vec4 vColor;"
                "void main() {"
                "   gl_FragColor = vColor;"
                "}";

        // --- Компиляция шейдеров ---
        auto compileShader = [](GLenum type, const char* src) -> GLuint {
            GLuint shader = glCreateShader(type);
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                char info[512];
                glGetShaderInfoLog(shader, 512, nullptr, info);
                LOGE("Shader compile error: %s", info);
            }
            return shader;
        };

        GLuint vs = compileShader(GL_VERTEX_SHADER, vertSrc);
        GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragSrc);

        // --- Линковка программы ---
        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glBindAttribLocation(program, 0, "aPos");
        glBindAttribLocation(program, 1, "aColor");
        glLinkProgram(program);

        GLint linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked) {
            char info[512];
            glGetProgramInfoLog(program, 512, nullptr, info);
            LOGE("Program link error: %s", info);
        }

        glUseProgram(program);

        // --- Матрицы ---
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -2.0f));

        glm::mat4 projection = glm::perspective(
                glm::radians(60.0f),
                1.0f,     // соотношение сторон (просто 1:1 для теста)
                0.1f,
                10.0f
        );

        GLint uTransform = glGetUniformLocation(program, "uTransform");
        GLint uProjection = glGetUniformLocation(program, "uProjection");
        glUniformMatrix4fv(uTransform, 1, GL_FALSE, &transform[0][0]);
        glUniformMatrix4fv(uProjection, 1, GL_FALSE, &projection[0][0]);

        // --- Создаём и заполняем буфер ---
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // --- Включаем атрибуты ---
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

        // --- Настройки рендера ---
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- Рисуем треугольник ---
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // --- Очистка ---
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vbo);

        glDeleteShader(vs);
        glDeleteShader(fs);
        glDeleteProgram(program);
    }

} // namespace Renderer
