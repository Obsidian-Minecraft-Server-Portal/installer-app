#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <memory>

namespace ObsidianInstaller {

    class Logger {
    public:
        /**
         * Initializes the logger based on build mode
         * In Debug: Logs to stdout
         * In Release: Logs to file in temp directory with name obsidian-installer.{timestamp}.log
         */
        static void init();

        /**
         * Get the default logger instance
         */
        static std::shared_ptr<spdlog::logger>& getLogger();

    private:
        static std::shared_ptr<spdlog::logger> s_logger;
    };

} // namespace ObsidianInstaller

// Convenience macros for logging
#define LOG_TRACE(...)    ::ObsidianInstaller::Logger::getLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)    ::ObsidianInstaller::Logger::getLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)     ::ObsidianInstaller::Logger::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)     ::ObsidianInstaller::Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    ::ObsidianInstaller::Logger::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::ObsidianInstaller::Logger::getLogger()->critical(__VA_ARGS__)

#endif // LOGGER_H
