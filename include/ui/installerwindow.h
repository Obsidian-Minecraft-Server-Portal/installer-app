#pragma once

#ifndef OBISIAN_INSTALLER_INSTALLERWINDOW_H
#define OBISIAN_INSTALLER_INSTALLERWINDOW_H

#include <QMainWindow>

namespace ObsidianInstaller {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class InstallerWindow;
    }

    QT_END_NAMESPACE

    class InstallerWindow : public QMainWindow {
        Q_OBJECT

    public:
        explicit InstallerWindow(QWidget *parent = nullptr);
        ~InstallerWindow() override;

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;

    private:
        bool isDragging = false;
        QPoint dragPosition;
        Ui::InstallerWindow *ui;
        void loadStyleSheet();
        static void loadFonts();
    };
} // ObsidianInstaller

#endif //OBISIAN_INSTALLER_INSTALLERWINDOW_H