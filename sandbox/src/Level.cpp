#include "Level.h"
#include "zirconium.h"

Level::Level() {
    m_Player.Init();
}
Level::~Level() {}

void Level::Init() {}
void Level::OnUpdate(const zirconium::TimeStep& delta) {
    m_Player.OnUpdate(delta);
}
void Level::OnRender() {
    m_Player.OnRender();
}
void Level::OnEvent() {}
