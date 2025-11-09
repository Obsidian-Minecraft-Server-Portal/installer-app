#include "ui/pages/welcomepage.h"
#include <ui/installerwindow.h>

namespace ObsidianInstaller {
    WelcomePage::WelcomePage(QWidget *parent) : QWidget(parent), ui(new Ui::WelcomePage) {
        ui->setupUi(this);
        connect(ui->nextButton, &QPushButton::clicked, this, &WelcomePage::onNext);
    }

    WelcomePage::~WelcomePage() {
        delete ui;
    }

    void WelcomePage::onNext() const {
        reinterpret_cast<InstallerWindow *>(window())->incrementPage();
    }
} // ObsidianInstaller
