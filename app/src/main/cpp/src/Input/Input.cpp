#include "Input.h"

namespace Input {

    void Input::onPointerDown(int id, float x, float y) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pointers[id] = {x, y, 0.0f, 0.0f, true, false, false};
    }

    void Input::onPointerMove(int id, float x, float y) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_pointers.find(id);
        if (it != m_pointers.end()) {
            float dx = x - it->second.x;
            float dy = y - it->second.y;
            it->second.dx += dx;
            it->second.dy += dy;
            it->second.x = x;
            it->second.y = y;
            it->second.isDragging = true;
        }
    }

    void Input::onPointerUp(int id) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_pointers.find(id);
        if (it != m_pointers.end()) {
            it->second.isDown = false;
            it->second.justReleased = true;
        }
    }

    bool Input::isPointerDown(int id) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_pointers.find(id);
        return it != m_pointers.end() && it->second.isDown;
    }

    bool Input::isPointerUp(int id) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_pointers.find(id);
        if (it != m_pointers.end() && it->second.justReleased) {
            it->second.justReleased = false;
            return true;
        }
        return false;
    }

    bool Input::isPointer(int id) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_pointers.find(id) != m_pointers.end();
    }

    std::pair<float, float> Input::getPointerPosition(int id) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_pointers.find(id);
        if (it != m_pointers.end()) {
            return {it->second.x, it->second.y};
        }
        return {0.0f, 0.0f};
    }

    std::pair<float, float> Input::getPointerDelta(int id) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_pointers.find(id);
        if (it != m_pointers.end()) {
            std::pair<float, float> delta = {it->second.dx, it->second.dy};
            it->second.dx = 0.0f;
            it->second.dy = 0.0f;
            return delta;
        }
        return {0.0f, 0.0f};
    }

} // namespace Input
