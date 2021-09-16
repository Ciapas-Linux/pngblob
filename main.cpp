#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint| Qt::MSWindowsFixedSizeDialogHint);
    //w.setStyleSheet("background-image:url(:null-byte.jpg)");
    w.setAttribute(Qt::WA_TranslucentBackground, true);
    w.setWindowIcon(QIcon(":appicon.ico"));
    w.show();

    return a.exec();
}
