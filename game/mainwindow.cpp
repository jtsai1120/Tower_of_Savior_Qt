#include "mainwindow.h"
#include <QDebug>
#include <QCursor>
#include <algorithm>
#include <QGraphicsOpacityEffect>
#include <cstdlib>
#include <ctime>
#include <random>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(540, 960);
    setMouseTracking(true); // 曾經註解掉，但似乎沒有影響 ? by Huang
    // 設定遊戲狀態為初始畫面
    game_status = 0;
    //
    bg = new Bg(this);
    icon_bar = new Icon_bar(this, init_hp);

    // add start_button
    start_button = new QPushButton(this);
    start_button_pic.load(":/dataset/ui/start button.jpg");
    start_button->setIcon(QIcon(start_button_pic));
    start_button->setIconSize(start_button_pic.size());
    start_button_item = new ButtonItem(start_button);
    start_button->setFixedWidth(180);
    start_button->setFixedHeight(83);
    start_button->move(350,520);

    connect(start_button, SIGNAL(clicked()), this, SLOT(on_start_button_clicked()));

    // add ui button
    ui_button_pic.load(":/dataset/ui/empty button.jpg");
    ui_button = new QLabel(this);
    ui_button->setPixmap(ui_button_pic);
    ui_button->setFixedWidth(180);
    ui_button->setFixedHeight(83);
    ui_button->move(10, 520);

    ui_text = new QLabel(this);
    QFont ui_text_font("Consolas", 20, QFont::Bold);
    ui_text->setFont(ui_text_font);
    ui_text->setStyleSheet("color: white");
    ui_text->resize(200, 200);
    ui_text->move(32, 461);
    ui_text->show();
    ui_text->setText("Mission 1");

    // 頂部角色設置欄位
    charac_slots.resize(6);
    for (int i = 0; i < 6; i++) {
        delete charac_slots[i];
        charac_slots[i] = new Charac_slot(this);
        charac_slots[i]->charac_item->setFixedWidth(90);
        charac_slots[i]->charac_item->setFixedHeight(90);
        charac_slots[i]->charac_item->move(0 + i * 90, 360);
        charac_slots[i]->attack_text->move(25 + i * 90, 315);
    }

    //set enemy and hp
    enemy.resize(3);
    enemy_hp.resize(3);
    for (int i = 0; i < 3; i++){
        enemy[i] = new Enemy(this);
        enemy[i]->enemy_item->setFixedWidth(120);
        enemy[i]->enemy_item->setFixedHeight(120);
        enemy[i]->enemy_item->move(0,1000);
        enemy[i]->show(1,i);
        enemy_hp[i] = new Enemy_hp(this);
        enemy_hp[i]->hp_bar->move(0,1000);
    }

    // 其他初始化
    seed = chrono::system_clock::now().time_since_epoch().count();
    gen.seed(seed);
    dist = uniform_int_distribution<int>(0,5);
    //
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


    combo_cd = new QTimer;
    connect(combo_cd, SIGNAL(timeout()), this, SLOT(combo_eliminate()));

    icon_bar->heal_text->show();
    icon_bar->heal_text->setText("+" + QString::number(init_heal));

    // 結算時的暗下畫面
    darken = new QLabel(this);
    darken_pic.load(":/dataset/ui/darken.png");
    darken_pic = darken_pic.scaled(1000, 510);

    darken->setPixmap(darken_pic);
    darken->move(0, 1000);
    darken->setFixedSize(darken_pic.width(), darken_pic.height());

    // 設計成半透明
    // Create an opacity effect
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    // Set the desired opacity (0.0 = fully transparent, 1.0 = fully opaque)
    opacityEffect->setOpacity(0.5); // 50% transparency
    // Apply the opacity effect to the label
    darken->setGraphicsEffect(opacityEffect);

}

