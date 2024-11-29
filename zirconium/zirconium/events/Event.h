#pragma once

#include "core.h"
#include <functional>
#include <string>

namespace zirconium {
// This will be a blocking event handler, it will halt the main execution
// until the funciton that manages the event is done.
// TODO: Maybe in the future bufffer this event and only process them during the
// event managment time.
//

// clang-format off
enum class EventType {
  None = 0,
  WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
  AppTick, AppUpdate, AppRender, 
  KeyPressed, KeyReleased, KeyTyped,
  MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
};

enum EventCategory {
  None = 0, 
  EventCategoryApplication =    BIT(0),
  EventCategoryInput =          BIT(1),
  EventCategoryKeyboard =       BIT(2),
  EventCategoryMouse  =         BIT(3),
  EventCategoryMouseButton =    BIT(4),
};
// clang-format on

class Event {
public:
  virtual EventType GetEventType() const = 0;
  virtual const char *GetName() const = 0;
  virtual int GetCategoryFlags() const = 0;
  virtual const std::string ToString() const { return GetName(); };

  inline bool IsInCategory(EventCategory category) {
    return GetCategoryFlags() & category;
  }

public:
  bool Handled = false;
};

class EventDispatcher {
  template <typename T> using EventFn = std::function<bool(T &)>;

public:
  EventDispatcher(Event &event) : m_Event(event) {}

  template <typename T> bool Dispatch(EventFn<T> func) {
    if (m_Event.GetEventType() == T::GetStaticType()) {
      m_Event.Handled = func(*(T *)&m_Event);
      return true;
    }
    return false;
  }

private:
  Event &m_Event;
};

inline std::ostream &operator<<(std::ostream &os, const Event &event) {
  return os << event.ToString();
}

} // namespace zirconium

#define EVENT_CLASS_TYPE(type)                                                 \
  static EventType GetStaticType() { return EventType::type; }                 \
  EventType GetEventType() const override { return GetStaticType(); }          \
  const char *GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)                                         \
  virtual int GetCategoryFlags() const override { return category; }
