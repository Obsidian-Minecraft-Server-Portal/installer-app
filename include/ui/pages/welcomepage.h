#pragma once

#ifndef OBISIAN_INSTALLER_WELCOMEPAGE_H
#define OBISIAN_INSTALLER_WELCOMEPAGE_H

#include <QWidget>

namespace ObsidianInstaller {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class WelcomePage;
    }

    QT_END_NAMESPACE

    class WelcomePage : public QWidget {
        Q_OBJECT

    public:
        explicit WelcomePage(QWidget *parent = nullptr);

        ~WelcomePage() override;

    private:
        Ui::WelcomePage *ui;
    };
} // ObsidianInstaller

#endif //OBISIAN_INSTALLER_WELCOMEPAGE_H