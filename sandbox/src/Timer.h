#pragma once

#include <chrono>
#include <thread>

// #include "zirconium.h"

template <typename Func>
class Timer {
public:
    explicit Timer(const char* name, Func&& func)
        : m_Name(name)
        , m_StartTime(std::chrono::high_resolution_clock::now())
        , m_StorageFunc(func) {}

    ~Timer() {
        stop();
    }

private:
void stop() {
    auto end_time = std::chrono::high_resolution_clock::now();
    float duration = std::chrono::duration<float, std::milli>(end_time - m_StartTime).count(); // Directly in milliseconds
    m_StorageFunc({m_Name, duration});
}

private:
    const char* m_Name;
    std::chrono::high_resolution_clock::time_point m_StartTime;
    std::chrono::high_resolution_clock::time_point m_PauseStart;
    Func m_StorageFunc;
};

struct ProfileResult {
    const char* Name;
    float Time;
};
