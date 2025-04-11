#pragma once

#include "Event.h"

namespace sl
{

class WindowResizeEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::WindowResize;
    }

public:
    WindowResizeEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_WINDOW;
    }

    std::string ToString() const override
    {
        return std::format("WindowResize: {}, {}", m_width, m_height);
    }

    uint32_t GetWidth() const { return m_width; }
    uint32_t GetHeight() const { return m_height; }

private:
    uint32_t m_width, m_height;
};

class WindowMinimizeEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::WindowMinimize;
    }

public:
    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_WINDOW;
    }

    std::string ToString() const override
    {
        return "WindowMinimize";
    }
};

class WindowMaximizeEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::WindowMaximize;
    }

public:
    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_WINDOW;
    }

    std::string ToString() const override
    {
        return "WindowMaximize";
    }
};

class WindowRestoreEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::WindowRestore;
    }

public:
    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_WINDOW;
    }

    std::string ToString() const override
    {
        return "WindowRestore";
    }
};

class WindowGetFocusEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::WindowGetFocus;
    }

public:
    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_WINDOW;
    }

    std::string ToString() const override
    {
        return "WindowGetFocus";
    }
};

class WindowLossFocusEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::WindowLostFocus;
    }

public:
    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_WINDOW;
    }

    std::string ToString() const override
    {
        return "WindowLossFocus";
    }
};

class WindowCloseEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::WindowClose;
    }

public:
    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_WINDOW;
    }

    std::string ToString() const override
    {
        return "WindowClose";
    }
};

} // namespace sl

SL_EVENT_FORMATTER_SPECIALIZATION(sl::WindowResizeEvent);
SL_EVENT_FORMATTER_SPECIALIZATION(sl::WindowMinimizeEvent);
SL_EVENT_FORMATTER_SPECIALIZATION(sl::WindowMaximizeEvent);
SL_EVENT_FORMATTER_SPECIALIZATION(sl::WindowRestoreEvent);
SL_EVENT_FORMATTER_SPECIALIZATION(sl::WindowGetFocusEvent);
SL_EVENT_FORMATTER_SPECIALIZATION(sl::WindowLossFocusEvent);
SL_EVENT_FORMATTER_SPECIALIZATION(sl::WindowCloseEvent);
