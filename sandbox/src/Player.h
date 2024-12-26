#pragma once
#include "zirconium.h"
class Player {
public:
    Player();
    ~Player();
    void Init();

    void OnRender();
    void OnUpdate(const zirconium::TimeStep& delta);

    void OnImGuiRender();

private:
    glm::vec2 m_Position;
    glm::vec2 m_Velocity;
    glm::vec2 m_Accel;

    zirconium::Ref<zirconium::Texture2D> m_Sprite;
    uint32_t m_Score;

    float m_JumpCooldown = 0;
};
