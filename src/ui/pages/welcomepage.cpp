
#include "ui/pages/welcomepage.h"
#include "ui_WelcomePage.h"

namespace ObsidianInstaller {
    WelcomePage::WelcomePage(QWidget *parent) : QWidget(parent), ui(new Ui::WelcomePage) {
        ui->setupUi(this);
    }

    WelcomePage::~WelcomePage() {
        delete ui;
    }
} // ObsidianInstaller