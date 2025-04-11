#pragma once

#include "Event.h"
#include "Window/MouseButtonCode.h"

namespace sl
{

class MouseMoveEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::MouseMove;
    }

public:
    MouseMoveEvent(float x, float y) : m_posX(x), m_posY(y) {}

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

    float GetPositionX() const { return m_posX; }
    float GetPositionY() const { return m_posY; }

private:
    float m_posX, m_posY;
};

class MouseButtonDownEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::MouseButtonDown;
    }

public:
    MouseButtonDownEvent(MouseButtonType button, bool doubleClick) :
        m_button(button), m_isDoubleClick(doubleClick) {}

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
        return std::format("MouseButtonDown: {}{}", m_button, (m_isDoubleClick ? ", double click" : ""));
    }

    MouseButtonType GetButton() const { return m_button; }
    bool IsDoubleClick() const { return m_isDoubleClick; }

private:
    MouseButtonType m_button;
    bool m_isDoubleClick;
};

class MouseButtonUpEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::MouseButtonUp;
    }

public:
    MouseButtonUpEvent(MouseButtonType button) : m_button(button) {}

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
        return std::format("MouseButtonUp: {}", m_button);
    }

    MouseButtonType GetButton() const { return m_button; }

private:
    MouseButtonType m_button;
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

SL_EVENT_FORMATTER_SPECIALIZE(sl::MouseMoveEvent);
SL_EVENT_FORMATTER_SPECIALIZE(sl::MouseButtonDownEvent);
SL_EVENT_FORMATTER_SPECIALIZE(sl::MouseButtonUpEvent);
SL_EVENT_FORMATTER_SPECIALIZE(sl::MouseScrollEvent);
