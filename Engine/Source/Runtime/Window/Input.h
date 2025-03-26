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

    static bool IsKeyDown(KeyCodeType key);
    static bool IsKeyModifierDown(KeyModifierType mod);
    static bool IsMouseButtonDown(MouseButtonType button);

    static glm::vec2 GetMousePos();
    static glm::vec2 GetMouseGlobalPos();
    static glm::vec2 GetMouseDelta();

    static void SetMousePos(void *pWindow, glm::vec2 pos);
    static bool SetMouseGlobalPos(glm::vec2 pos);

    static bool SetMouseRelativeMode(void *pWindow, bool enable, bool restoreMousePos = true);

private:
    inline static void *m_pWindow = nullptr;
};

} // namespace sl
