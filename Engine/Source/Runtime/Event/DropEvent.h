#pragma once

#include "Event.h"

namespace sl
{

class DropEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::Drop;
    }

public:
    DropEvent(std::string_view path) : m_path(path) {}

    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_DROP;
    }

    std::string ToString() const override
    {
        return std::format("Path: {}", m_path);
    }

    std::string_view GetData() const { return m_path; }

private:
    std::string m_path;
};

} // namespace sl

SL_EVENT_FORMATTER_SPECIALIZATION(sl::DropEvent);