void MainWindow::on_start_button_clicked() {
    qDebug() << "Start Button Clicked!";

    // 至少要有隊長和副隊長才能開始
    if(charac_slots[0]->charac_ID == -1 || charac_slots[5]->charac_ID == -1)
        return;

    start_button->move(40, 1000); // 只是移出螢幕
    game_status = 1; // game start


    // 進入遊戲
    runestones.resize(5, vector<Runestone*>(6));
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            delete runestones[i][j];
            runestones[i][j] = new Runestone(this, i, j, random_runestone_color());
        }
    }

    for (int i = 0; i < 3; i++){
        enemy[i]->enemy_item->move(85 +  130*i, 200);
        enemy_hp[i]->hp_bar->move(100 +  133*i, 330);
    }

    ui_button->hide();
    ui_text->hide();
    ui_button->move(180, 250);
    ui_text->move(190, 200);

    combo_text = new QLabel(this);
    QFont combo_text_font("Consolas", 35, QFont::Bold);
    combo_text->setFont(combo_text_font);
    combo_text->setStyleSheet("color: yellow");
    combo_text->resize(300, 200);
    combo_text->move(540-300, 960-200);
    combo_text->hide();

    // 清除空的隊伍欄位
    init_heal = 0;
    harm = 0;
    for (int i = 0; i < 6; i++) {
        if(charac_slots[i]->charac_ID == -1)
            charac_slots[i]->charac_item->move(0, 1000); // 只是移出螢幕
        else {
            charac_slots[i]->attack_text->hide();
            // 結算選擇的角色的 init_heal 總值
            init_heal += charac_slots[i]->charac_heal[charac_slots[i]->charac_ID];
        }
    }

    // 基礎模式則置成1
    if (basic){
        init_heal = 1;
    }

    icon_bar->heal_text->hide();
    hp = init_hp;
    burn_road = false;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    if (runestone_drift) {
        qDebug() << ms_elapsed;
        if (ms_elapsed >= time_limit * 1000) {
            // 第二種結束方式： 倒數時間到 (第一種結束方式： 放開 Cursor)
            drift_timer->stop();
            drift_timer_started = false;
            runestone_drift = false;
            runestone_selected = false;
            can_move_runestone = false;
            icon_bar->change_status("hp", 1.0 - hp/init_hp);
            icon_bar->hp_text->show();
            icon_bar->hp_text->setText(QString::number(hp) + "/" + QString::number(init_hp));
            runestones[selected_runestone.first][selected_runestone.second]->move(selected_runestone.first, selected_runestone.second);
            // 計算 Combo
            qDebug() << "call combo_count_and_drop (timeup)";
            combo_count_and_drop(true);
        }
        else // 依時間改變 cd_bar 長度
            icon_bar->change_status("cd" , double(ms_elapsed) / double(time_limit * 1000));

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
    if (game_status == 0 && event->button() == Qt::LeftButton){
        // 更換角色
        if (event->y() >= 360 && event->y() <= 450) {
            charac_slots[event->x() / 90] -> change_charac(basic);
        }
        // 更換任務
        if (event->x() >= 10 && event->x() <= 190 && event->y() >= 520 && event->y() <= 603) {
            if (basic){
                basic = false;
                ui_text->setText("Mission 2");
            }
            else{
                basic = true;
                ui_text->setText("Mission 1");
            }
            for (int i = 0; i < int(charac_slots.size()); i++)
                if(charac_slots[i]->charac_ID != -1)
                    for (int j = 0; j < 5; j++)
                            charac_slots[i] -> change_charac(basic);
        }

        // 結算選擇的角色的 init_heal 總值
        init_heal = 0;
        for (int i = 0; i < 6; i++) {
            if(charac_slots[i]->charac_ID != -1){
                init_heal += charac_slots[i]->charac_heal[charac_slots[i]->charac_ID];
            }
        }
        // 基礎模式則置成1
        if (basic){
            init_heal = 1;
        }
        icon_bar->heal_text->setText("+" + QString::number(init_heal));

        return;
    }
    if (game_status == 1 && event->button() == Qt::LeftButton) {
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
    if (game_status == 0)
        return;
    if (game_status == 1 && event->button() == Qt::LeftButton) {
        runestone_selected = false;
        runestones[selected_runestone.first][selected_runestone.second]->move(selected_runestone.first, selected_runestone.second);
        if (runestone_drift) {
            // 第一種結束方式： 放開 Cursor (第二種結束方式： 倒數時間到)
            drift_timer->stop();
            drift_timer_started = false;
            runestone_drift = false;
            runestone_selected = false;
            can_move_runestone = false;
            icon_bar->change_status("hp", 1.0 - hp/init_hp);
            icon_bar->hp_text->show();
            icon_bar->hp_text->setText(QString::number(hp) + "/" + QString::number(init_hp));
            runestones[selected_runestone.first][selected_runestone.second]->move(selected_runestone.first, selected_runestone.second);
            // 計算 Combo
            qDebug() << "call combo_count_and_drop (mouseRelease)";
            combo_count_and_drop(true);
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (game_status == 0)
        return;
    if (game_status == 1 && runestone_selected) {
        runestone_swap = ((event->y()-510)/90!=selected_runestone.first || event->x()/90!=selected_runestone.second);
        if (runestone_swap) {
            //qDebug() << "swap";
            QSound::play(":/dataset/close_hi_hat.wav");
            QString tmp = runestones[(event->y()-510)/90][event->x()/90]->get_color();

            // 判定燃燒
            bool burned = false; // 0 = not burned, 1 = burned, 2 = burned before
            bool burned_before = false; // 0 = not burned, 1 = burned before


            if (runestones[(event->y()-510)/90][event->x()/90]->status == 1){
                harm += 100;
                hp = hp - 100;
                icon_bar->heal_text->setStyleSheet("color: red");
                icon_bar->heal_text->show();
                icon_bar->heal_text->setText("-" + QString::number(harm));
                burned = true;
            }
            if (runestones[selected_runestone.first][selected_runestone.second]->status == 1){
                burned_before = true;
            }

            if (runestones[(event->y()-510)/90][event->x()/90]->status == 2 || runestones[selected_runestone.first][selected_runestone.second]->status == 2){
                harm += 100;
                hp = hp - 100;
                icon_bar->heal_text->setStyleSheet("color: red");
                icon_bar->heal_text->show();
                icon_bar->heal_text->setText("-" + QString::number(harm));

                runestones[(event->y()-510)/90][event->x()/90]->change_color(runestones[selected_runestone.first][selected_runestone.second]->get_color(), 0);
                runestones[selected_runestone.first][selected_runestone.second]->change_color(tmp, 0);

                // 第三種結束方式： 摸到風化 (第三種結束方式： 摸到風化)
                drift_timer->stop();
                drift_timer_started = false;
                runestone_drift = false;
                runestone_selected = false;
                can_move_runestone = false;
                icon_bar->change_status("hp", 1.0 - hp/init_hp);
                icon_bar->hp_text->show();
                icon_bar->hp_text->setText(QString::number(hp) + "/" + QString::number(init_hp));
                runestones[selected_runestone.first][selected_runestone.second]->move(selected_runestone.first, selected_runestone.second);
                // 計算 Combo
                qDebug() << "call combo_count_and_drop (touch weathered)";
                combo_count_and_drop(true);
                return;
            }

            runestones[(event->y()-510)/90][event->x()/90]->change_color(runestones[selected_runestone.first][selected_runestone.second]->get_color(), 0);

            if (burned)
                runestones[(event->y()-510)/90][event->x()/90]->change_color(runestones[selected_runestone.first][selected_runestone.second]->get_color(), 1);

            if (burned_before)
                runestones[selected_runestone.first][selected_runestone.second]->change_color(tmp, 1);
            else
                runestones[selected_runestone.first][selected_runestone.second]->change_color(tmp, 0);

            // 燃燒軌跡
            if (burn_road) {
                runestones[selected_runestone.first][selected_runestone.second]->change_color(tmp, 1);
            }

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

void MainWindow::combo_count_and_drop(bool is_first_count) { // 回合計算在這裡
    if (is_first_count) {
        // Reset
        qDebug() << "reset combo count";
        combo = 0;
        combo_count();
    } else if (game_status == 1){ // 準備攻擊及回血
        for (int i = 0; i < cur_pair_num; i++) delete light_halo_vfxs[i];
        light_halo_vfxs.clear();
        QTimer::singleShot(105, [&](){
            if (combo_counter.count(runestones).empty()) {
                qDebug() << "final combo : " << combo;
                combo_text->hide();
                can_move_runestone = true;

                // 關閉燃燒路徑的位置
                for (int i = 0; i < int(runestones.size()); i++){
                    for (int j = 0; j < int(runestones[i].size()); j++){
                        if (runestones[i][j]->status == 1) runestones[i][j]->change_color(runestones[i][j]->get_color(), 0);
                    }
                }
                // 還原本來就有燃燒的位置
                for (int i = 0; i < int(burning.size()); i++){
                    runestones[burning[i].first][burning[i].second]->change_color(runestones[burning[i].first][burning[i].second]->get_color(), 1);
                }

                // 暗下畫面
                darken->move(0, 510);
                darken->raise();

                // 我方攻擊階段
                game_status = 2;
                combo_cd->start(150);

            } else combo_count();
        });
    } else if (game_status == 2){ // 輪流攻擊
        if (attack_wait_count > 5){ // 進入下個wave
            if (enemy[0]->dead && enemy[1]->dead && enemy[2]->dead && attack_wait_count == 6){
                ui_button->show();
                ui_text->show();
                ui_text->setText("Wave 0" + QString::number(level + 1) + "/ 03");
                if (level == 3){ // 結束了
                    ui_text->setText("  VICTORY!");
                }
                combo_cd->start(800);
                attack_wait_count++;
                for (int i = 0; i < 3; i++){
                    enemy[i]->enemy_item->hide();
                }
            }
            else{
                game_status = 3;
                combo_count_and_drop(false); // 交給下個階段的自己
            }
            return;
        }
        if (attack_wait_count == -1){ // 不攻擊，乘上combo數
            attack_wait_count ++;
            for(int i = 0; i < int(charac_slots.size()); i++){
                if (charac_slots[i]->attack == 0) continue;
                charac_slots[i]->attack *= combo;
                charac_slots[i]->attack_text->setText(QString::number(charac_slots[i]->attack));
            }
            heal *= combo;
            icon_bar->heal_text->setText("+" + QString::number(heal));

            combo_cd->start(90);
            return;
        }

        // 根據 attack_wait_count 輪流攻擊
        if (charac_slots[attack_wait_count]->charac_ID == -1) {
            attack_wait_count ++;
            combo_count_and_drop(false); // 叫自己
            return;
        }
        charac_slots[attack_wait_count]->attack_text->hide();
        qDebug()<<"original attack"<<charac_slots[attack_wait_count]->attack;

        //攻擊敵人
        if ((charac_slots[attack_wait_count]->attack) > 0 && !(enemy[0]->dead && enemy[1]->dead && enemy[2]->dead)){
            if (level == 1 || level == 2){
                random_device rd;  // 使用真實隨機數據生成種子
                mt19937 gen(rd()); // 生成器
                uniform_int_distribution<> dis(0, survive.size() - 1); // 範圍 [0, vec.size() - 1]

                // 隨機挑選一個數
                int random_index = dis(gen);
                attack_enemy = survive[random_index];
            }
            if (level == 3){
                attack_enemy = 0;
            }
            damage = 0;
            qDebug()<<"before"<<enemy[attack_enemy]->hp;
            if (charac_slots[attack_wait_count]->attribute == "water" && enemy[attack_enemy]->enemy_clr == "fire"){
                    damage = 2*charac_slots[attack_wait_count]->attack;
                    qDebug()<<charac_slots[attack_wait_count]->attack<<damage;
            }
            else if (charac_slots[attack_wait_count]->attribute == "water" && enemy[attack_enemy]->enemy_clr == "earth"){
                    damage = charac_slots[attack_wait_count]->attack*0.5;
                    qDebug()<<charac_slots[attack_wait_count]->attack<<damage;

            }
            else if (charac_slots[attack_wait_count]->attribute == "fire" && enemy[attack_enemy]->enemy_clr == "earth"){
                    damage = 2*charac_slots[attack_wait_count]->attack;
                    qDebug()<<charac_slots[attack_wait_count]->attack<<damage;

            }
            else if (charac_slots[attack_wait_count]->attribute == "fire" && enemy[attack_enemy]->enemy_clr == "water"){
                    enemy[attack_enemy]->hp -= charac_slots[attack_wait_count]->attack/2;
                    damage = charac_slots[attack_wait_count]->attack*0.5;
                    qDebug()<<charac_slots[attack_wait_count]->attack<<damage;

            }
            else if (charac_slots[attack_wait_count]->attribute == "earth" && enemy[attack_enemy]->enemy_clr == "water"){
                    damage = 2*charac_slots[attack_wait_count]->attack;
                    qDebug()<<charac_slots[attack_wait_count]->attack<<damage;
            }
            else if (charac_slots[attack_wait_count]->attribute == "earth" && enemy[attack_enemy]->enemy_clr == "fire"){
                    damage = charac_slots[attack_wait_count]->attack*0.5;
                    qDebug()<<charac_slots[attack_wait_count]->attack<<damage;

            }
            else if (charac_slots[attack_wait_count]->attribute == "light" && enemy[attack_enemy]->enemy_clr == "dark"){
                    damage = 2*charac_slots[attack_wait_count]->attack;
                    qDebug()<<charac_slots[attack_wait_count]->attack<<damage;

            }
            else if (charac_slots[attack_wait_count]->attribute == "dark" && enemy[attack_enemy]->enemy_clr == "light"){
                    damage = 2*charac_slots[attack_wait_count]->attack;
                    qDebug()<<charac_slots[attack_wait_count]->attack<<damage;

            }
            else {
               damage = charac_slots[attack_wait_count]->attack;
            }
            qDebug()<<damage;
            enemy[attack_enemy]->hp = enemy[attack_enemy]->hp - damage;

            enemy_hp[attack_enemy]->hp_lost(level,attack_enemy,damage);
            if ((enemy[attack_enemy]->hp) <= 0){
                enemy[attack_enemy]->dead = true;
                enemy_hp[attack_enemy]->hp_bar->move(0,1000);
                if (survive.size() > 0)
                    survive.erase(remove(survive.begin(), survive.end(), attack_enemy), survive.end());
                else
                    survive.clear();
                qDebug()<<enemy[attack_enemy]->enemy_clr<<"is dead";
            }
            qDebug()<<charac_slots[attack_wait_count]->attribute<<"attacks"<<enemy[attack_enemy]->enemy_clr;
            qDebug()<<"Damage is :"<<damage;
            if (survive.size() == 0){
                for (int i = 0; i < 6; i++){
                    charac_slots[i]->attack = 0;
                }
            }
            qDebug()<<"after"<<enemy[attack_enemy]->hp;

        }

        charac_slots[attack_wait_count]->new_round();
        attack_wait_count ++;

        combo_cd->start(190);
    } else if (game_status == 3){ // 結算準備下一局
        attack_wait_count = -1;
        burn_road = false;

        if (level == 1 || level == 2){ //敵人全滅
            if (enemy[0]->dead && enemy[1]->dead && enemy[2]->dead){
                level++;
                ui_button->hide();
                ui_text->hide();
                if (level == 2){
                    for (int i = 0; i < 3; i++){
                        enemy[i]->enemy_item->show();
                        enemy[i]->show(2,i);
                        enemy[i]->enemy_item->move(85 +  130*i, 200);
                        enemy_hp[i]->reset(2,i);
                        enemy_hp[i]->hp_bar->move(100 +  133*i, 330);
                        survive.push_back(0);
                        survive.push_back(1);
                        survive.push_back(2);
                    }
                }
                if (level == 3){
                    enemy[0]->enemy_item->show();
                    enemy[0]->show(3,0);
                    enemy[0]->enemy_item->move(142, 50);
                    enemy[1]->enemy_item->move(0, 1000);
                    enemy[2]->enemy_item->move(0, 1000);
                    enemy_hp[0]->reset(3,0);
                    enemy_hp[0]->hp_bar->move(140, 330);
                    survive.push_back(0);
                }

            }
        }

        for (int i = 0; i < 3; i++){
            if (enemy[i]->cd > 0)
                enemy[i]->cd--;
            if ((enemy[i]->dead) == true)
                enemy[i]->enemy_item->move(0,1000); //將死亡敵人移除
        }

        // 敵人的攻擊
        for (int i = 0; i < 3; i++){
            if (enemy[i]->cd == 0){
                if (level == 1){
                    if (enemy[i]->dead == false){
                        hp = hp - enemy[i]->attack;
                        qDebug()<<"enemy attack!";
                        enemy[i]->cd_reset(level);
                    }
                }
                if (level == 2){
                    if (enemy[i]->dead == false){
                        hp = hp - enemy[i]->attack;
                        qDebug()<<"enemy attack!";
                        enemy[i]->cd_reset(level);
                    }
                }
            }

        }
        if (level == 3){
            if (enemy[0]->dead == false && enemy[0]->cd == 0){
                hp = hp - enemy[0]->attack;
                qDebug()<<"enemy attack!";
                enemy[0]->cd_reset(level);
            }
        }
        // 風化指定符石位置
        runestones[2][2]->change_color(runestones[2][2]->get_color(), 2);

        // 燃燒指定位置
        burning.push_back({0, 0});
        burning.push_back({1, 2});

        // 顯示燃燒位置
        runestones[0][0]->change_color(runestones[0][0]->get_color(), 1);
        runestones[1][2]->change_color(runestones[1][2]->get_color(), 1);

        // 啟動燃燒軌跡
        burn_road = true;


        // 畫面亮回，準備下一局
        hp += heal;
        if (hp > init_hp) hp = init_hp;
        darken->move(0, 1000);
        heal = 0;
        harm = 0;
        icon_bar->heal_text->hide();

        // 更新血條
        icon_bar->change_status("hp", 1.0 - hp/init_hp);
        icon_bar->hp_text->setText(QString::number(hp) + "/" + QString::number(init_hp));
        game_status = 1; // 開始下一局
        can_move_runestone = true;
        return;
    }
}

void MainWindow::combo_count() {
    combo_counter_result = combo_counter.count(runestones);
    cur_stage_combo = combo_counter_result.size();
    if (cur_stage_combo == 0) {
        qDebug() << "No Combo";
        game_status = 2;
        combo_cd->start(190); // start again

        // 暗下畫面
        darken->move(0, 510);
        darken->raise();

        // 關閉燃燒路徑的位置
        for (int i = 0; i < int(runestones.size()); i++){
            for (int j = 0; j < int(runestones[i].size()); j++){
                if (runestones[i][j]->status == 1) runestones[i][j]->change_color(runestones[i][j]->get_color(), 0);
            }
        }
        // 還原本來就有燃燒的位置
        for (int i = 0; i < int(burning.size()); i++){
            runestones[burning[i].first][burning[i].second]->change_color(runestones[burning[i].first][burning[i].second]->get_color(), 1);
        }

        return;
    }
    combo_text->show();
    cur_pair_num = 0;
    combo_eliminate();
    combo_cd->start(390);
}

void MainWindow::combo_eliminate() {
    // 用來算 攻擊時長 間距
    if (game_status == 2){
        combo_cd->stop();
        combo_count_and_drop(false); // 交給下個階段
        return;
    }

    // 用來算 combo時長 間距
    if (cur_pair_num == cur_stage_combo) {
        combo_cd->stop();
        drop_detect();
        return;
    }
    Runestone_pair cur_pair = combo_counter_result[cur_pair_num];
    for (pair<int,int> pii : cur_pair.pair)
        runestones[pii.first][pii.second]->change_color("empty", 0);
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

    // 讓角色知道是否增加攻擊力
    for (int i = 0; i < 6; i++) {
        if (charac_slots[i]->charac_ID != -1){ // 確定不是空欄位
            charac_slots[i]->add_attack(cur_pair);
            // 顯示得到攻擊力值
            if (charac_slots[i]->attack > 0 ){
                charac_slots[i]->attack_text->show();
                charac_slots[i]->attack_text->setText(QString::number(charac_slots[i]->attack));
            }
        }
    }

    // 若消除心珠則回血
    if (cur_pair.color == "heart"){
        heal += init_heal * cur_pair.pair.size();
        icon_bar->heal_text->show();
        qDebug()<<"heal = "<<heal;
    }
    icon_bar->heal_text->setStyleSheet("color: lime");
    icon_bar->heal_text->setText("+" + QString::number(heal));


    cur_pair_num++;
    combo++;
    combo_text->setText(QString::number(combo) + " Combo");
}

void MainWindow::drop_detect() {
    tmp_runestones = vector<vector<Runestone*>>(10, vector<Runestone*>(6, nullptr));
    result_colors = vector<vector<QString>>(5, vector<QString>(6, "empty"));
    col_bottom = vector<int>(6, 4);
    for (int col = 0; col < 6; col++) {
        // 找最下面的空白珠
        while(col_bottom[col] >= 0 && runestones[col_bottom[col]][col]->get_color() != "empty") {
            result_colors[col_bottom[col]][col] = runestones[col_bottom[col]][col]->get_color();
            col_bottom[col]--;
        }
        int count_empty = (col_bottom[col]>=0)? 1:0;
        for (int row = col_bottom[col] - 1; row >= 0; row--) {
            if (runestones[row][col]->get_color()!="empty") {
                tmp_runestones[row+5][col] = new Runestone(this, row, col, runestones[row][col]->get_color());
                runestones[row][col]->change_color("empty", 0);
            } else count_empty++;
        }
        while(count_empty > 0) {
            tmp_runestones[5-count_empty][col] = new Runestone(this, -1*count_empty, col, random_runestone_color());
            count_empty--;
        }
    }

    int max_wait_time = 0;

    for (int col = 0; col < 6; col++) {
        if (col_bottom[col]==-1) continue;
        for (int row = col_bottom[col]+5; row >= 0; row--) {
            if (tmp_runestones[row][col]==nullptr || tmp_runestones[row][col]->get_color()=="empty" || row == col_bottom[col]+5) {
                continue;
            }
            if (col_bottom[col]==-1) continue;
            tmp_runestones[row][col]->drop(row-5, col_bottom[col]);
            result_colors[col_bottom[col]][col] = tmp_runestones[row][col]->get_color();
            max_wait_time = std::max(max_wait_time, (col_bottom[col]+5-row));
            col_bottom[col]--;
        }
    }
    QTimer::singleShot((max_wait_time+1)*105, [&](){
        for (auto &row : tmp_runestones) {
            for (auto &i : row) {
                delete i;
            }
        }
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 6; j++) {
                runestones[i][j]->change_color(result_colors[i][j], 0);
            }
        }
        result_colors.clear();
        combo_count_and_drop(false); // count combo and drop again
        return;
    });
}
