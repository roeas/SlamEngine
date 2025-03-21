#pragma once

#include "Event.h"
#include "Window/KeyCode.h"

namespace sl
{

class KeyPressEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::KeyPress;
    }

public:
    KeyPressEvent(int keycode, bool isRepeat = false)
        : m_key(keycode), m_isRepeat(isRepeat) {}

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
        return std::format("KeyPress: {}{}", m_key, (m_isRepeat ? ", repeat" : ""));
    }

    int GetKey() const { return m_key; }
    bool IsRepeat() const { return m_isRepeat; }

private:
    int m_key;
    bool m_isRepeat;
};

class KeyReleaseEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::KeyRelease;
    }

public:
    KeyReleaseEvent(int keycode) : m_key(keycode) {}

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

    int GetKey() const { return m_key; }

private:
    int m_key;
};

class KeyTypeEvent : public Event
{
public:
    static EventType GetStaticEventType()
    {
        return EventType::KeyType;
    }

public:
    KeyTypeEvent(int keycode) : m_key(keycode) {}

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
        return std::format("KeyType: {}", m_key);
    }
    int GetKey() const { return m_key; }

private:
    int m_key;
};

} // namespace sl

EVENT_FORMATTER_SPECIALIZE(sl::KeyPressEvent);
EVENT_FORMATTER_SPECIALIZE(sl::KeyReleaseEvent);
EVENT_FORMATTER_SPECIALIZE(sl::KeyTypeEvent);
