#pragma once
#include <functional>
#include <string>
#include <vector>

namespace ObsidianInstaller {
    enum class GitHubReleaseType {
        RELEASE,
        BETA,
        ALPHA
    };

    enum class OSVersion {
        WINDOWS,
        LINUX,
        MACOS,
        UNKNOWN
    };
    struct ReleaseArtifact {
        std::string name;
        std::string url;
        OSVersion os;
    };

    struct Release {
        std::string version;
        GitHubReleaseType type;
        std::vector<ReleaseArtifact> artifacts;
    };

    class GitHub {
    public:
        static void getReleases(std::function<void(std::vector<Release>, bool success)> callback);
    private:
        static GitHubReleaseType getReleaseType(const std::string &tag);

        static OSVersion getOSVersion(const std::string &artifact);
    };
}
