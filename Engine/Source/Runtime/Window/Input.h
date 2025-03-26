#pragma once

#include "Window/KeyCode.h"
#include "Window/MouseButtonCode.h"

#include <glm/vec2.hpp>

namespace sl
{

class Input final
{
public:
    Input() = delete;
    Input(const Input &) = delete;
    Input &operator=(const Input &) = delete;
    Input(Input &&) = delete;
    Input &operator=(Input &&) = delete;
    ~Input() = delete;

    static bool IsKeyPressed(uint32_t key);
    static bool IsMouseButtonPressed(uint8_t button);

    static glm::vec2 GetMousePos();
    static glm::vec2 GetMouseGlobalPos();
    static glm::vec2 GetMouseDelta();

private:
    inline static void *m_pWindow = nullptr;
};

} // namespace sl
