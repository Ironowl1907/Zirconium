#include "Pipe.h"

Pipe(glm::vec2 position, bool alive)
    : m_Position(position)
    , m_Alive(alive) {}

Pipe::~Pipe() {}

void Pipe::OnUpdate(const zirconium::TimeStep& delta) {}

void Pipe::OnRender() {
    ZR_TRACE("Rendering Pipe");
    zirconium::Renderer2D::DrawQuad(m_Position, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f});
}

PipeContainer::PipeContainer(uint32_t size)
    : m_ContainerSize(size)
    , m_ContainerIndex(0) {
    m_Container.resize(size, {{0, 0}, false});
}

PipeContainer::~PipeContainer() {}

void PipeContainer::Init() {}

std::vector<Pipe>::iterator PipeContainer::begin() {
    return m_Container.begin();
}

std::vector<Pipe>::iterator PipeContainer::end() {
    return m_Container.end();
}

std::vector<Pipe>::const_iterator PipeContainer::begin() const {
    return m_Container.begin();
}

std::vector<Pipe>::const_iterator PipeContainer::end() const {
    return m_Container.end();
}

void PipeContainer::OnUpdate(const zirconium::TimeStep& delta) {
    ZR_TRACE("PipeContatiner::update");
    ZR_TRACE(m_Container.size());
    for (auto pipe : m_Container) {
        ZR_TRACE(pipe.ToString());
        pipe.OnUpdate(delta);
    }
}
void PipeContainer::OnRender() {
    for (auto pipe : m_Container) {
        pipe.OnRender();
    }
}

void PipeContainer::GenPipe(const Pipe pipe) {
  m_Container[m_ContainerIndex++ % m_ContainerSize] = pipe;
}
