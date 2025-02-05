#pragma once
#include "core.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

namespace zirconium {
class Log {

public:
    static void Init();

    inline static Ref<spdlog::logger>& GetCoreLogger() {
        return s_CoreLogger;
    };
    inline static Ref<spdlog::logger>& GetClientLogger() {
        return s_ClientLogger;
    };

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
} // namespace zirconium

// clang-format off
// Core log macros
#define ZR_CORE_TRACE(...)   ::zirconium::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ZR_CORE_INFO(...)    ::zirconium::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ZR_CORE_WARN(...)    ::zirconium::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ZR_CORE_ERROR(...)   ::zirconium::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ZR_CORE_FATAL(...)   ::zirconium::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Core log macros
#define ZR_TRACE(...)        ::zirconium::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ZR_INFO(...)         ::zirconium::Log::GetClientLogger()->info(__VA_ARGS__)
#define ZR_WARN(...)         ::zirconium::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ZR_ERROR(...)        ::zirconium::Log::GetClientLogger()->error(__VA_ARGS__)
#define ZR_FATAL(...)        ::zirconium::Log::GetClientLogger()->fatal(__VA_ARGS__)

// clang-format on
