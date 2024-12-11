#include "imgui.h"
#include "zirconium.h"

class ExampleLayer : public zirconium::Layer {
public:
    ExampleLayer()
        : Layer("Example") {}

    virtual void OnUpdate() override {}

    virtual void OnImGuiRender() override {}

    virtual void OnEvent(zirconium::Event& event) override {
        /*ZR_TRACE("{0}", event.ToString());*/
    }
};

class SandBox : public zirconium::Application {

public:
    SandBox() {
        PushLayer(new ExampleLayer());
    }
    ~SandBox() {}
};

zirconium::Application* zirconium::CreateApplication() {
    return new SandBox();
}
