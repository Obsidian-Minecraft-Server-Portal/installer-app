#include "state_manager.h"

namespace ObsidianInstaller {
    StateManager::StateManager() {
#ifdef __WIN64__
        install_path = "C:\\Program Files\\Obsidian Minecraft Server Panel";
#else
        install_path = "/opt/obsidian-server-panel"
#endif
    }

    StateManager *StateManager::GetInstance() {
        static StateManager instance;
        return &instance;
    }
}
