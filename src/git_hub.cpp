#include "git_hub.h"

#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QTimer>
#include <nlohmann/json.hpp>

namespace ObsidianInstaller {
    using json = nlohmann::json;

    std::vector<Release> GitHub::getReleases() {
        std::vector<Release> releases;

        // Create manager
        auto *manager = new QNetworkAccessManager();

        QNetworkRequest request(QUrl(R"(https://api.github.com/repos/Obsidian-Minecraft-Server-Portal/obsidian-server-panel/releases)"));
        request.setHeader(QNetworkRequest::UserAgentHeader, "Obsidian-Installer");
        request.setRawHeader("Accept", "application/vnd.github+json");

        // Make synchronous request
        QNetworkReply *reply = manager->get(request);

        // Create an event loop to wait for a response
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        // Setup timeout
        QTimer timer;
        timer.setSingleShot(true);
        timer.setInterval(10000); // 10-second timeout
        QObject::connect(&timer, &QTimer::timeout, &loop, [&loop, reply]() {
            printf("Request timed out\n");
            reply->abort();
            loop.quit();
        });

        timer.start();
        loop.exec(); // Wait for the request to complete or timeout

        // Check for errors
        if (reply->error() != QNetworkReply::NoError) {
            printf("Network error: %s\n", reply->errorString().toStdString().c_str());
            reply->deleteLater();
            manager->deleteLater();
            return releases;
        }

        const auto data = reply->readAll();
        printf("Received %zu bytes of data\n", data.size());

        if (data.isEmpty()) {
            printf("No data received\n");
            reply->deleteLater();
            manager->deleteLater();
            return releases;
        }

        try {
            for (json jsonData = json::parse(data.data()); const auto &release: jsonData) {
                Release r;
                r.version = release["tag_name"].get<std::string>();
                r.type = getReleaseType(r.version);
                for (const auto &asset: release["assets"]) {
                    ReleaseArtifact a;
                    a.name = asset["name"].get<std::string>();
                    a.os = getOSVersion(a.name);
                    a.url = asset["browser_download_url"].get<std::string>();
                    r.artifacts.push_back(a);
                }
                releases.push_back(r);
            }
        } catch (const json::exception &e) {
            printf("JSON parse error: %s\n", e.what());
        }

        // Cleans the memory after the event loop exits.
        reply->deleteLater();
        manager->deleteLater();
        return releases;
    }

    GitHubReleaseType GitHub::getReleaseType(const std::string &tag) {
        if (tag.find("beta") != std::string::npos) {
            return GitHubReleaseType::BETA;
        }
        if (tag.find("alpha") != std::string::npos) {
            return GitHubReleaseType::ALPHA;
        }
        return GitHubReleaseType::RELEASE;
    }

    OSVersion GitHub::getOSVersion(const std::string &artifact) {
        if (artifact.find("win") != std::string::npos || artifact.find("windows") != std::string::npos) {
            return OSVersion::WINDOWS;
        }
        if (artifact.find("linux") != std::string::npos) {
            return OSVersion::LINUX;
        }
        if (artifact.find("mac") != std::string::npos || artifact.find("macos") != std::string::npos || artifact.find("darwin") != std::string::npos) {
            return OSVersion::MACOS;
        }
        return OSVersion::UNKNOWN;
    }
}
