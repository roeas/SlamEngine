#pragma once

#include <concepts>
#include <cstdint>
#include <format>
#include <functional>

#define SL_EVENT_CATEGORY_NONE           UINT8_C(0x00)
#define SL_EVENT_CATEGORY_WINDOW         UINT8_C(0x01)
#define SL_EVENT_CATEGORY_INPUT          UINT8_C(0x02)
#define SL_EVENT_CATEGORY_KEYBOARD       UINT8_C(0x04)
#define SL_EVENT_CATEGORY_MOUSE          UINT8_C(0x08)
#define SL_EVENT_CATEGORY_DROP           UINT8_C(0x10)
#define SL_EVENT_CATEGORY_SCENE_VIEWPORT UINT8_C(0x11)

#define SL_EVENT_CATEGORY_ALL ( \
        SL_EVENT_CATEGORY_WINDOW | \
        SL_EVENT_CATEGORY_INPUT | \
        SL_EVENT_CATEGORY_KEYBOARD | \
        SL_EVENT_CATEGORY_MOUSE | \
        SL_EVENT_CATEGORY_DROP | \
        SL_EVENT_CATEGORY_SCENE_VIEWPORT)

#define BIND_EVENT_CALLBACK(fun) std::bind_front(&fun, this)

// Use it in the global namespace
#define EVENT_FORMATTER_SPECIALIZE(event) \
template <> \
struct std::formatter<event> : std::formatter<std::string> \
{ \
    auto format(const event &e, format_context &ctx) const \
    { \
        return formatter<string>::format(e.ToString(), ctx); \
    } \
}

namespace sl
{

enum class EventType
{
    None = 0,
    WindowResize, WindowMinimize, WindowMaximize, WindowRestore,
    WindowGetFocus, WindowLostFocus, WindowClose,
    KeyPress, KeyRelease, KeyType,
    MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll,
    Drop,
    SceneViewportResize, SceneViewportGetFocus, SceneViewportLostFocus, SceneViewportHover,
};

class Event
{
public:
    virtual ~Event() = default;

    virtual EventType GetEventType() const = 0;
    virtual uint8_t GetCategories() const = 0;
    virtual std::string ToString() const = 0;

    bool IsInCategory(uint8_t cat) const{ return cat & GetCategories(); }

    void SetHandled(bool handled) { m_handled = handled; }
    bool IsHandled() const { return m_handled; }

private:
    bool m_handled = false;
};

template<typename T, typename Fun>
concept DispatchableDerivedEvent = requires(T e, Fun fun)
{
    requires std::is_base_of_v<Event, T>;

    { e.GetEventType() } -> std::same_as<EventType>;
    { T::GetStaticEventType() } -> std::same_as<EventType>;

    { e.IsHandled() } -> std::same_as<bool>;
    { fun(e) } -> std::same_as<bool>;
};

class EventDispatcher
{
public:
    EventDispatcher(Event &event) : m_event(event) {}

    template<typename T, typename Fun> requires DispatchableDerivedEvent<T, Fun>
    bool Dispatch(Fun fun)
    {
        // Call #fun if type of #m_event is #T
        if (m_event.GetEventType() == T::GetStaticEventType())
        {
            m_event.SetHandled(fun(static_cast<T &>(m_event)));
            return true;
        }
        return false;
    }

private:
    Event &m_event;
};

using EventCallback = std::function<void(Event &)>;

} // namespace sl

EVENT_FORMATTER_SPECIALIZE(sl::Event);
