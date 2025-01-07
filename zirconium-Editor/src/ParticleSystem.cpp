#include "ParticleSystem.h"
#include <random>

#include <limits>
#include <random>

class Random {
public:
    static void Init() {
        s_RandomEngine.seed(std::random_device()());
    }

    static float Float() {
        return s_Distribution(s_RandomEngine);
    }

private:
    static std::mt19937 s_RandomEngine;
    static std::uniform_real_distribution<float> s_Distribution;
};

// Define static members
std::mt19937 Random::s_RandomEngine;
std::uniform_real_distribution<float> Random::s_Distribution(0.0f, 1.0f);

ParticleSystem::ParticleSystem() {
    m_ParticlePool.resize(100000);
}
ParticleSystem::~ParticleSystem() {}

void ParticleSystem::OnUpdate(zirconium::TimeStep delta) {
    for (auto& particle : m_ParticlePool) {
        if (!particle.Active)
            continue;
        if (particle.LifeRemaining <= 0.0f) {
            particle.Active = false;
            continue;
        }

        particle.LifeRemaining -= delta;
        particle.Position += particle.Velocity * (float)delta;
        particle.Rotation += 0.01f * delta;
    }
}
void ParticleSystem::OnRender(zirconium::OrthoCamera& camera) {
    zirconium::Renderer2D::BeginScene(camera);
    for (auto& particle : m_ParticlePool) {
        if (!particle.Active)
            continue;

        // Fade Away particles
        float life = particle.LifeRemaining / particle.Lifetime;
        glm::vec4 color = glm::mix(particle.ColorEnd, particle.ColorBegin, life);
        float sizeP = glm::mix(particle.SizeEnd, particle.SizeBegin, life);

        // particle.Log();

        zirconium::Renderer2D::DrawRotatedQuad(particle.Position, {sizeP, sizeP}, particle.Rotation, color);
    }
    zirconium::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps) {
    Particle& particle = m_ParticlePool[m_PoolIndex];
    particle.Active = true;
    particle.Position = particleProps.Position;
    particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

    // Velocity
    particle.Velocity = particleProps.Velocity;
    particle.Velocity.x += particleProps.Aceleration.x * (Random::Float() - 0.5f);
    particle.Velocity.y += particleProps.Aceleration.y * (Random::Float() - 0.5f);

    // Color
    particle.ColorBegin = particleProps.ColorBegin;
    particle.ColorEnd = particleProps.ColorEnd;

    particle.Lifetime = particleProps.LifeTime;
    particle.LifeRemaining = particleProps.LifeTime;
    particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
    particle.SizeEnd = particleProps.SizeEnd;

    m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}
