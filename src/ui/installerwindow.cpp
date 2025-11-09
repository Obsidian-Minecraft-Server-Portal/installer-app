#include "ui/installerwindow.h"
#include "ui_InstallerWindow.h"
#include <QFile>
#include <QFontDatabase>

namespace ObsidianInstaller {
    InstallerWindow::InstallerWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::InstallerWindow) {
        ui->setupUi(this);
        loadStyleSheet();
        loadFonts();
    }

    InstallerWindow::~InstallerWindow() {
        delete ui;
    }

    void InstallerWindow::loadStyleSheet() {
        QFile styleSheet(":/styles/base.qss");
        if (styleSheet.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&styleSheet);
            this->setStyleSheet(stream.readAll());
            styleSheet.close();
        }
    }

    void InstallerWindow::loadFonts() {
        QFontDatabase::addApplicationFont(":/fonts/Minecraft-Tenv2.ttf");
        QFontDatabase::addApplicationFont(":/fonts/Minecraft-Seven_v2.ttf");
    }

} // ObsidianInstaller
