#pragma once

namespace zirconium {
class TimeStep {
public:
    TimeStep(float time = 0.0f)
        : m_Time(time) {}

    float GetTimeSeconds() const {
        return m_Time;
    };
    float GetTimeMiliseconds() const {
        return m_Time * 1000;
    };

    operator float() const {
        return GetTimeSeconds();
    }

private:
    float m_Time;
};
}; // namespace zirconium
