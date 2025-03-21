#pragma once

#include "Event.h"
#include "Window/MouseButtonCode.h"

namespace sl
{

class MouseButtonPressEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::MouseButtonPress;
    }

public:
    MouseButtonPressEvent(int button) : m_button(button) {}

    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_MOUSE;
    }

    std::string ToString() const override
    {
        return std::format("MouseButtonPress: {}", m_button);
    }

    int GetButton() const { return m_button; }

private:
    int m_button;
};

class MouseButtonReleaseEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::MouseButtonRelease;
    }

public:
    MouseButtonReleaseEvent(const int button) : m_button(button) {}

    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_MOUSE;
    }

    std::string ToString() const override
    {
        return std::format("MouseButtonRelease: {}", m_button);
    }

    int GetButton() const { return m_button; }

private:
    int m_button;
};

class MouseMoveEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::MouseMove;
    }

public:
    MouseMoveEvent(int x, int y) : m_posX(x), m_posY(y) {}

    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }
    
    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_MOUSE;
    }

    std::string ToString() const override
    {
        return std::format("MouseMove: {}, {}", m_posX, m_posY);
    }

    int GetPositionX() const { return m_posX; }
    int GetPositionY() const { return m_posY; }

private:
    int m_posX, m_posY;
};

class MouseScrollEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::MouseScroll;
    }

public:
    MouseScrollEvent(float x, float y) : m_offsetX(x), m_offsetY(y) {}

    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_MOUSE;
    }

    std::string ToString() const override
    {
        return std::format("MouseScroll: {}, {}", m_offsetX, m_offsetY);
    }
    
    float GetOffsetX() const { return m_offsetX; }
    float GetOffsetY() const { return m_offsetY; }

private:
    float m_offsetX, m_offsetY;
};

} // namespace sl

EVENT_FORMATTER_SPECIALIZE(sl::MouseButtonPressEvent);
EVENT_FORMATTER_SPECIALIZE(sl::MouseButtonReleaseEvent);
EVENT_FORMATTER_SPECIALIZE(sl::MouseMoveEvent);
EVENT_FORMATTER_SPECIALIZE(sl::MouseScrollEvent);
