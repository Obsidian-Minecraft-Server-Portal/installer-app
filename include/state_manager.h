#pragma once

namespace ObsidianInstaller {
    class StateManager final {
    public:
        static StateManager *GetInstance();

    private:
        const char *install_path;
        const char *installation_artifact_url = nullptr;
        bool install_service = false;
        bool autostart = false;

        StateManager();
    };
}
