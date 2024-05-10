#include "mainwindow.h"
//#include "EventFilter.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Tower of Savior");
    //EventFilter ef;
    //w.installEventFilter(&ef);
    w.show();
    return a.exec();
}
