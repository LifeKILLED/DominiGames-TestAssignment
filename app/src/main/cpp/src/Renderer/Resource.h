#pragma once

namespace Renderer {

    class Resource {
    public:
        virtual ~Resource() = default;

        virtual void Load() = 0;
        virtual void Destroy() = 0;
    };

} // namespace Renderer
