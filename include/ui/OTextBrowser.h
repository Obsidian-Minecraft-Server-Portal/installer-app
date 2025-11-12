#pragma once

#include <QTextBrowser>
#include <QWheelEvent>

class OTextBrowser : public QTextBrowser {
    Q_OBJECT
public:
    explicit OTextBrowser(QWidget *parent = nullptr) : QTextBrowser(parent) {}

protected:
    void wheelEvent(QWheelEvent *event) override {
        if (event->modifiers() & Qt::ControlModifier) {
            event->ignore();
            return;
        }
        QTextBrowser::wheelEvent(event);
    }
};
