#pragma once

#ifndef OBSIDIAN_INSTALLER_TOSPAGE_H
#define OBSIDIAN_INSTALLER_TOSPAGE_H

#include "ui_TOSPage.h"

namespace ObsidianInstaller {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class TOSPage;
    }

    QT_END_NAMESPACE

    class TOSPage final : public QWidget {
        Q_OBJECT

    public:
        explicit TOSPage(QWidget *parent = nullptr);

        ~TOSPage() override;
    private:
        Ui::TOSPage *ui;
        void onBack() const;
        void onNext() const;
        void onCheckBoxStateChanged(int state);
    };
} // ObsidianInstaller

#endif //OBSIDIAN_INSTALLER_TOSPAGE_H