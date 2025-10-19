#include "Renderer/Renderer.h"

#if DCONTEXT_GLES
    #include "Renderer/GLES/ContextGLES.h"
    #define CONTEXT_TYPE ContextGLES
#else
    #include "Renderer/Context.h"
    #define CONTEXT_TYPE Context
#endif

namespace Renderer {

    void Renderer::draw() {
        if (context && context->isInitialized())
            context->drawFrame();
    }

    void Renderer::destroyContext() {
        if (context)
            context->onPause();
    }

    void Renderer::createContext(void* window) {
        if (!context)
            context = std::make_unique<CONTEXT_TYPE>();

        context->onResume(window);
    }

} // namespace Renderer
