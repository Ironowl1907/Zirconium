#pragma once

#include "../zirconium/window.h"
#include <memory>
namespace zirconium {

class Application {

public:
  Application();
  virtual ~Application();
  void Run();
  void onEvent(Event& event);
private:
  std::unique_ptr<Window> m_Window;
  bool m_Running = true;
};
// To be defined in a client
Application *CreateApplication();

} // namespace zirconium
