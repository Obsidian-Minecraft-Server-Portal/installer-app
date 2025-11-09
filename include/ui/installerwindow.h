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

    private:
        Ui::InstallerWindow *ui;
        void loadStyleSheet();
        static void loadFonts();
        HRESULT UpdateWindows11RoundCorners();
    };
} // ObsidianInstaller

#endif //OBISIAN_INSTALLER_INSTALLERWINDOW_H