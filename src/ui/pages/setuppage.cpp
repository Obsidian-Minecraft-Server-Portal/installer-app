#include "ui/pages/setuppage.h"
#include "ui_SetupPage.h"
#include "ui/installerwindow.h"
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>

namespace ObsidianInstaller {
    SetupPage::SetupPage(QWidget *parent) : QWidget(parent), ui(new Ui::SetupPage) {
        ui->setupUi(this);
        ui->nextButton->setDisabled(true);
        connect(ui->backButton, &QPushButton::clicked, this, &SetupPage::onBack);
        connect(ui->nextButton, &QPushButton::clicked, this, &SetupPage::onNext);
        connect(ui->browseInstallationPathButton, &QPushButton::clicked, this, &SetupPage::onBrowse);
        connect(this, &SetupPage::releasesLoaded, this, &SetupPage::onReleasesLoaded);
#ifdef Q_OS_WIN
        this->installPath = QString("C:\\Program Files\\Obsidian Minecraft Server Panel");
#elifdef Q_OS_LINUX
        this->installPath = QString("/opt/obsidian-server-panel");
#endif
        this->ui->browseInstallPathInput->setText(this->installPath);
        refresh();
    }

    SetupPage::~SetupPage() {
        delete ui;
    }

    void SetupPage::onNext() const {
        reinterpret_cast<InstallerWindow *>(window())->incrementPage();
    }

    void SetupPage::onBack() const {
        reinterpret_cast<InstallerWindow *>(window())->decrementPage();
    }

    void SetupPage::onBrowse() {
        QFileDialog dialog;
        dialog.setOptions(QFileDialog::ShowDirsOnly);
        dialog.setFileMode(QFileDialog::Directory);
        if (this->installPath.isEmpty()) {
            dialog.setDirectory(QDir::homePath());
        } else {
            dialog.setDirectory(this->installPath);
        }
        dialog.setWindowTitle(tr("Select installation directory"));
        dialog.setLabelText(QFileDialog::Accept, tr("Select"));

        if (dialog.exec() == QDialog::Accepted) {
            const auto path = dialog.selectedFiles().first();
            this->installPath = path;
            this->ui->browseInstallPathInput->setText(path);
        }
    }

    void SetupPage::refresh() {
        this->ui->releaseChannelSelector->clear();
        this->ui->releaseChannelSelector->addItem("Release (Stable) - Loading...");
        this->ui->releaseChannelSelector->addItem("Beta - Loading...");
        this->ui->releaseChannelSelector->addItem("Alpha (Pre-Release) - Loading...");
        this->ui->releaseChannelSelector->setEnabled(false);

        // Run network request in background thread
        QThreadPool::globalInstance()->start([this] {
            const auto releases = GitHub::getReleases();
            emit releasesLoaded(releases);
        });
    }

    void SetupPage::onReleasesLoaded(const std::vector<Release> &releases) {
        this->ui->releaseChannelSelector->clear();
        this->release = releases;
        bool hasRelease = false;
        bool hasBeta = false;
        bool hasAlpha = false;
        QString releaseLabel("Release (Stable) - Not Available");
        QString betaLabel("Beta - Not Available");
        QString alphaLabel("Alpha (Pre-Release) - Not Available");
        for (const auto &item: releases) {
            if (hasRelease && hasBeta && hasAlpha) break;
            if (item.type == GitHubReleaseType::RELEASE && !hasRelease) {
                releaseLabel = std::format("Release (Stable) - {}", item.version).data();
                hasRelease = true;
            } else if (item.type == GitHubReleaseType::BETA && !hasBeta) {
                betaLabel = std::format("Beta - {}", item.version).data();
                hasBeta = true;
            } else if (item.type == GitHubReleaseType::ALPHA && !hasAlpha) {
                alphaLabel = std::format("Alpha (Pre-Release) - {}", item.version).data();
                hasAlpha = true;
            }
        }
        this->ui->releaseChannelSelector->addItem(releaseLabel);
        this->ui->releaseChannelSelector->addItem(betaLabel);
        this->ui->releaseChannelSelector->addItem(alphaLabel);
        this->ui->releaseChannelSelector->setEnabled(true);
    }
} // ObsidianInstaller
