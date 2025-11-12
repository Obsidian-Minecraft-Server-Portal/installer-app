#include "ui/pages/setuppage.h"
#include "ui_SetupPage.h"
#include "ui/installerwindow.h"
#include "logger.h"
#include <QFileDialog>
#include <QThreadPool>
#include <format>

#include "installer.h"

namespace ObsidianInstaller {
    SetupPage::SetupPage(QWidget *parent) : QWidget(parent), ui(new Ui::SetupPage) {
        LOG_INFO("Initializing SetupPage");
        ui->setupUi(this);
        ui->nextButton->setDisabled(true);
        connect(ui->backButton, &QPushButton::clicked, this, &SetupPage::onBack);
        connect(ui->nextButton, &QPushButton::clicked, this, &SetupPage::onNext);
        connect(ui->browseInstallationPathButton, &QPushButton::clicked, this, &SetupPage::onBrowse);
        connect(this, &SetupPage::releasesLoaded, this, &SetupPage::onReleasesLoaded);
        connect(ui->releaseChannelSelector, &QComboBox::currentIndexChanged, this, &SetupPage::onReleaseSelected);
        LOG_TRACE("All signals connected");

#ifdef Q_OS_WIN
        this->installPath = QString("C:\\Program Files\\Obsidian Minecraft Server Panel");
        LOG_DEBUG(std::format("Default install path (Windows): {}", this->installPath.toStdString()));
#elifdef Q_OS_LINUX
        this->installPath = QString("/opt/obsidian-server-panel");
        LOG_DEBUG(std::format("Default install path (Linux): {}", this->installPath.toStdString()));
#endif
        this->ui->browseInstallPathInput->setText(this->installPath);
        refresh();
        LOG_INFO("SetupPage initialized successfully");
    }

    SetupPage::~SetupPage() {
        LOG_TRACE("Destroying SetupPage");
        delete ui;
    }

    void SetupPage::onNext() const {
        Installer::LaunchProcess(std::format("\"mkdir={}\"", this->installPath.toStdString().c_str()).c_str());
        LOG_INFO(std::format("User clicked 'Next' on SetupPage, install path: {}", this->installPath.toStdString()));
        reinterpret_cast<InstallerWindow *>(window())->incrementPage();
    }

    void SetupPage::onBack() const {
        LOG_INFO("User clicked 'Back' on SetupPage");
        reinterpret_cast<InstallerWindow *>(window())->decrementPage();
    }

    void SetupPage::onBrowse() {
        LOG_DEBUG("Opening directory browser dialog");
        QFileDialog dialog;
        dialog.setOptions(QFileDialog::ShowDirsOnly);
        dialog.setFileMode(QFileDialog::Directory);
        if (this->installPath.isEmpty()) {
            LOG_TRACE("Install path is empty, using home directory");
            dialog.setDirectory(QDir::homePath());
        } else {
            LOG_TRACE(std::format("Using current install path: {}", this->installPath.toStdString()));
            dialog.setDirectory(this->installPath);
        }
        dialog.setWindowTitle(tr("Select installation directory"));
        dialog.setLabelText(QFileDialog::Accept, tr("Select"));

        if (dialog.exec() == QDialog::Accepted) {
            const auto path = dialog.selectedFiles().first().append("/Obsidian Minecraft Server Panel");
            LOG_INFO(std::format("User selected installation path: {}", path.toStdString()));
            this->installPath = path;
            this->ui->browseInstallPathInput->setText(path);
        } else {
            LOG_DEBUG("User cancelled directory selection");
        }
    }

