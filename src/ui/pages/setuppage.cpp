#include "ui/pages/setuppage.h"
#include "ui_SetupPage.h"
#include "ui/installerwindow.h"
#include <QFileDialog>

namespace ObsidianInstaller {
    SetupPage::SetupPage(QWidget *parent) : QWidget(parent), ui(new Ui::SetupPage) {
        ui->setupUi(this);
        ui->nextButton->setDisabled(true);
        connect(ui->backButton, &QPushButton::clicked, this, &SetupPage::onBack);
        connect(ui->nextButton, &QPushButton::clicked, this, &SetupPage::onNext);
        connect(ui->browseInstallationPathButton, &QPushButton::clicked, this, &SetupPage::onBrowse);
#ifdef Q_OS_WIN
        this->installPath = QString("C:\\Program Files\\Obsidian Minecraft Server Panel");
#elifdef Q_OS_LINUX
        this->installPath = QString("/opt/obsidian-server-panel");
#endif
        this->ui->browseInstallPathInput->setText(this->installPath);
        this->ui->releaseChannelSelector->addItem("Release (Stable) - Not Available");
        this->ui->releaseChannelSelector->addItem("Beta - Not Available");
        this->ui->releaseChannelSelector->addItem("Alpha (Pre-Release) - Not Available");
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
} // ObsidianInstaller
