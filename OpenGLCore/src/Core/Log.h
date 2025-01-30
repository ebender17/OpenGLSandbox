#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace OpenGLCore {

    class Log
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
    private:
        static std::shared_ptr<spdlog::logger> s_Logger;
    };

}

// TODO: strip in distribution versions

#define LOG_TRACE(...)         ::OpenGLCore::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)          ::OpenGLCore::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)          ::OpenGLCore::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)         ::OpenGLCore::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)      ::OpenGLCore::Log::GetLogger()->critical(__VA_ARGS__)

