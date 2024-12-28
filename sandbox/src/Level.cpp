#include "Level.h"
#include "zirconium.h"

Level::Level()
    : m_Pipes(5) {
    m_Player.Init();
}
Level::~Level() {}

void Level::Init() {
  m_Pipes.GenPipe({{0.0f,0.0f}});
}
void Level::OnUpdate(const zirconium::TimeStep& delta) {
    m_Player.OnUpdate(delta);
    m_Pipes.OnUpdate(delta);
}
void Level::OnRender() {
    m_Player.OnRender();
    m_Pipes.OnRender();
}

void Level::OnEvent() {}
