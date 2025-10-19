#pragma once

namespace Renderer {

        class Context {
            public:
            virtual ~Context() = default;

            virtual void initialize(void* windowData) = 0;
            virtual void beginFrame() = 0;
            virtual void endFrame() = 0;
            virtual void terminate() = 0;

            virtual void onPause() = 0;
            virtual void onResume(void* windowData) = 0;

            virtual bool isInitialized() const = 0;
            virtual bool destroyResourcesWithContext() const = 0;

            [[maybe_unused]] virtual int getWidth() const = 0;
            [[maybe_unused]] virtual int getHeight() const = 0;
        };

} // namespace Renderer
