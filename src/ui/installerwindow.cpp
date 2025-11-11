#include "ui/installerwindow.h"
#include "ui_InstallerWindow.h"
#include "ui/pages/tospage.h"
#include "logger.h"
#include <QFile>
#include <QFontDatabase>
#include <QMouseEvent>
#include <format>

namespace ObsidianInstaller {
    InstallerWindow::InstallerWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::InstallerWindow) {
        LOG_INFO("Initializing InstallerWindow");
#ifdef Q_OS_WIN
        LOG_DEBUG("Setting up frameless window for Windows");
        setWindowFlags(Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
#endif
        ui->setupUi(this);
        LOG_TRACE("UI setup complete");

        loadStyleSheet();
        loadFonts();

#ifdef Q_OS_WIN
        connect(ui->closeButton, &QPushButton::clicked, this, &InstallerWindow::close);
        LOG_TRACE("Close button connected");
#endif
        LOG_INFO("InstallerWindow initialized successfully");
    }

    InstallerWindow::~InstallerWindow() {
        LOG_INFO("Destroying InstallerWindow");
        delete ui;
    }

    void InstallerWindow::setPage(const int index) const {
        LOG_INFO(std::format("Setting page to index: {}", index));
        ui->stackedWidget->setCurrentIndex(index);
    }

    int InstallerWindow::getPage() const {
        const int currentPage = ui->stackedWidget->currentIndex();
        LOG_TRACE(std::format("Current page index: {}", currentPage));
        return currentPage;
    }

    void InstallerWindow::incrementPage() const {
        const int newPage = getPage() + 1;
        LOG_DEBUG(std::format("Incrementing page to: {}", newPage));
        setPage(newPage);
    }


    void InstallerWindow::decrementPage() const {
        const int newPage = getPage() - 1;
        LOG_DEBUG(std::format("Decrementing page to: {}", newPage));
        setPage(newPage);
    }


    void InstallerWindow::mousePressEvent(QMouseEvent *event) {
        QMainWindow::mousePressEvent(event);
        if (event->button() == Qt::LeftButton && ui->titlebar->underMouse()) {
            LOG_TRACE("Window drag started");
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
            LOG_TRACE("Window drag ended");
            isDragging = false;
            event->accept();
        }
    }

    void InstallerWindow::loadStyleSheet() {
        LOG_DEBUG("Loading application stylesheet");
        QFile styleSheet(":/styles/base.qss");
        if (styleSheet.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&styleSheet);
            this->setStyleSheet(stream.readAll());
            styleSheet.close();
            LOG_INFO("Stylesheet loaded successfully");
        } else {
            LOG_ERROR("Failed to open stylesheet file: :/styles/base.qss");
        }
    }

    void InstallerWindow::loadFonts() {
        LOG_DEBUG("Loading application fonts");
        int fontId1 = QFontDatabase::addApplicationFont(":/fonts/Minecraft-Tenv2.ttf");
        int fontId2 = QFontDatabase::addApplicationFont(":/fonts/Minecraft-Seven_v2.ttf");

        if (fontId1 != -1) {
            LOG_INFO(std::format("Loaded font: Minecraft-Tenv2.ttf (ID: {})", fontId1));
        } else {
            LOG_WARN("Failed to load font: Minecraft-Tenv2.ttf");
        }

        if (fontId2 != -1) {
            LOG_INFO(std::format("Loaded font: Minecraft-Seven_v2.ttf (ID: {})", fontId2));
        } else {
            LOG_WARN("Failed to load font: Minecraft-Seven_v2.ttf");
        }
    }
} // ObsidianInstaller
