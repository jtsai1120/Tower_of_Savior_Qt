#include "mainwindow.h"
#include <QDebug>
#include "EventFilter.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 設定 view 視窗相關設定
    view->setFixedSize(540, 960);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 設定畫面更新
    update_timer = new QTimer(this);
    update_timer->start(1);
    connect(update_timer, SIGNAL(timeout()), this, SLOT(update_frame()));

    //this->setCentralWidget(view);
    //view->installEventFilter(this);
    cur_scene = &game_scene;
}

void MainWindow::update_frame() {
    view->setScene(cur_scene);
}







