#pragma once

#include "Event.h"

namespace zirconium {

class KeyEvent : public Event {
public:
  inline int GetKeyCode() const { return m_KeyCode; }

protected:
  KeyEvent(int key) : m_KeyCode(key) {}
  int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
  KeyPressedEvent(int keyCode, int repeatedCount)
      : KeyEvent(keyCode), m_RepeatCount(repeatedCount) {}

  inline int GetRepeatedCount() const { return m_RepeatCount; }
  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount
       << " repeats)";
    return ss.str();
  }
  EVENT_CLASS_TYPE(KeyPressed);
  EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

private:
  int m_RepeatCount;
};

}; // namespace zirconium
