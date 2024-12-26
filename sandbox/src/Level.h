#pragma once
#include "Player.h"
#include "zirconium.h"
class Level {
public:
    Level();
    ~Level();

    void Init();

    void OnUpdate(const zirconium::TimeStep& delta);
    void OnRender();
    void OnEvent();

    Player& GetPlayer() {
        return m_Player;
    }

private:
    Player m_Player;
    // std::vector<Pipe> m_Pipes
};
