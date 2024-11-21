#pragma once
#include "../window.h"
#include "../../vendor/glfw/include/GLFW/glfw3.h"

namespace zirconium {

class LinuxWindow : public Window {
public:
  LinuxWindow(const WindowProps &props);
  virtual ~LinuxWindow();

  void SetEventCallback(const EventCallbackFn &callback) override;

  void onUpdate() override;

  inline unsigned int GetWidth() const override { return m_windowData.Width; }
  inline unsigned int GetHeight() const override { return m_windowData.Height; }

  void SetVSync(bool enbabled) override;
  bool IsVSyncOn() const override;

protected:
  virtual void init(const WindowProps &props);
  virtual void shutdown();

  GLFWwindow *m_window;

  struct WindowData {
    std::string Title;
    unsigned int Width, Height;
    bool VSync;

    EventCallbackFn EventCallback;
  };
  WindowData m_windowData;
};

} // namespace zirconium
