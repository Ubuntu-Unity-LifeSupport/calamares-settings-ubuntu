#include "mainwindow.h"

#include <QApplication>
#include <QGuiApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString wallpaperFile;

    if (argc > 1) {
        wallpaperFile = QString(argv[1]);
    } else {
        return 1;
    }

    for (QScreen *screen : QApplication::screens()) {
        MainWindow *w = new MainWindow(wallpaperFile);
        // On X11, a desktop window that never takes focus: the window manager
        // keeps it below every other window. A fullscreen window goes into the
        // fullscreen layer whenever it has focus - when it maps after
        // Calamares, or on Alt+Tab - and covers the installer.
        bool x11 = QGuiApplication::platformName() == QLatin1String("xcb");
        if (x11) {
            w->setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);
            w->setWindowFlags(Qt::Window | Qt::FramelessWindowHint
                              | Qt::WindowStaysOnBottomHint
                              | Qt::WindowDoesNotAcceptFocus);
        } else {
            w->setWindowFlags(Qt::WindowStaysOnBottomHint);
        }
        w->setGeometry(screen->geometry());
        if (!x11)
            w->showFullScreen();
        w->show();
        w->applyWallpaper();
    }

    return a.exec();
}
