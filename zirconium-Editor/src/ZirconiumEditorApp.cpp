#include "EditorLayer.h"
#include "zirconium.h"
#include "zirconium/Core/entryPoint.h"

#include "imgui/imgui.h"

namespace zirconium {
class ZirconiumEditor : public Application {

public:
    ZirconiumEditor()
        : Application("Zirconium Editor") {
        PushLayer(new EditorLayer());
    }
    ~ZirconiumEditor() {}
};

Application* CreateApplication() {
    return new ZirconiumEditor();
}
} // namespace zirconium
