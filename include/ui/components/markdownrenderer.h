#pragma once

#ifndef OBSIDIAN_INSTALLER_MARKDOWNRENDERER_H
#define OBSIDIAN_INSTALLER_MARKDOWNRENDERER_H

#include <QWidget>

namespace ObsidianInstaller {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class MarkdownRenderer;
    }

    QT_END_NAMESPACE

    class MarkdownRenderer final : public QWidget {
        Q_OBJECT

    public:
        explicit MarkdownRenderer(QWidget *parent = nullptr);

        ~MarkdownRenderer() override;

        void setText(const QString &text);

    private:
        Ui::MarkdownRenderer *ui;

        QString parseMarkdown(const QString &markdown);
        QString processInlineFormatting(QString text);
    };
} // ObsidianInstaller

#endif //OBSIDIAN_INSTALLER_MARKDOWNRENDERER_H
