#include "Sandbox2D.h"
#include "zirconium.h"
#include "zirconium/Core/entryPoint.h"

#include "imgui.h"

class SandBox : public zirconium::Application {
public:
    SandBox() {
        // PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }
    ~SandBox() {}
};

zirconium::Application* zirconium::CreateApplication() {
    return new SandBox();
}
