#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>

#include <thread>

namespace zirconium {
struct ProfileResult {
    std::string Name;
    long long Start, End;
    uint32_t ThreadID;
};

struct InstrumentationSession {
    std::string Name;
};

class Instrumentor {
public:
    Instrumentor()
        : m_CurrentSession(nullptr)
        , m_ProfileCount(0) {}

    void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
        m_OutputStream.open(filepath);
        WriteHeader();
        m_CurrentSession = new InstrumentationSession{name};
    }

    void EndSession() {
        WriteFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
        m_ProfileCount = 0;
    }

    void WriteProfile(const ProfileResult& result) {
        if (m_ProfileCount++ > 0)
            m_OutputStream << ",";

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"function\",";
        m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
        m_OutputStream << "\"name\":\"" << name << "\",";
        m_OutputStream << "\"ph\":\"X\",";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << result.ThreadID << ",";
        m_OutputStream << "\"ts\":" << result.Start;
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    void WriteHeader() {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        m_OutputStream.flush();
    }

    void WriteFooter() {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

    static Instrumentor& Get() {
        static Instrumentor instance;
        return instance;
    }

private:
    InstrumentationSession* m_CurrentSession;
    std::ofstream m_OutputStream;
    int m_ProfileCount;
};

class InstrumentationTimer {
public:
    InstrumentationTimer(const char* name)
        : m_Name(name)
        , m_Stopped(false) {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer() {
        if (!m_Stopped)
            Stop();
    }

    void Stop() {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start =
            std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end =
            std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::Get().WriteProfile({m_Name, start, end, threadID});

        m_Stopped = true;
    }

private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
};
} // namespace zirconium

#define ZR_PROFILE 1
#if ZR_PROFILE
#    define ZR_PROFILE_BEGIN_SESSION(name, filepath) ::zirconium::Instrumentor::Get().BeginSession(name, filepath)
#    define ZR_PROFILE_END_SESSION() ::zirconium::Instrumentor::Get().EndSession()
#    define ZR_PROFILE_SCOPE(name) ::zirconium::InstrumentationTimer timer##__LINE__(name)
#    define ZR_PROFILE_FUNCTION() ZR_PROFILE_SCOPE(__FUNCSIG__)
#else
#    define ZR_PROFILE_BEGIN_SESSION(name, filepath)
#    define ZR_PROFILE_END_SESSION()
#    define ZR_PROFILE_SCOPE(name)
#    define ZR_PROFILE_FUNCTION()
#endif
