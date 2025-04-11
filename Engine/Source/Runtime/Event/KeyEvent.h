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
        return EventType::KeyDown;
    }

public:
    KeyDownEvent(KeyCodeType keycode, KeyModifierType modifier, bool isRepeat = false)
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
        return std::format("KeyDown: {}, modifier: {}{}", m_key, m_modifier, (m_isRepeat ? ", repeat" : ""));
    }

    KeyCodeType GetKey() const { return m_key; }
    KeyModifierType GetModifier() const{ return m_modifier; }
    bool IsRepeat() const { return m_isRepeat; }

private:
    KeyCodeType m_key;
    KeyModifierType m_modifier;
    bool m_isRepeat;
};

class KeyUpEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::KeyUp;
    }

public:
    KeyUpEvent(KeyCodeType keycode) : m_key(keycode) {}

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
        return std::format("KeyUp: {}", m_key);
    }

    KeyCodeType GetKey() const { return m_key; }

private:
    KeyCodeType m_key;
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
    const char *GetText() const { return m_pText; }

private:
    const char *m_pText;
};

} // namespace sl

SL_EVENT_FORMATTER_SPECIALIZATION(sl::KeyDownEvent);
SL_EVENT_FORMATTER_SPECIALIZATION(sl::KeyUpEvent);
SL_EVENT_FORMATTER_SPECIALIZATION(sl::KeyTypeEvent);
