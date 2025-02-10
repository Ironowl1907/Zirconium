#include "EditorLayer.h"
#include "zirconium.h"
#include "zirconium/Core/entryPoint.h"

#include "imgui.h"

namespace zirconium {
class ZirconiumEditor : public Application {

public:
    ZirconiumEditor()
        : Application("Zirconium Editor") {
        PushLayer(new EditorLayer());
    }
    ~ZirconiumEditor() {}
};

Application* CreateApplication(zirconium::ApplicationCommandLineArgs args) {
    return new ZirconiumEditor();
}
} // namespace zirconium
