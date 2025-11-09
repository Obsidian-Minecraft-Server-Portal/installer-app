#include <QApplication>
#include "ui/installerwindow.h"
using namespace ObsidianInstaller;


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    InstallerWindow window;
    window.show();
    return QApplication::exec();
}
