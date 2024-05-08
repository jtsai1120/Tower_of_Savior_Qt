#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QPushButton>
#include <QMouseEvent>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:



private slots:
    void update_frame(); // 畫面更新
    void mousePressEvent(QMouseEvent *event);


private:
    QGraphicsView *view = new QGraphicsView;
    QGraphicsScene *cur_scene;
    QGraphicsScene game_scene;

    void game_init();

    QTimer *update_timer;



};

#endif // MAINWINDOW_H
