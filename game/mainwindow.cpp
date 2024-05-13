#include "mainwindow.h"
#include <QDebug>
#include <QCursor>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(540, 960);
    setMouseTracking(true);

    bg = new Bg(this);
    icon_bar = new Icon_bar(this);

    seed = chrono::system_clock::now().time_since_epoch().count();
    gen.seed(seed);
    dist = uniform_int_distribution<int>(0,5);

    runestones.resize(5, vector<Runestone*>(6));
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            delete runestones[i][j];
            runestones[i][j] = new Runestone(this, i, j, random_runestone_color());
        }
    }
    // 讓盤面先經過消珠直到不會有任3顆連線

    can_move_runestone = true;
    runestone_selected = false; // is cursor select a runestone ?
    runestone_swap = false;
    runestone_drift = false;
    ms_elapsed = 0;
    drift_timer = new QTimer(this);
    drift_timer->setInterval(1);
    connect(drift_timer, &QTimer::timeout, [&]() {
        ms_elapsed++;
        update();
    });
    drift_timer_started = false;

    hp = 2000;

    combo_cd = new QTimer;
    connect(combo_cd, SIGNAL(timeout()), this, SLOT(combo_eliminate()));

    drop_timer = new QTimer;
    connect(drop_timer, SIGNAL(timeout()), this, SLOT(drop_trigger()));

    combo_text = new QLabel(this);
    QFont combo_text_font("Consolas", 35, QFont::Bold);
    combo_text->setFont(combo_text_font);
    combo_text->setStyleSheet("color: yellow");
    combo_text->resize(300, 200);
    combo_text->move(540-300, 960-200);
    combo_text->hide();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    if (runestone_drift) {
        qDebug() << ms_elapsed;
        if (ms_elapsed>=time_limit*1000) {
            // 第二種結束方式： 倒數時間到 (第一種結束方式： 放開 Cursor)
            drift_timer->stop();
            drift_timer_started = false;
            runestone_drift = false;
            runestone_selected = false;
            can_move_runestone = false;
            icon_bar->change_status("hp", 1.0-double(hp)/2000.0);
            runestones[selected_runestone.first][selected_runestone.second]->move(selected_runestone.first, selected_runestone.second);
            // 計算 Combo
            qDebug() << "call combo_count_and_drop (timeup)";
            combo_count_and_drop(true);
        }
        // 依時間改變 cd_bar 長度
        icon_bar->change_status("cd" , double(ms_elapsed)/double(time_limit*1000));

    }
}

