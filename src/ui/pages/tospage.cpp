#include "ui/pages/tospage.h"
#include <ui/installerwindow.h>

namespace ObsidianInstaller {
    TOSPage::TOSPage(QWidget *parent) : QWidget(parent), ui(new Ui::TOSPage)  {
        ui->setupUi(this);
        connect(ui->backButton, &QPushButton::clicked, this, &TOSPage::onBack);
        connect(ui->nextButton, &QPushButton::clicked, this, &TOSPage::onNext);
        connect(ui->tosAcceptCheckbox, &QCheckBox::stateChanged, this, &TOSPage::onCheckBoxStateChanged);

        ui->nextButton->setDisabled(true);
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

    void TOSPage::onCheckBoxStateChanged(int state) {
        ui->nextButton->setDisabled(state != Qt::Checked);
    }
} // ObsidianInstaller
