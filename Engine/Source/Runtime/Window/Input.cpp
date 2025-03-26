#include "Input.h"

#include <SDL3/SDL.h>

namespace sl
{

bool Input::IsKeyPressed(uint32_t key)
{
    const auto *pStates = SDL_GetKeyboardState(nullptr);
    return pStates[static_cast<size_t>(key)];
}

bool Input::IsMouseButtonPressed(uint8_t button)
{
    uint32_t state = SDL_GetMouseState(nullptr, nullptr);
    return state & (1 << (button - 1));
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

} // namespace sl
