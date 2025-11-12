#include <iostream>
#include <QApplication>
#include "ui/installerwindow.h"
#include "logger.h"
#include <string>
#include "installer.h"
using namespace ObsidianInstaller;


int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        const std::string arg(argv[i]);
        std::cout << arg << std::endl;
        if (arg.starts_with("mkdir=")) {
            const auto dir = arg.substr(6);
            Installer::CreateInstallationFolder(dir.c_str());
            return 0;
        }
    }

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
