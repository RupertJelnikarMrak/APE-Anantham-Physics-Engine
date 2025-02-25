#include "Core/Logger.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <spdlog/common.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// std
#include <chrono>
#include <ctime>
#include <memory>

namespace Core
{

void Logger::init()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::string log_filename = fmt::format(fmt::runtime("logs/run_{:%Y-%m-%d_%H-%M-%S}.log"), *std::localtime(&now_c));

    size_t max_size = 5 * 1024 * 1024; // 5MB
    size_t max_files = 5;

    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_filename, max_size, max_files);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    file_sink->set_pattern("[%T - %s:%# in %!] [%l] %v");
    console_sink->set_pattern("\033[90m[%T - %s:%#]\033[0m %^[%l] %v%$");

#ifdef NDEBUG
    file_sink->set_level(spdlog::level::info);
    console_sink->set_level(spdlog::level::warn);
#else
    file_sink->set_level(spdlog::level::trace);
    console_sink->set_level(spdlog::level::info);
#endif

    spdlog::set_default_logger(
        std::make_shared<spdlog::logger>("ape", spdlog::sinks_init_list{console_sink, file_sink}));
    spdlog::set_level(spdlog::level::trace);

    // SPDLOG_TRACE("This is a trace message");
    // SPDLOG_DEBUG("This is a debug message");
    // SPDLOG_INFO("This is an info message");
    // SPDLOG_WARN("This is a warning message");
    // SPDLOG_ERROR("This is an error message");
}

} // namespace Core
