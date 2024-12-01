#include "zirconium.h"


class ExampleLayer : public zirconium::Layer {
public:
  ExampleLayer() : Layer("Example") {}

  void OnUpdate() override {
    if (zirconium::Input::IsKeyPressed(ZR_KEY_TAB))
      ZR_TRACE("Tab Key Pressed");
  }

  void OnEvent(zirconium::Event &event) override {
    /*ZR_TRACE("{0}", event.ToString());*/
  }
};

class SandBox : public zirconium::Application {

public:
  SandBox() {
    PushLayer(new ExampleLayer());
    PushOverlay(new zirconium::ImGuiLayer());
  }
  ~SandBox() {}
};

zirconium::Application *zirconium::CreateApplication() { return new SandBox(); }
