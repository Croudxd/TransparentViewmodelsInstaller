#include "mainwindow.h"

#include <QApplication>
#include <direct.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


QString MainWindow::getSourcePath(){
    return QCoreApplication::applicationDirPath();
}
