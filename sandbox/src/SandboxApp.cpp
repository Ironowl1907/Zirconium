#include "../../zirconium/zirconium.h"
#include <cstdio>

class ExampleLayer : public zirconium::Layer {
public:
  ExampleLayer() : Layer("Example") {}

  void OnUpdate() override { ZR_INFO("ExampleLayer::Update"); }

  void Onevent(zirconium::Event &event) override {
    ZR_TRACE("{0}", event.ToString());
  }
};

class SandBox : public zirconium::Application {

public:
  SandBox() { PushLayer(new ExampleLayer()); }
  ~SandBox() {}
};

zirconium::Application *zirconium::CreateApplication() { return new SandBox(); }
