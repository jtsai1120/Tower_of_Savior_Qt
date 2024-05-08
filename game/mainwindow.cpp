#include "mainwindow.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 設定 view 視窗相關設定
    view->setFixedSize(540, 960);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 設定畫面更新
    update_timer = new QTimer(this);
    update_timer->start(1);
    connect(update_timer, SIGNAL(timeout()), this, SLOT(update_frame()));

    // temp
    cur_scene = &game_scene;
}

void MainWindow::update_frame() {
    view->setScene(cur_scene);
    this->setCentralWidget(view);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == cur_scene) {
        // Press Event
        QGraphicsSceneMouseEvent *mouseSceneEvent;
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            mouseSceneEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
            qDebug() << mouseSceneEvent->scenePos()
                     << mouseSceneEvent->lastScenePos();
        }
    }
    return MainWindow::eventFilter(watched, event);
}







