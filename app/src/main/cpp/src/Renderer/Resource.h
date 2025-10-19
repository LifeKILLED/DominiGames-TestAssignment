#pragma once

namespace Renderer {

    class Resource {
    public:
        virtual ~Resource() = default;

        virtual void Load() = 0;
        virtual void Unload() = 0;
    };

} // namespace Renderer
