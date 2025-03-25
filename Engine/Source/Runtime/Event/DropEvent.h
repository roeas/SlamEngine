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
    DropEvent(const char *pPath) : m_path(pPath) {}

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
        return std::format("Data: {}", m_path);
    }

    std::string_view GetData() const { return m_path; }

private:
    std::string m_path;
};

} // namespace sl

EVENT_FORMATTER_SPECIALIZE(sl::DropEvent);
