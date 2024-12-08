#pragma once
#include "Layer.h"
#include <vector>

namespace zirconium {

class LayerStack {
    using LayerVector = std::vector<Layer*>;

public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    std::vector<Layer*>::iterator begin() {
        return m_Layers.begin();
    }
    std::vector<Layer*>::iterator end() {
        return m_Layers.end();
    }

private:
    LayerVector m_Layers;
    unsigned int m_LayerInsertIndex = 0;
};
} // namespace zirconium
