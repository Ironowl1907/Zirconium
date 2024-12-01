#include "zirconium.h"
#include <cstdio>

class ExampleLayer : public zirconium::Layer {
public:
  ExampleLayer() : Layer("Example") {}

  void OnUpdate() override {}

  void OnEvent(zirconium::Event &event) override {
    ZR_TRACE("{0}", event.ToString());
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
