#include "ui/pages/welcomepage.h"
#include "logger.h"
#include <ui/installerwindow.h>

namespace ObsidianInstaller {
    WelcomePage::WelcomePage(QWidget *parent) : QWidget(parent), ui(new Ui::WelcomePage) {
        LOG_INFO("Initializing WelcomePage");
        ui->setupUi(this);
        connect(ui->nextButton, &QPushButton::clicked, this, &WelcomePage::onNext);
        LOG_DEBUG("WelcomePage initialized, next button connected");
    }

    WelcomePage::~WelcomePage() {
        LOG_TRACE("Destroying WelcomePage");
        delete ui;
    }

    void WelcomePage::onNext() const {
        LOG_INFO("User clicked 'Next' on WelcomePage");
        reinterpret_cast<InstallerWindow *>(window())->incrementPage();
    }
} // ObsidianInstaller
