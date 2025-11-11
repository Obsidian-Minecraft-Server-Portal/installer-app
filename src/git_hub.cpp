#include "git_hub.h"
#include "logger.h"

#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QTimer>
#include <nlohmann/json.hpp>
#include <format>

namespace ObsidianInstaller {
    using json = nlohmann::json;

    std::vector<Release> GitHub::getReleases() {
        LOG_INFO("Fetching releases from GitHub API");
        std::vector<Release> releases;

        // Create manager
        auto *manager = new QNetworkAccessManager();
        LOG_TRACE("QNetworkAccessManager created");

        QNetworkRequest request(QUrl(R"(https://api.github.com/repos/Obsidian-Minecraft-Server-Portal/obsidian-server-panel/releases)"));
        request.setHeader(QNetworkRequest::UserAgentHeader, "Obsidian-Installer");
        request.setRawHeader("Accept", "application/vnd.github+json");
        LOG_DEBUG("Network request configured for GitHub API");

        // Make synchronous request
        QNetworkReply *reply = manager->get(request);
        LOG_TRACE("Network request sent");

        // Create an event loop to wait for a response
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        // Setup timeout
        QTimer timer;
        timer.setSingleShot(true);
        timer.setInterval(10000); // 10-second timeout
        QObject::connect(&timer, &QTimer::timeout, &loop, [&loop, reply]() {
            LOG_ERROR("GitHub API request timed out after 10 seconds");
            reply->abort();
            loop.quit();
        });

        timer.start();
        loop.exec(); // Wait for the request to complete or timeout

        // Check for errors
        if (reply->error() != QNetworkReply::NoError) {
            LOG_ERROR(std::format("Network error while fetching releases: {}", reply->errorString().toStdString()));
            reply->deleteLater();
            manager->deleteLater();
            return releases;
        }

        const auto data = reply->readAll();
        LOG_INFO(std::format("Received {} bytes of data from GitHub API", data.size()));

        if (data.isEmpty()) {
            LOG_WARN("No data received from GitHub API");
            reply->deleteLater();
            manager->deleteLater();
            return releases;
        }

        try {
            for (json jsonData = json::parse(data.data()); const auto &release: jsonData) {
                Release r;
                r.version = release["tag_name"].get<std::string>();
                r.type = getReleaseType(r.version);
                LOG_DEBUG(std::format("Parsing release: {} (type: {})", r.version, static_cast<int>(r.type)));

                for (const auto &asset: release["assets"]) {
                    ReleaseArtifact a;
                    a.name = asset["name"].get<std::string>();
                    a.os = getOSVersion(a.name);
                    a.url = asset["browser_download_url"].get<std::string>();
                    r.artifacts.push_back(a);
                    LOG_TRACE(std::format("  - Artifact: {} (OS: {})", a.name, static_cast<int>(a.os)));
                }
                releases.push_back(r);
            }
            LOG_INFO(std::format("Successfully parsed {} releases from GitHub", releases.size()));
        } catch (const json::exception &e) {
            LOG_ERROR(std::format("JSON parse error: {}", e.what()));
        }

        // Cleans the memory after the event loop exits.
        reply->deleteLater();
        manager->deleteLater();
        LOG_TRACE("Network resources cleaned up");
        return releases;
    }

    GitHubReleaseType GitHub::getReleaseType(const std::string &tag) {
        LOG_TRACE(std::format("Determining release type for tag: {}", tag));
        if (tag.find("beta") != std::string::npos) {
            LOG_TRACE("  -> BETA");
            return GitHubReleaseType::BETA;
        }
        if (tag.find("alpha") != std::string::npos) {
            LOG_TRACE("  -> ALPHA");
            return GitHubReleaseType::ALPHA;
        }
        LOG_TRACE("  -> RELEASE");
        return GitHubReleaseType::RELEASE;
    }

    OSVersion GitHub::getOSVersion(const std::string &artifact) {
        LOG_TRACE(std::format("Determining OS version for artifact: {}", artifact));
        if (artifact.find("win") != std::string::npos || artifact.find("windows") != std::string::npos) {
            LOG_TRACE("  -> WINDOWS");
            return OSVersion::WINDOWS;
        }
        if (artifact.find("linux") != std::string::npos) {
            LOG_TRACE("  -> LINUX");
            return OSVersion::LINUX;
        }
        if (artifact.find("mac") != std::string::npos || artifact.find("macos") != std::string::npos || artifact.find("darwin") != std::string::npos) {
            LOG_TRACE("  -> MACOS");
            return OSVersion::MACOS;
        }
        LOG_TRACE("  -> UNKNOWN");
        return OSVersion::UNKNOWN;
    }
}
