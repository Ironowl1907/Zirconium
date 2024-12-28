#pragma once
#include "Pipe.h"
#include "Player.h"
// std::vector<Pipe> m_Pipes
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
    PipeContainer m_Pipes;
};
