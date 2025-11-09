#include "ui/components/markdownrenderer.h"

#include <QRegularExpression>
#include <QFont>

#include "ui_MarkdownRenderer.h"

namespace ObsidianInstaller {
    MarkdownRenderer::MarkdownRenderer(QWidget *parent) : QWidget(parent), ui(new Ui::MarkdownRenderer) {
        ui->setupUi(this);

        // Set default font
        QFont font("Minecraft Seven v2", 12);
        ui->textEdit->setFont(font);
    }

    MarkdownRenderer::~MarkdownRenderer() {
        delete ui;
    }

    void MarkdownRenderer::setText(const QString &text) {
        const QString html = parseMarkdown(text);
        ui->textEdit->setHtml(html);
    }

    QString MarkdownRenderer::parseMarkdown(const QString &markdown) {
        QString html;
        const QStringList lines = markdown.split('\n');

        bool inList = false;

        for (const QString &line: lines) {
            if (line.trimmed().isEmpty()) {
                if (inList) {
                    html += "</ul>";
                    inList = false;
                }
                html += "<br>";
                continue;
            }

            QString processedLine = line;

            // Handle headers (must be done before inline formatting)
            if (processedLine.startsWith("# ")) {
                if (inList) {
                    html += "</ul>";
                    inList = false;
                }
                processedLine = processedLine.mid(2);
                processedLine = processInlineFormatting(processedLine);
                html += QString("<h1 style='font-size: 18pt; font-weight: bold; margin: 10px 0;'>%1</h1>").arg(processedLine);
                continue;
            } else if (processedLine.startsWith("## ")) {
                if (inList) {
                    html += "</ul>";
                    inList = false;
                }
                processedLine = processedLine.mid(3);
                processedLine = processInlineFormatting(processedLine);
                html += QString("<h2 style='font-size: 16pt; font-weight: bold; margin: 10px 0;'>%1</h2>").arg(processedLine);
                continue;
            } else if (processedLine.startsWith("### ")) {
                if (inList) {
                    html += "</ul>";
                    inList = false;
                }
                processedLine = processedLine.mid(4);
                processedLine = processInlineFormatting(processedLine);
                html += QString("<h3 style='font-size: 14pt; font-weight: bold; margin: 10px 0;'>%1</h3>").arg(processedLine);
                continue;
            } else if (processedLine.startsWith("#### ")) {
                if (inList) {
                    html += "</ul>";
                    inList = false;
                }
                processedLine = processedLine.mid(5);
                processedLine = processInlineFormatting(processedLine);
                html += QString("<h4 style='font-size: 12pt; font-weight: bold; margin: 10px 0;'>%1</h4>").arg(processedLine);
                continue;
            }

            // Handle lists
            if (processedLine.trimmed().startsWith("- ") || processedLine.trimmed().startsWith("* ")) {
                if (!inList) {
                    html += "<ul style='margin: 5px 0; padding-left: 20px;'>";
                    inList = true;
                }
                processedLine = processedLine.trimmed().mid(2);
                processedLine = processInlineFormatting(processedLine);
                html += QString("<li style='margin: 5px 0;'>%1</li>").arg(processedLine);
                continue;
            }

            // Close list if we're in one
            if (inList) {
                html += "</ul>";
                inList = false;
            }

            // Regular paragraph
            processedLine = processInlineFormatting(processedLine);
            html += QString("<p style='margin: 10px 0;'>%1</p>").arg(processedLine);
        }

        // Close list if still open
        if (inList) {
            html += "</ul>";
        }

        // Wrap in styled div
        return QString("<div style='font-family: \"Minecraft Seven v2\", monospace; font-size: 12pt;'>%1</div>").arg(html);
    }

    QString MarkdownRenderer::processInlineFormatting(QString text) {
        // Escape HTML entities first
        text = text.toHtmlEscaped();

        // Images: ![alt](url) - must be before links
        QRegularExpression imgRegex(R"(!\[([^\]]*)\]\(([^\)]+)\))");
        text.replace(imgRegex, R"(<img src="\2" alt="\1" style="max-width: 100%; height: auto;">)");

        // Links: [text](url)
        QRegularExpression linkRegex(R"(\[([^\]]+)\]\(([^\)]+)\))");
        text.replace(linkRegex, R"(<a href="\2" style="color: #4A9EFF; text-decoration: underline;">\1</a>)");

        // Bold: **text** or __text__
        QRegularExpression boldRegex(R"((\*\*|__)(.+?)\1)");
        text.replace(boldRegex, R"(<b>\2</b>)");

        // Italic: *text* or _text_ (must be after bold to avoid conflicts)
        QRegularExpression italicRegex(R"((?<!\*)(\*|_)(?!\*)(.+?)\1(?!\*))");
        text.replace(italicRegex, R"(<i>\2</i>)");

        // Inline code: `code`
        QRegularExpression codeRegex(R"(`([^`]+)`)");
        text.replace(codeRegex, R"(<code style="background-color: rgba(128, 128, 128, 0.2); padding: 2px 4px; font-family: monospace; border-radius: 3px;">\1</code>)");

        return text;
    }
} // ObsidianInstaller
