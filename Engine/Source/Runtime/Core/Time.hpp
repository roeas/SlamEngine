#pragma once

#include <chrono>

namespace sl
{

class Clock final
{
public:
    void Tick()
    {
        auto crtTimePoint = std::chrono::steady_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(crtTimePoint - m_lastTimePoint).count();

        m_deltaTime = (float)deltaTime * 0.001f;
        m_lastTimePoint = crtTimePoint;
    }

    // Returns in milliseconds
    float GetDeltatIme() const { return m_deltaTime; }

private:
    float m_deltaTime = 0.0f;
    std::chrono::steady_clock::time_point m_lastTimePoint;
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
