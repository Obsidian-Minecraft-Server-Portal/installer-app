#include "logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <chrono>
#include <format>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

namespace ObsidianInstaller {

    std::shared_ptr<spdlog::logger> Logger::s_logger;

    std::string getTempDirectory() {
#ifdef _WIN32
        char tempPath[MAX_PATH];
        DWORD result = GetTempPathA(MAX_PATH, tempPath);
        if (result > 0 && result < MAX_PATH) {
            return std::string(tempPath);
        }
        return "C:\\Temp\\";
#else
        const char* tmpDir = std::getenv("TMPDIR");
        if (tmpDir) return std::string(tmpDir);
        return "/tmp/";
#endif
    }

    void Logger::init() {
        try {
            // Get current timestamp in milliseconds
            auto now = std::chrono::system_clock::now();
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()
            ).count();

#ifdef NDEBUG
            // Release mode: Log to file
            std::string tempDir = getTempDirectory();
            std::string logFileName = "obsidian-installer." + std::to_string(ms) + ".log";
            std::string logFilePath = tempDir + logFileName;

            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);
            file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

            s_logger = std::make_shared<spdlog::logger>("obsidian_installer", file_sink);
            s_logger->set_level(spdlog::level::trace);

            s_logger->info("Logger initialized in RELEASE mode");
            s_logger->info(std::format("Log file: {}", logFilePath));
#else
            // Debug mode: Log to stdout
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_pattern("[%^%l%$] %v");

            s_logger = std::make_shared<spdlog::logger>("obsidian_installer", console_sink);
            s_logger->set_level(spdlog::level::trace); // Debug mode: trace and above

            s_logger->info("Logger initialized in DEBUG mode");
#endif

            // Flush on every log message (can be adjusted for performance)
            s_logger->flush_on(spdlog::level::trace);

            // Set as default logger
            spdlog::set_default_logger(s_logger);

        } catch (const spdlog::spdlog_ex& ex) {
            fprintf(stderr, "Log initialization failed: %s\n", ex.what());
        }
    }

    std::shared_ptr<spdlog::logger>& Logger::getLogger() {
        if (!s_logger) {
            init();
        }
        return s_logger;
    }

} // namespace ObsidianInstaller
