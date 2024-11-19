#pragma once

#include "../../zrpch.h"
#include "../events/Event.h"

namespace zirconium {

struct WindowProps {
  WindowProps(std::string title = "Zirconium Engine", unsigned int width = 1280,
              unsigned int height = 720)
      : Title(title), Width(width), Height(height) {}

  unsigned int Width, Height;
  std::string Title;
};

class Window {
public:
  using EventCallback = std::function<void(Event &)>();
  virtual ~Window();

  virtual void onUpdate() = 0;
  virtual unsigned int GetWidth() const = 0;
  virtual unsigned int GetHeight() const = 0;
  virtual void SetVSync(bool enbabled) = 0;

  static Window *Create(const WindowProps &props = WindowProps());
};
} // namespace zirconium
