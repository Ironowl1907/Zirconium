#pragma once
#include "zirconium.h"
#include <cstdint>
#include <iomanip> // For fixed and setprecision

struct ParticleProps {
    glm::vec2 Position;
    glm::vec2 Velocity, Aceleration;
    glm::vec4 ColorBegin, ColorEnd;
    float SizeBegin, SizeEnd, SizeVariation;
    float LifeTime = 1.0f;

    void Log() const {
        std::cout << std::fixed << std::setprecision(2) << "Position: (" << Position.x << ", " << Position.y << "), "
                  << "Velocity: (" << Velocity.x << ", " << Velocity.y << "), "
                  << "Aceleration: (" << Aceleration.x << ", " << Aceleration.y << "), "
                  << "ColorBegin: (" << ColorBegin.r << ", " << ColorBegin.g << ", " << ColorBegin.b << ", "
                  << ColorBegin.a << "), "
                  << "ColorEnd: (" << ColorEnd.r << ", " << ColorEnd.g << ", " << ColorEnd.b << ", " << ColorEnd.a
                  << "), "
                  << "SizeBegin: " << SizeBegin << ", "
                  << "SizeEnd: " << SizeEnd << ", "
                  << "SizeVariation: " << SizeVariation << ", "
                  << "LifeTime: " << LifeTime << std::endl;
    }
};

class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem();

    void OnUpdate(zirconium::TimeStep delta);
    void OnRender(zirconium::OrthoCamera& camera);

    void Emit(const ParticleProps& particleProp);

private:
    struct Particle {
        glm::vec2 Position = {0, 0};
        glm::vec2 Velocity = {0, 0};
        glm::vec4 ColorBegin, ColorEnd;
        float Rotation = 0.0f;
        float SizeBegin = 0.5f, SizeEnd = 0.0f;

        float Lifetime = 5.0f;
        float LifeRemaining = 0.0f;

        bool Active = false;

        void Log() const {
            std::cout << std::fixed << std::setprecision(2) // Set precision for floating-point values
                      << "Position: (" << Position.x << ", " << Position.y << "), "
                      << "Velocity: (" << Velocity.x << ", " << Velocity.y << "), "
                      << "ColorBegin: (" << ColorBegin.r << ", " << ColorBegin.g << ", " << ColorBegin.b << ", "
                      << ColorBegin.a << "), "
                      << "ColorEnd: (" << ColorEnd.r << ", " << ColorEnd.g << ", " << ColorEnd.b << ", " << ColorEnd.a
                      << "), "
                      << "Rotation: " << Rotation << ", "
                      << "SizeBegin: " << SizeBegin << ", "
                      << "SizeEnd: " << SizeEnd << ", "
                      << "Lifetime: " << Lifetime << ", "
                      << "LifeRemaining: " << LifeRemaining << ", "
                      << "Active: " << (Active ? "true" : "false") << std::endl;
        }
    };
    std::vector<Particle> m_ParticlePool;
    uint32_t m_PoolIndex = 999;
};
