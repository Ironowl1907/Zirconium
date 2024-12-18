#include "ApplicationEvent.h"

namespace zirconium {

// Implementation of WindowResizeEvent
WindowResizeEvent::WindowResizeEvent(unsigned int width, unsigned int height)
    : m_Width(width), m_Height(height) {}

unsigned int WindowResizeEvent::GetWidth() const { return m_Width; }

unsigned int WindowResizeEvent::GetHeight() const { return m_Height; }

const std::string WindowResizeEvent::ToString() const {
  std::stringstream ss;
  ss << "WindowResizeEvent: Width: " << m_Width << ", Height: " << m_Height;
  return ss.str();
}

WindowCloseEvent::WindowCloseEvent() {}

AppTickEvent::AppTickEvent() {}

AppUpdateEvent::AppUpdateEvent() {}

AppRender::AppRender() {}

} // namespace zirconium
