#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPixmap>
#include <QPushButton>
#include <QEvent>
#include <QPoint>
//#include "EventFilter.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:


private slots:
    void update_frame(); // 畫面更新


private:
    //EventFilter ef;

    QGraphicsView *view = new QGraphicsView;
    QGraphicsScene *cur_scene;
    QGraphicsScene game_scene;

    void game_init();

    QTimer *update_timer;

};

#endif // MAINWINDOW_H
