#pragma once

#ifndef OBSIDIAN_INSTALLER_SETUPPAGE_H
#define OBSIDIAN_INSTALLER_SETUPPAGE_H

#include <QWidget>

namespace ObsidianInstaller {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class SetupPage;
    }

    QT_END_NAMESPACE

    class SetupPage : public QWidget {
        Q_OBJECT

    public:
        explicit SetupPage(QWidget *parent = nullptr);

        ~SetupPage() override;

    private:
        QString installPath;
        Ui::SetupPage *ui;
        void onNext() const;
        void onBack() const;
        void onBrowse();

    };
} // ObsidianInstaller

#endif //OBSIDIAN_INSTALLER_SETUPPAGE_H