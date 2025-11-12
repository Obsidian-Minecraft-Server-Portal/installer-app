#pragma once

namespace ObsidianInstaller {
    class Installer {
    public:
        static void CreateInstallationFolder(const char *path);
        static bool LaunchProcess(const char *args = nullptr);
    };
}
