#include "mainwindow.h"
#include <QApplication>
#include<QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load(":/translations/DataAnaly_zh.qm");
    a.installTranslator(&translator);
    MainWindow w;
    w.setMinimumSize(600,600);
    w.show();

    return a.exec();
}
