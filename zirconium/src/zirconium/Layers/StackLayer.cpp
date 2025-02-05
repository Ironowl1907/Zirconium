#include "StackLayer.h"
#include "zrpch.h"

namespace zirconium {
LayerStack::LayerStack() {}
LayerStack::~LayerStack() {

    ZR_PROFILE_FUNCTION();

    for (Layer* layer : m_Layers) {
        delete layer;
    }
}

void LayerStack::PushLayer(Layer* layer) {

    ZR_PROFILE_FUNCTION();

    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex++, layer);
}

void LayerStack::PushOverlay(Layer* overlay) {

    ZR_PROFILE_FUNCTION();

    m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer) {

    ZR_PROFILE_FUNCTION();

    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it == m_Layers.end())
        return;
    m_Layers.erase(it);
    --m_LayerInsertIndex;
}
void LayerStack::PopOverlay(Layer* overlay) {

    ZR_PROFILE_FUNCTION();

    auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
    if (it == m_Layers.end())
        return;
    m_Layers.erase(it);
}

} // namespace zirconium
