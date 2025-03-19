#pragma once

#include "Event.h"

namespace sl
{

class SceneViewportResizeEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::SceneViewportResize;
    }

public:
    SceneViewportResizeEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }
    
    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_SCENE_VIEWPORT;
    }

    std::string ToString() const override
    {
        return std::format("SceneViewportResize: {}, {}", m_width, m_height);
    }

    uint32_t GetWidth() const { return m_width; }
    uint32_t GetHeight() const { return m_height; }

private:
    uint32_t m_width, m_height;
};

class SceneViewportGetFocusEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::SceneViewportGetFocus;
    }

public:
    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_SCENE_VIEWPORT;
    }

    std::string ToString() const override
    {
        return "SceneViewportFocus";
    }
};

class SceneViewportLostFocusEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::SceneViewportLostFocus;
    }

public:
    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_SCENE_VIEWPORT;
    }

    std::string ToString() const override
    {
        return "SceneViewportLostFocus";
    }
};

class SceneViewportHoverEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::SceneViewportHover;
    }

public:
    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_SCENE_VIEWPORT;
    }

    std::string ToString() const override
    {
        return "SceneViewportHover";
    }
};

} // namespace sl
