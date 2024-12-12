#pragma once

#include "events/Event.h"
#include "Timestep.h"

namespace zirconium {

class Layer {
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(TimeStep delta) {}
    virtual void OnImGuiRender() {}
    virtual void OnEvent([[maybe_unused]] Event& event) {}

    inline const std::string GetName() const {
        return m_DName;
    }

protected:
    std::string m_DName;
};

} // namespace zirconium
