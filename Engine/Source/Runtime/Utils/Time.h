#pragma once

#include "Core/Core.h"

#include <chrono>

namespace sl
{

class Clock final
{
public:
    Clock() : m_preTimePoint(std::chrono::steady_clock::now()), m_crtTimePoint(std::chrono::steady_clock::now()) {}

    SL_FORCEINLINE void Tick()
    {
        m_preTimePoint = m_crtTimePoint;
        m_crtTimePoint = std::chrono::steady_clock::now();
    }

    // Returns in milliseconds
    SL_FORCEINLINE float GetDeltatIme() const
    {
        auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(m_crtTimePoint - m_preTimePoint).count();
        return (float)deltaTime * 0.001f;
    }

private:
    std::chrono::steady_clock::time_point m_preTimePoint;
    std::chrono::steady_clock::time_point m_crtTimePoint;
};

class Timer final
{
public:
    Timer() : m_startTimePoint(std::chrono::steady_clock::now()) {}

    SL_FORCEINLINE void Reset()
    {
        m_startTimePoint = std::chrono::steady_clock::now();
    }

    // Returns in milliseconds
    SL_FORCEINLINE float GetDuration() const
    {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - m_startTimePoint).count();
        return (float)duration * 0.001f;
    }

private:
    std::chrono::steady_clock::time_point m_startTimePoint;
};

} // namespace sl