    void SetupPage::onReleaseSelected(const int index) const {
        LOG_DEBUG(std::format("Release channel selected: index={}", index));
        switch (index) {
            case 0:
                LOG_INFO(std::format("User selected RELEASE channel (available: {})", hasRelease));
                if (hasRelease) {
                    this->ui->releaseChannelMessage->setText("The release channel is the most stable channel, this is the recommended channel for most users.");
                    this->ui->releaseChannelMessage->setStyleSheet("color: #a0a0a0;");
                    this->ui->nextButton->setDisabled(false);
                } else {
                    LOG_WARN("Release channel not available");
                    this->ui->releaseChannelMessage->setText("Release channel not available - select another channel");
                    this->ui->releaseChannelMessage->setStyleSheet("color: #ff4444;");
                    this->ui->nextButton->setDisabled(true);
                }
                break;
            case 1:
                LOG_INFO(std::format("User selected BETA channel (available: {})", hasBeta));
                if (hasBeta) {
                    this->ui->releaseChannelMessage->setText("The beta channel could be unstable, use this channel if you want to try out new features.");
                    this->ui->releaseChannelMessage->setStyleSheet("color: #a0a0a0;");
                    this->ui->nextButton->setDisabled(false);
                } else {
                    LOG_WARN("Beta channel not available");
                    this->ui->releaseChannelMessage->setText("Beta channel not available - select another channel");
                    this->ui->releaseChannelMessage->setStyleSheet("color: #ff4444;");
                    this->ui->nextButton->setDisabled(true);
                }
                break;
            case 2:
                LOG_INFO(std::format("User selected ALPHA channel (available: {})", hasAlpha));
                if (hasAlpha) {
                    this->ui->releaseChannelMessage->setText("The alpha channel is unstable, use this channel if you want to try out new features.");
                    this->ui->releaseChannelMessage->setStyleSheet("color: #a0a0a0;");
                    this->ui->nextButton->setDisabled(false);
                } else {
                    LOG_WARN("Alpha channel not available");
                    this->ui->releaseChannelMessage->setText("Alpha channel not available - select another channel");
                    this->ui->releaseChannelMessage->setStyleSheet("color: #ff4444;");
                    this->ui->nextButton->setDisabled(true);
                }
                break;
            default:
                LOG_ERROR(std::format("Unknown release channel index: {}", index));
                this->ui->releaseChannelMessage->setText("Unkown error occurred - please try again later.");
                this->ui->releaseChannelMessage->setStyleSheet("color: #ff4444;");
                this->ui->nextButton->setDisabled(true);
                break;
        }
    }

    void SetupPage::refresh() {
        LOG_INFO("Refreshing release channel list");
        this->ui->releaseChannelSelector->clear();
        this->ui->releaseChannelSelector->addItem("Release (Stable) - Loading...");
        this->ui->releaseChannelSelector->addItem("Beta - Loading...");
        this->ui->releaseChannelSelector->addItem("Alpha (Pre-Release) - Loading...");
        this->ui->releaseChannelSelector->setEnabled(false);
        LOG_DEBUG("Release selector populated with loading placeholders, disabled");

        // Run network request in background thread
        LOG_TRACE("Starting background thread to fetch releases from GitHub");
        QThreadPool::globalInstance()->start([this] {
            const auto releases = GitHub::getReleases();
            emit releasesLoaded(releases);
        });
    }

    void SetupPage::onReleasesLoaded(const std::vector<Release> &releases) {
        LOG_INFO(std::format("Releases loaded from GitHub, processing {} releases", releases.size()));
        this->ui->releaseChannelSelector->clear();
        this->release = releases;
        hasRelease = false;
        hasBeta = false;
        hasAlpha = false;
        QString releaseLabel("Release (Stable) - Not Available");
        QString betaLabel("Beta - Not Available");
        QString alphaLabel("Alpha (Pre-Release) - Not Available");

        for (const auto &item: releases) {
            if (hasRelease && hasBeta && hasAlpha) {
                LOG_TRACE("All release types found, stopping search");
                break;
            }
            if (item.type == GitHubReleaseType::RELEASE && !hasRelease) {
                releaseLabel = std::format("Release (Stable) - {}", item.version).data();
                hasRelease = true;
                LOG_DEBUG(std::format("Found RELEASE version: {}", item.version));
            } else if (item.type == GitHubReleaseType::BETA && !hasBeta) {
                betaLabel = std::format("Beta - {}", item.version).data();
                hasBeta = true;
                LOG_DEBUG(std::format("Found BETA version: {}", item.version));
            } else if (item.type == GitHubReleaseType::ALPHA && !hasAlpha) {
                alphaLabel = std::format("Alpha (Pre-Release) - {}", item.version).data();
                hasAlpha = true;
                LOG_DEBUG(std::format("Found ALPHA version: {}", item.version));
            }
        }

        this->ui->releaseChannelSelector->addItem(releaseLabel);
        this->ui->releaseChannelSelector->addItem(betaLabel);
        this->ui->releaseChannelSelector->addItem(alphaLabel);
        this->ui->releaseChannelSelector->setEnabled(true);
        LOG_INFO(std::format("Release selector updated and enabled (Release: {}, Beta: {}, Alpha: {})",
                 hasRelease, hasBeta, hasAlpha));
    }
} // ObsidianInstaller
