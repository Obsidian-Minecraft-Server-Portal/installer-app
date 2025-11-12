#include "installer.h"

#include <filesystem>
#include <iostream>

#include "logger.h"

namespace ObsidianInstaller {
    void Installer::CreateInstallationFolder(const char *path) {
        LOG_INFO("Creating installation folder...");
        std::filesystem::create_directories(path);
#ifdef __WIN64__
#include <winbase.h>
        LOG_DEBUG("Windows 64-bit detected, setting the directories permissions to allow for the installer to write to it...");
        SetFileSecurityA(path, OWNER_SECURITY_INFORMATION, nullptr);
#endif
    }


#ifdef __WIN64__
#include <windows.h>
#include <shellapi.h>

    bool Installer::LaunchProcess(const char *args) {
        wchar_t executablePath[MAX_PATH];
        GetModuleFileNameW(nullptr, executablePath, MAX_PATH);

        const int bufferSize = MultiByteToWideChar(CP_UTF8, 0, args, -1, nullptr, 0);
        const auto parametersW = new wchar_t[bufferSize];
        MultiByteToWideChar(CP_UTF8, 0, args, -1, parametersW, bufferSize);
        std::wcout << parametersW << std::endl;

        SHELLEXECUTEINFO shExInfo = {};
        shExInfo.cbSize = sizeof(shExInfo);
        shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        shExInfo.hwnd = nullptr;
        shExInfo.lpVerb = L"runas"; // This is the key - requests elevation
        shExInfo.lpFile = executablePath;
        shExInfo.lpParameters = parametersW;
        shExInfo.lpDirectory = nullptr;
        shExInfo.nShow = SW_SHOW;

        shExInfo.hInstApp = nullptr;

        if (!ShellExecuteEx(&shExInfo)) {
            if (const DWORD error = GetLastError(); error == ERROR_CANCELLED) {
                // User declined the UAC prompt
                return false;
            }
            // Other error occurred
            return false;
        }

        // Optional: Wait for the process to complete
        if (shExInfo.hProcess != nullptr) {
            WaitForSingleObject(shExInfo.hProcess, INFINITE);
            CloseHandle(shExInfo.hProcess);
        }

        return true;
    }
#else
    bool Installer::LaunchProcess(const wchar_t *parameters) {
    }
#endif
}
