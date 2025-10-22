#pragma once

#include <unordered_map>
#include <mutex>

namespace Input {

    struct PointerInfo {
        float x = 0.0f;
        float y = 0.0f;
        float dx = 0.0f;
        float dy = 0.0f;
        bool isDown = false;
        bool justReleased = false;
        bool isDragging = false;
    };

    class Input {
    public:
        static Input& get() {
            static Input instance;
            return instance;
        }

        void onPointerDown(int id, float x, float y);
        void onPointerMove(int id, float x, float y);
        void onPointerUp(int id);

        bool isPointerDown(int id);
        bool isPointerUp(int id);
        bool isPointer(int id);
        std::pair<float, float> getPointerPosition(int id);
        std::pair<float, float> getPointerDelta(int id);

    private:
        Input() = default;
        ~Input() = default;

    private:
        std::unordered_map<int, PointerInfo> m_pointers;
        std::mutex m_mutex;
    };

} // namespace Input
