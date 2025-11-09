#include "ui/pages/tospage.h"
#include <ui/installerwindow.h>
#include <QNetworkRequest>
#include <QUrl>

namespace ObsidianInstaller {
    TOSPage::TOSPage(QWidget *parent) : QWidget(parent), ui(new Ui::TOSPage)  {
        ui->setupUi(this);
        connect(ui->backButton, &QPushButton::clicked, this, &TOSPage::onBack);
        connect(ui->nextButton, &QPushButton::clicked, this, &TOSPage::onNext);

        // Initialize network manager
        networkManager = new QNetworkAccessManager(this);
        connect(networkManager, &QNetworkAccessManager::finished, this, &TOSPage::onTermsLoaded);

        // Load terms from URL
        loadTermsOfService();
    }

    TOSPage::~TOSPage() {
        delete ui;
    }

    void TOSPage::onBack() const {
        reinterpret_cast<InstallerWindow *>(window())->decrementPage();
    }

    void TOSPage::onNext() const {
        reinterpret_cast<InstallerWindow *>(window())->incrementPage();
    }

    void TOSPage::loadTermsOfService() {
        // Show loading message
        ui->textEdit->setText("Loading Terms of Service...");

        // Fetch terms from GitHub
        const QUrl url("https://raw.githubusercontent.com/Obsidian-Minecraft-Server-Portal/obsidian-server-panel/refs/heads/main/terms-of-service.md");
        QNetworkRequest request(url);
        networkManager->get(request);
    }

    void TOSPage::onTermsLoaded(QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            // Successfully loaded
            const QString markdown = QString::fromUtf8(reply->readAll());
            ui->textEdit->setText(markdown);
        } else {
            // Error loading
            ui->textEdit->setText(QString("**Error loading Terms of Service**\n\nFailed to fetch terms from server: %1\n\nPlease check your internet connection and try again.").arg(reply->errorString()));
        }
        reply->deleteLater();
    }
} // ObsidianInstaller
