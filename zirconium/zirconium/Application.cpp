#include "Application.h"
#include "../zrpch.h"
#include "../core.h"

#include "../zirconium/events/ApplicationEvent.cpp"
#include "events/Event.h"
#include "log.h"

namespace zirconium {

Application::Application() {}
Application::~Application() {}

void Application::Run() {
  WindowResizeEvent e(1920, 1080);
  if (e.IsInCategory(EventCategoryApplication))
    ZR_TRACE(e.ToString());
  if (e.IsInCategory(EventCategoryInput))
    ZR_TRACE(e.ToString());
  ZR_ASSERT(1>2, "Generic Error");
}

} // namespace zirconium
