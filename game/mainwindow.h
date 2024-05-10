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
#include <QDebug>
#include <QMouseEvent>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            qDebug() << "Mouse button released at position:" << mouseEvent->pos();
            // 处理鼠标释放事件
            return true; // 返回 true 表示事件已处理
        }

        return QMainWindow::eventFilter(watched, event);
    }
private slots:
    void update_frame(); // 畫面更新


private:
    QGraphicsView *view = new QGraphicsView;
    QGraphicsScene *cur_scene;
    QGraphicsScene game_scene;

    void game_init();

    QTimer *update_timer;

};

#endif // MAINWINDOW_H
