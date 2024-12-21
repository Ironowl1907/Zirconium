#pragma once

#include <chrono>
#include <thread>

class Timer {
public:
    explicit Timer(const char* name = "Timer")
        : m_Name(name)
        , m_StartTime(std::chrono::high_resolution_clock::now()) {
        ZR_TRACE("Profiling: Timer stared '{0}'", m_Name);
    }

    ~Timer() {
        stop();
    }

private:
    void stop() {
        auto end_time = std::chrono::high_resolution_clock::now();
        float duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - m_StartTime).count();
        m_Results->Name = m_Name;
        m_Results->Time = duration;
        ZR_TRACE("Profiling: Timer stoped '{0}' duration: {1} ms", m_Name, duration / 1000);
    }

    const char* m_Name;
    std::chrono::high_resolution_clock::time_point m_StartTime;
    std::chrono::high_resolution_clock::time_point m_PauseStart;
};

struct ProfileResult {
    const char* Name;
    float Time;
};
