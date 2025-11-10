#include "git_hub.h"

#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QTimer>
#include <nlohmann/json.hpp>

namespace ObsidianInstaller {
    using json = nlohmann::json;

    void GitHub::getReleases(std::function<void(std::vector<Release>, bool success)> callback) {
        // Create manager on the heap so it persists after function returns
        auto *manager = new QNetworkAccessManager();
        
        QNetworkRequest request(QUrl(R"(https://api.github.com/repos/Obsidian-Minecraft-Server-Portal/obsidian-server-panel/releases)"));
        request.setHeader(QNetworkRequest::UserAgentHeader, "Obsidian-Installer");
        request.setRawHeader("Accept", "application/vnd.github+json");

        QNetworkReply *reply = manager->get(request);

        // Setup timeout
        auto *timeout = new QTimer(reply);
        timeout->setSingleShot(true);
        timeout->setInterval(10000); // 10-second timeout
        
        // Handle timeout
        QObject::connect(timeout, &QTimer::timeout, reply, [reply, callback]() {
            printf("Request timed out\n");
            reply->abort();
            callback({}, false);
        });
        
        // Handle successful completion
        QObject::connect(reply, &QNetworkReply::finished, [reply, manager, timeout, callback]() {
            timeout->stop();
            
            std::vector<Release> releases;
            
            // Check for errors
            if (reply->error() != QNetworkReply::NoError) {
                printf("Network error: %s\n", reply->errorString().toStdString().c_str());
                reply->deleteLater();
                manager->deleteLater();
                callback(releases, false);
                return;
            }
            
            const auto data = reply->readAll();
            printf("Received %zu bytes of data\n", data.size());
            
            if (data.isEmpty()) {
                printf("No data received\n");
                reply->deleteLater();
                manager->deleteLater();
                callback(releases, false);
                return;
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
                
                callback(releases, true);
            } catch (const json::exception &e) {
                printf("JSON parse error: %s\n", e.what());
                callback(releases, false);
            }
            
            reply->deleteLater();
            manager->deleteLater();
        });
        
        timeout->start();
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
