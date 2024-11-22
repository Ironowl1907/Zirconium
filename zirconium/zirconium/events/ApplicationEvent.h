#pragma once

#include "../../zrpch.h"
#include "Event.h"

namespace zirconium {

class WindowResizeEvent : public Event {
public:
  WindowResizeEvent(unsigned int width, unsigned int height);

  unsigned int GetWidth() const;
  unsigned int GetHeight() const;

  const std::string ToString() const override;

  EVENT_CLASS_TYPE(WindowResize);
  EVENT_CLASS_CATEGORY(EventCategoryApplication);

private:
  unsigned int m_Width, m_Height;
};

class WindowCloseEvent : public Event {
public:
  WindowCloseEvent();

  EVENT_CLASS_TYPE(WindowClose);
  EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

class AppTickEvent : public Event {
public:
  AppTickEvent();

  EVENT_CLASS_TYPE(AppTick);
  EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

class AppUpdateEvent : public Event {
public:
  AppUpdateEvent();

  EVENT_CLASS_TYPE(AppUpdate);
  EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

class AppRender : public Event {
public:
  AppRender();

  EVENT_CLASS_TYPE(AppRender);
  EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

} // namespace zirconium
