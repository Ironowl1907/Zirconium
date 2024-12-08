#pragma once

#include "Event.h"

namespace zirconium {

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float x, float y)
        : m_xPosition(x)
        , m_yPosition(y) {}
    inline float getXPosition() const {
        return m_xPosition;
    };
    inline float getYPosition() const {
        return m_yPosition;
    };

    std::string const ToString() const override {
        std::stringstream ss;
        ss << "MouseMovementEvent: " << m_xPosition << ' ' << m_yPosition;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
    float m_xPosition;
    float m_yPosition;
};

class MouseScrollEvent : public Event {
public:
    MouseScrollEvent(float xOffset, float yOffset)
        : m_xScrollOffset(xOffset)
        , m_yScrollOffset(yOffset) {};
    inline float getXOffset() const {
        return m_xScrollOffset;
    };
    inline float getYOffset() const {
        return m_yScrollOffset;
    };

    std::string const ToString() const override {
        std::stringstream ss;
        ss << "MouseScrollEvent: " << m_xScrollOffset << ", " << m_yScrollOffset;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
    float m_xScrollOffset;
    float m_yScrollOffset;
};

class MouseButtonEvent : public Event {
public:
    inline int GetMouseButton() {
        return m_Button;
    }

protected:
    MouseButtonEvent(int button)
        : m_Button(button) {};
    int m_Button;
};

class MousePressedButtonEvent : public MouseButtonEvent {
public:
    MousePressedButtonEvent(int button)
        : MouseButtonEvent(button) {}
    std::string const ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << m_Button;
        return ss.str();
    }
    EVENT_CLASS_TYPE(MouseButtonPressed)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
};

class MouseReleasedButtonEvent : public MouseButtonEvent {
public:
    MouseReleasedButtonEvent(int button)
        : MouseButtonEvent(button) {}
    std::string const ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << m_Button;
        return ss.str();
    }
    EVENT_CLASS_TYPE(MouseButtonReleased)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
};

} // namespace zirconium
