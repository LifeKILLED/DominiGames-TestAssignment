#pragma once

#include "Renderer/Context.h"
#include "Util/Singleton.h"

#include <memory>

namespace Renderer {

    class Renderer : public Singleton<Renderer> {
    public:
        void createContext(void* windowData);
        void destroyContext();
        void draw();

    private:
        friend class Singleton<Renderer>;
        Renderer() = default;

        std::unique_ptr<Context> context;
    };

} // namespace Renderer