QString MainWindow::random_runestone_color() {
    switch(dist(gen)) {
    case 0:
        return "dark";
    case 1:
        return "earth";
    case 2:
        return "fire";
    case 3:
        return "heart";
    case 4:
        return "light";
    case 5:
        return "water";
    }
    // error
    return "empty";
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
            drift_timer->stop();
            drift_timer_started = false;
            runestone_drift = false;
            runestone_selected = false;
            can_move_runestone = false;
            icon_bar->change_status("hp", 1.0-double(hp)/2000.0);
            runestones[selected_runestone.first][selected_runestone.second]->move(selected_runestone.first, selected_runestone.second);
            // 計算 Combo
            qDebug() << "call combo_count_and_drop (mouseRelease)";
            combo_count_and_drop(true);
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (runestone_selected) {
        runestone_swap = ((event->y()-510)/90!=selected_runestone.first || event->x()/90!=selected_runestone.second);
        if (runestone_swap) {
            //qDebug() << "swap";
            QSound::play(":/dataset/close_hi_hat.wav");
            QString tmp = runestones[(event->y()-510)/90][event->x()/90]->get_color();
            runestones[(event->y()-510)/90][event->x()/90]->change_color(runestones[selected_runestone.first][selected_runestone.second]->get_color());
            runestones[selected_runestone.first][selected_runestone.second]->change_color(tmp);
            runestones[selected_runestone.first][selected_runestone.second]->move(selected_runestone.first, selected_runestone.second);
            selected_runestone = make_pair((event->y()-510)/90, event->x()/90); 
            if (!drift_timer_started) { // 開始計時
                ms_elapsed = 0; // 計時器 (經過多少 ms)
                qDebug() << "start countdown";
                runestone_drift = true;
                drift_timer_started = true;
                drift_timer->start();
            }
        }
        runestones[selected_runestone.first][selected_runestone.second]->stick_cursor(event->x(), event->y());
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

void MainWindow::combo_count_and_drop(bool is_first_count) {
    if (is_first_count) {
        // Reset
        qDebug() << "reset combo count";
        combo = 0;
        combo_count();
    } else {
        for (int i = 0; i < cur_pair_num; i++) delete light_halo_vfxs[i];
        light_halo_vfxs.clear();
        if (combo_counter.count(runestones).empty()) {
            qDebug() << "final combo : " << combo;
            combo_text->hide();
            can_move_runestone = true;
            return;
        } else combo_count();
    }
}

void MainWindow::combo_count() {
    combo_counter_result = combo_counter.count(runestones);
    cur_stage_combo = combo_counter_result.size();
    if (cur_stage_combo == 0) {
        qDebug() << "No Combo";
        can_move_runestone = true;
        return;
    }
    combo_text->show();
    cur_pair_num = 0;
    combo_eliminate();
    combo_cd->start(390);
}

void MainWindow::combo_eliminate() {
    if (cur_pair_num == cur_stage_combo) {
        combo_cd->stop();
        drop_detect();
        return;
    }
    Runestone_pair cur_pair = combo_counter_result[cur_pair_num];
    for (pair<int,int> pii : cur_pair.pair)
        runestones[pii.first][pii.second]->change_color("empty");
    switch (cur_pair_num) {
    case 0:
        QSound::play(":/dataset/combo_sound/combo1.wav");
        break;
    case 1:
        QSound::play(":/dataset/combo_sound/combo2.wav");
        break;
    case 2:
        QSound::play(":/dataset/combo_sound/combo3.wav");
        break;
    case 3:
        QSound::play(":/dataset/combo_sound/combo4.wav");
        break;
    case 4:
        QSound::play(":/dataset/combo_sound/combo5.wav");
        break;
    case 5:
        QSound::play(":/dataset/combo_sound/combo6.wav");
        break;
    case 6:
        QSound::play(":/dataset/combo_sound/combo7.wav");
        break;
    case 7:
        QSound::play(":/dataset/combo_sound/combo8.wav");
        break;
    case 8:
        QSound::play(":/dataset/combo_sound/combo9.wav");
        break;
    default:
        QSound::play(":/dataset/combo_sound/combo10.wav");
    }
    light_halo_vfxs.push_back(new Light_halo_vfx(this));
    light_halo_vfxs[cur_pair_num]->show(cur_pair);
    cur_pair_num++;
    combo++;
    combo_text->setText(QString::number(combo) + " Combo");
}

void MainWindow::drop_detect() {
    col_bottom = vector<int>(6, 4);
    drop_interval = 187;
    drop_timer->start(drop_interval);
    drop_trigger();
}

void MainWindow::drop_trigger() {
    drop_timer->stop();
    if (std::count(col_bottom.begin(), col_bottom.end(), -1)==6) {
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 6; j++)
                runestones[i][j]->move(i, j);
        combo_count_and_drop(false); // count combo and drop again
        return;
    }
    for (int col = 0; col < 6; col++) {
        // 找最下面的空白珠
        while(col_bottom[col] >= 0 && runestones[col_bottom[col]][col]->get_color() != "empty") {
            col_bottom[col]--;
        }
        if (col_bottom[col] == -1) // 代表該 col 完全不用 drop
            continue;
        if (col_bottom[col] != 0) {// 如果是 0 就直接放新的隨機珠子就好了
            for (int row = col_bottom[col]; row > 0; row--) {
                runestones[row][col]->drop(runestones[row-1][col]->get_color(), drop_interval);
            }
        }
        runestones[0][col]->drop(random_runestone_color(), drop_interval);
    }
    drop_interval -= drop_acceleration;
    drop_timer->start(drop_interval);
}

