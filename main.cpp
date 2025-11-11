#include <QApplication>
#include "ui/installerwindow.h"
#include "logger.h"
using namespace ObsidianInstaller;


int main(int argc, char *argv[]) {
    // Initialize logger first
    Logger::init();
    LOG_INFO("Application starting...");

    QApplication app(argc, argv);
    LOG_DEBUG("QApplication initialized");

    InstallerWindow window;
    LOG_DEBUG("InstallerWindow created");

    window.show();
    LOG_INFO("Application window shown");

    auto result = QApplication::exec();
    LOG_INFO(std::format("Application exiting with code: {}", result));

    return result;
}
