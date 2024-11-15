#pragma once

namespace zirconium {

class Application {

public:
  Application();
  virtual ~Application();
  void Run();
};
// To be defined in a client
Application *CreateApplication();

} // namespace zirconium
