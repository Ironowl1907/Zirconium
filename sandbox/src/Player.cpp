#include "Player.h"
#include "zirconium.h"

#include "imgui.h"

Player::Player() {}
Player::~Player() {}

void Player::Init() {
    m_Sprite = zirconium::Texture2D::Create("/home/ironowl/dev/Zirconium/sandbox/res/textures/Flappy.png");
    m_Position = glm::vec2(-10.0f, 0.0f);
    m_Velocity = glm::vec2(0.0f);
    m_Accel = glm::vec2(0.0, 0.2f);

    m_Score = 0;
}

void Player::OnRender() {
    ZR_CORE_ASSERT(m_Sprite, "Texture is null!");
    zirconium::Renderer2D::DrawRotatedTexQuad(m_Position, {5.0f, 5.0f}, 0.0f, m_Sprite);
}
void Player::OnUpdate(const zirconium::TimeStep& delta) {
    if (zirconium::Input::IsKeyPressed(ZR_KEY_SPACE) && m_JumpCooldown <= 0.0f) {
        m_Velocity = glm::vec2(0.0f, -0.2f);
        m_JumpCooldown = 1.0f;
    }

    m_JumpCooldown -= (float)delta;
    m_Velocity += m_Accel * (float)delta;
    m_Position += m_Velocity;
}

void Player::OnImGuiRender() {
    ImGui::Begin("Player Debug");

    // Display the position
    ImGui::Text("Position");
    ImGui::InputFloat2("Position", glm::value_ptr(m_Position));

    // Display the velocity
    ImGui::Text("Velocity");
    ImGui::InputFloat2("Velocity", glm::value_ptr(m_Velocity));

    // Display the acceleration
    ImGui::Text("Acceleration");
    ImGui::InputFloat2("Acceleration", glm::value_ptr(m_Accel));

    ImGui::End();
}
