#include "ui/pages/tospage.h"
#include "logger.h"
#include <ui/installerwindow.h>
#include <format>

namespace ObsidianInstaller {
    TOSPage::TOSPage(QWidget *parent) : QWidget(parent), ui(new Ui::TOSPage)  {
        LOG_INFO("Initializing TOSPage");
        ui->setupUi(this);
        connect(ui->backButton, &QPushButton::clicked, this, &TOSPage::onBack);
        connect(ui->nextButton, &QPushButton::clicked, this, &TOSPage::onNext);
        connect(ui->tosAcceptCheckbox, &QCheckBox::checkStateChanged, this, &TOSPage::onCheckBoxStateChanged);

        ui->nextButton->setDisabled(true);
        LOG_DEBUG("TOSPage initialized, buttons connected, next button disabled");
    }

    TOSPage::~TOSPage() {
        LOG_TRACE("Destroying TOSPage");
        delete ui;
    }


    void TOSPage::onBack() const {
        LOG_INFO("User clicked 'Back' on TOSPage");
        reinterpret_cast<InstallerWindow *>(window())->decrementPage();
    }

    void TOSPage::onNext() const {
        LOG_INFO("User clicked 'Next' on TOSPage (TOS accepted)");
        reinterpret_cast<InstallerWindow *>(window())->incrementPage();
    }

    void TOSPage::onCheckBoxStateChanged(const Qt::CheckState state) const {
        const bool isChecked = (state == Qt::Checked);
        LOG_DEBUG(std::format("TOS checkbox state changed: {}", isChecked ? "checked" : "unchecked"));
        ui->nextButton->setDisabled(!isChecked);
    }
} // ObsidianInstaller
