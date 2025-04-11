#include "Input.h"

#include <SDL3/SDL.h>

namespace sl
{

bool Input::IsKeyDown(KeyCodeType key)
{
    const auto *pStates = SDL_GetKeyboardState(nullptr);
    return pStates[(size_t)key];
}

bool Input::IsKeyModifierDown(KeyModifierType mod)
{
    auto mods = SDL_GetModState();
    return mods & mod;
}

bool Input::IsMouseButtonDown(MouseButtonType button)
{
    auto state = SDL_GetMouseState(nullptr, nullptr);
    return state & SL_MOUSE_BUTTON_MASK(button);
}

glm::vec2 Input::GetMousePos()
{
    float posX, posY;
    SDL_GetMouseState(&posX, &posY);
    return glm::vec2{ posX, posY };
}

glm::vec2 Input::GetMouseGlobalPos()
{
    float posX, posY;
    SDL_GetGlobalMouseState(&posX, &posY);
    return glm::vec2{ posX, posY };
}

glm::vec2 Input::GetMouseDelta()
{
    float deltaX, deltaY;
    SDL_GetRelativeMouseState(&deltaX, &deltaY);
    return glm::vec2{ deltaX, deltaY };
}

void Input::SetMousePos(void *pWindow, glm::vec2 pos)
{
    SDL_WarpMouseInWindow(static_cast<SDL_Window *>(pWindow), pos.x, pos.y);
}

bool Input::SetMouseGlobalPos(glm::vec2 pos)
{
    return SDL_WarpMouseGlobal(pos.x, pos.y);
}

bool Input::SetMouseRelativeMode(void *pWindow, bool enable, bool restoreMousePos)
{
    static float s_posX, s_posY;

    SDL_Window *pSDLWindow = static_cast<SDL_Window *>(pWindow);
    bool isInRelativeMode = SDL_GetWindowRelativeMouseMode(pSDLWindow);

    if (enable && !isInRelativeMode)
    {
        SDL_GetMouseState(&s_posX, &s_posY);
    }
    else if(!enable && isInRelativeMode && restoreMousePos)
    {
        SDL_WarpMouseInWindow(pSDLWindow, s_posX, s_posY);
    }

    return SDL_SetWindowRelativeMouseMode(pSDLWindow, enable);
}

} // namespace sl
