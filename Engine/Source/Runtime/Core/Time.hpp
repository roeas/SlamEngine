#pragma once

#include <chrono>

namespace sl
{

class Clock final
{
public:
    void Tick()
    {
        m_preTimePoint = m_crtTimePoint;
        m_crtTimePoint = std::chrono::steady_clock::now();
    }

    // Returns in milliseconds
    float GetDeltatIme() const
    {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(m_crtTimePoint - m_preTimePoint).count();
        return (float)duration * 0.001f;
    }

private:
    std::chrono::steady_clock::time_point m_preTimePoint;
    std::chrono::steady_clock::time_point m_crtTimePoint;
};

class Timer final
{
public:
    Timer() : m_startTimePoint(std::chrono::steady_clock::now()) {}

    void Reset()
    {
        m_startTimePoint = std::chrono::steady_clock::now();
    }

    // Returns in milliseconds
    float GetDuration() const
    {
        auto crtTimePoint = std::chrono::steady_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(crtTimePoint - m_startTimePoint).count();

        return (float)deltaTime * 0.001f;
    }

private:
    std::chrono::steady_clock::time_point m_startTimePoint;
};

} // namespace sl
