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
#include <QMouseEvent>
#include <QLabel>
#include <QSound>
#include <vector>
#include <random>
#include "bg.h"
#include "icon_bar.h"
#include "runestone.h"
using namespace std;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void random_runestone();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Bg *bg; // battle_bg, runestone_bg, battle_bgm
    Icon_bar *icon_bar; // cd_icon, hp_icon, cd_bar, hp_bar

    vector<vector<Runestone*>> runestones;
    random_device rd;
    uniform_int_distribution<int> dist;

    bool can_move_runestone;
    pair<int,int> selected_runestone;
    bool runestone_selected;
    bool runestone_swap;
    bool runestone_drift;
    QTimer *drift_timer;
    bool drift_timer_started;
    int ms_elapsed;
    const int time_limit = 4;

    int hp;
};

#endif // MAINWINDOW_H
