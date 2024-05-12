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
#include <chrono>
#include "bg.h"
#include "icon_bar.h"
#include "runestone.h"
#include "runestone_pair.h"
#include "combo_counter.h"
#include "light_halo_vfx.h"
using namespace std;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void random_runestone();

    Combo_counter combo_counter;
    void combo_count();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Bg *bg; // battle_bg, runestone_bg, battle_bgm
    Icon_bar *icon_bar; // cd_icon, hp_icon, cd_bar, hp_bar

    vector<vector<Runestone*>> runestones;

    bool can_move_runestone;
    pair<int,int> selected_runestone;
    bool runestone_selected;
    bool runestone_swap;
    bool runestone_drift;
    QTimer *drift_timer;
    bool drift_timer_started;
    int ms_elapsed;
    const int time_limit = 10;

    int hp;

    Light_halo_vfx *light_halo_vfx;

    vector<Runestone_pair> combo_counter_result;
    int cur_pair_num;
    QTimer *combo_cd;
};

#endif // MAINWINDOW_H
