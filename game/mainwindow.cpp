#include "mainwindow.h"
#include <QDebug>
#include <QCursor>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(540, 960);
    setMouseTracking(true);

    bg = new Bg(this);
    icon_bar = new Icon_bar(this);

    runestones.resize(5, vector<Runestone*>(6));
    random_runestone();

    can_move_runestone = true;
    runestone_selected = false; // is cursor select a runestone ?
    runestone_swap = false;
    runestone_drift = false;
    ms_elapsed = 0;
    drift_timer = new QTimer;
    drift_timer_started = false;

    hp = 2000;

    light_halo_vfx = new Light_halo_vfx(this);

    combo_cd = new QTimer;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    if (runestone_drift) {
        // 依時間改變 cd_bar 長度
        icon_bar->change_status("cd" ,double(ms_elapsed)/double(time_limit*1000));
    }
}

void MainWindow::random_runestone() {
    auto seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    uniform_int_distribution<int> dist(0, 5);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            delete runestones[i][j];
            runestones[i][j] = new Runestone(this, i, j, dist(gen));
        }
    }
    // 讓盤面先經過消珠直到不會有任3顆連線
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (event->y() >= 510) {
            if (can_move_runestone) {
                runestone_selected = true;
                selected_runestone = make_pair((event->y()-510)/90, event->x()/90);
                runestones[selected_runestone.first][selected_runestone.second]->stick_cursor(event->x(), event->y());
            }
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        runestone_selected = false;
        runestones[selected_runestone.first][selected_runestone.second]->move(selected_runestone.first, selected_runestone.second);
        if (runestone_drift) {
            // 第一種結束方式： 放開 Cursor (第二種結束方式： 倒數時間到)
            runestone_drift = false;
            runestone_selected = false;
            can_move_runestone = false;
            icon_bar->change_status("hp", 1.0-double(hp)/2000.0);
            runestones[selected_runestone.first][selected_runestone.second]->move(selected_runestone.first, selected_runestone.second);
            // 計算 Combo
            combo_count();
        }
    }
}

void MainWindow::combo_count() {
    combo_counter_result = combo_counter.count(runestones);
    qDebug() << "Combo:" << combo_counter_result.size();
    cur_pair_num = 0;
    connect(combo_cd, &QTimer::timeout, [&]{
        qDebug() << cur_pair_num;
        Runestone_pair cur_pair = combo_counter_result[cur_pair_num];
        for (pair<int,int> pii : cur_pair.pair)
            runestones[pii.first][pii.second]->change_color("empty");
        //light_halo_vfx->show(cur_pair);
        cur_pair_num++;
        if (cur_pair_num == int(combo_counter_result.size())) {
            combo_cd->stop();
        }
    });
    combo_cd->start(500);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (ms_elapsed>=time_limit*1000) {
        ms_elapsed = 0;
        qDebug() << "timeup";
        drift_timer->stop();
        drift_timer_started = false;
        // 第二種結束方式： 倒數時間到 (第一種結束方式： 放開 Cursor)
        runestone_drift = false;
        runestone_selected = false;
        can_move_runestone = false;
        icon_bar->change_status("hp", 1.0-double(hp)/2000.0);
        runestones[selected_runestone.first][selected_runestone.second]->move(selected_runestone.first, selected_runestone.second);
        // 呼叫計算 Combo 的函數
    }
    if (runestone_selected) {
        runestone_swap = ((event->y()-510)/90!=selected_runestone.first || event->x()/90!=selected_runestone.second);
        if (runestone_swap) {
            QSound::play(":/dataset/close_hi_hat.wav");
            QString tmp = runestones[(event->y()-510)/90][event->x()/90]->get_color();
            runestones[(event->y()-510)/90][event->x()/90]->change_color(runestones[selected_runestone.first][selected_runestone.second]->get_color());
            runestones[selected_runestone.first][selected_runestone.second]->change_color(tmp);
            runestones[selected_runestone.first][selected_runestone.second]->move(selected_runestone.first, selected_runestone.second);
            selected_runestone = make_pair((event->y()-510)/90, event->x()/90); 
        }
        runestones[selected_runestone.first][selected_runestone.second]->stick_cursor(event->x(), event->y());
        if (!drift_timer_started && runestone_swap) {
            // 開始計時
            drift_timer->start(1); // 每 1ms 觸發器
            ms_elapsed = 0; // 計時器 (經過多少 ms)
            connect(drift_timer, &QTimer::timeout, [&]() {
                ms_elapsed++;
                update();
            });
            qDebug() << "start countdown";
            runestone_drift = true;
            drift_timer_started = true;
        }
    }

    if (runestone_drift) {
        // 使 Cursor 無法移動到畫面外導致 Crash
        QRect windowRect = this->geometry();
        const int de_unit = 45;
        if (event->globalX() < windowRect.left() + de_unit && event->globalY() < windowRect.top() + 510 + de_unit) {
            QCursor::setPos(QPoint(windowRect.left() + de_unit, windowRect.top() + 510 + de_unit));
        } else if (event->globalX() < windowRect.left() + de_unit && event->globalY() > windowRect.bottom() - de_unit) {
            QCursor::setPos(QPoint(windowRect.left() + de_unit, windowRect.bottom() - de_unit));
        } else if (event->globalX() > windowRect.right() - de_unit && event->globalY() < windowRect.top() + 510 + de_unit) {
            QCursor::setPos(QPoint(windowRect.right() - de_unit, windowRect.top() + 510 + de_unit));
            qDebug() << "err";
        } else if (event->globalX() > windowRect.right() - de_unit && event->globalY() > windowRect.bottom() - de_unit) {
            QCursor::setPos(QPoint(windowRect.right() - de_unit, windowRect.bottom() - de_unit));
        } else {
            if (event->globalX() < windowRect.left() + de_unit) QCursor::setPos(QPoint(windowRect.left() + de_unit, event->globalY()));
            if (event->globalX() > windowRect.right() - de_unit) QCursor::setPos(QPoint(windowRect.right() - de_unit, event->globalY()));
            if (event->globalY() < windowRect.top() + 510 + de_unit) QCursor::setPos(QPoint(event->globalX(), windowRect.top() + 510 + de_unit));
            if (event->globalY() > windowRect.bottom() - de_unit) QCursor::setPos(QPoint(event->globalX(), windowRect.bottom() - de_unit));
        }
    }
}
