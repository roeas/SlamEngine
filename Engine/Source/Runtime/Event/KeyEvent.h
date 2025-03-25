#pragma once

#include "Event.h"
#include "Window/KeyCode.h"

namespace sl
{

class KeyDownEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::KeyPress;
    }

public:
    KeyDownEvent(uint32_t keycode, uint16_t modifier, bool isRepeat = false)
        : m_key(keycode), m_modifier(modifier), m_isRepeat(isRepeat) {}

    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_KEYBOARD;
    }

    std::string ToString() const override
    {
        return std::format("KeyPress: {}, modifier: {}{}", m_key, m_modifier, (m_isRepeat ? ", repeat" : ""));
    }

    uint32_t GetKey() const { return m_key; }
    uint16_t GetModifier() const{ return m_modifier; }
    bool IsRepeat() const { return m_isRepeat; }

private:
    uint32_t m_key;
    uint16_t m_modifier;
    bool m_isRepeat;
};

class KeyUpEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::KeyRelease;
    }

public:
    KeyUpEvent(uint32_t keycode) : m_key(keycode) {}

    EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    virtual uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_KEYBOARD;
    }

    std::string ToString() const override
    {
        return std::format("KeyRelease: {}", m_key);
    }

    uint32_t GetKey() const { return m_key; }

private:
    uint32_t m_key;
};

class KeyTypeEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::KeyType;
    }

public:
    KeyTypeEvent(const char *text) : m_pText(text) {}

    virtual EventType GetEventType() const override
    {
        return GetStaticEventType();
    }

    virtual uint8_t GetCategories() const override
    {
        return SL_EVENT_CATEGORY_INPUT | SL_EVENT_CATEGORY_KEYBOARD;
    }

    virtual std::string ToString() const override
    {
        return std::format("KeyType: {}", m_pText);
    }
    const char *GetKey() const { return m_pText; }

private:
    const char *m_pText;
};

} // namespace sl

EVENT_FORMATTER_SPECIALIZE(sl::KeyDownEvent);
EVENT_FORMATTER_SPECIALIZE(sl::KeyUpEvent);
EVENT_FORMATTER_SPECIALIZE(sl::KeyTypeEvent);
