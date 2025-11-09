#include "ui/installerwindow.h"
#include "ui_InstallerWindow.h"
#include <QFile>
#include <QFontDatabase>
#include <QMouseEvent>
#include <QMessageBox>

namespace ObsidianInstaller {
    InstallerWindow::InstallerWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::InstallerWindow) {
#ifdef Q_OS_WIN
        setWindowFlags(Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
#endif
        ui->setupUi(this);
        loadStyleSheet();
        loadFonts();
#ifdef Q_OS_WIN
        connect(ui->closeButton, &QPushButton::clicked, this, &InstallerWindow::close);
#endif
        setPage(0);
    }

    InstallerWindow::~InstallerWindow() {
        delete ui;
    }

    void InstallerWindow::setPage(const int index) const {
        ui->stackedWidget->setCurrentIndex(index);
    }

    int InstallerWindow::getPage() const {
        if (!ui) {
            qDebug() << "UI is null";
            return 0;
        }
        if (!ui->stackedWidget) {
            qDebug() << "Stacked widget is null";
            return 0;
        }

        if (ui->stackedWidget->currentIndex() < 0) {
            qDebug() << "Current index is less than 0";
            return 0;
        }

        return ui->stackedWidget->currentIndex();
    }

    void InstallerWindow::incrementPage() const {
        setPage(getPage() + 1);
    }


    void InstallerWindow::decrementPage() const {
        setPage(getPage() - 1);
    }

    void InstallerWindow::mousePressEvent(QMouseEvent *event) {
        QMainWindow::mousePressEvent(event);
        if (event->button() == Qt::LeftButton && ui->titlebar->underMouse()) {
            isDragging = true;
            dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            event->accept();
        }
    }

    void InstallerWindow::mouseMoveEvent(QMouseEvent *event) {
        QMainWindow::mouseMoveEvent(event);

        if (isDragging && event->buttons() & Qt::LeftButton) {
            move(event->globalPosition().toPoint() - dragPosition);
            event->accept();
        }
    }

    void InstallerWindow::mouseReleaseEvent(QMouseEvent *event) {
        QMainWindow::mouseReleaseEvent(event);
        if (event->button() == Qt::LeftButton) {
            isDragging = false;
            event->accept();
        }
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
