#include "mainwindow.h"
#include <QDebug>
#include <QCursor>
#include <algorithm>
#include <QGraphicsOpacityEffect>
#include <cstdlib>
#include <ctime>
#include <random>
#include <QFontDatabase>
#include <QPointF>


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
    QFont ui_text_font("Consolas", 14, QFont::Bold);
    ui_text->setFont(ui_text_font);
    ui_text->setStyleSheet("color: white");
    ui_text->resize(200, 200);
    ui_text->move(32, 461);
    ui_text->show();
    ui_text->setText("Mission 1");

    skill_text = new QLabel(this);
    QFont skill_text_font("Consolas", 14, QFont::Bold);
    skill_text->setFont(skill_text_font);
    skill_text->setStyleSheet("color: white");
    skill_text->resize(2000, 300);
    skill_text->move(20, 600);
    skill_text->show();
    skill_text->setText("Mission 1:\n  Level 1: No Skill\n  Level 2: Weather 2 Stones After\n\n  Level 3: Burn While Move\n");

    gameover_text = new QLabel(this);
    QFontDatabase::addApplicationFont(":/dataset/Survival Instinx");
    QFont gameover_text_font("Survival Instinx", 50, QFont::Bold);
    gameover_text->setFont(gameover_text_font);
    gameover_text->setStyleSheet("color: red");
    gameover_text->resize(1000, 1000);
    gameover_text->move(50, -250);
    gameover_text->setText("Game Over");
    gameover_text->hide();

    cd_text1 = new QLabel(this);
    QFont cd1_text_font("Consolas", 10);
    cd_text1->setFont(cd1_text_font);
    cd_text1->setStyleSheet("color: white");
    cd_text1->resize(200, 200);
    cd_text2 = new QLabel(this);
    cd_text2->setFont(cd1_text_font);
    cd_text2->setStyleSheet("color: white");
    cd_text2->resize(200, 200);
    cd_text3 = new QLabel(this);
    cd_text3->setFont(cd1_text_font);
    cd_text3->setStyleSheet("color: white");
    cd_text3->resize(200, 200);

    //assign bullet to each character
    bullet.resize(6);
    for (int i = 0; i < 6; i++){
        bullet[i] = new Bullet(this);
    }



    // 頂部角色設置欄位
    charac_slots.resize(6);
    for (int i = 0; i < 6; i++) {
        delete charac_slots[i];
        charac_slots[i] = new Charac_slot(this);
        charac_slots[i]->charac_item->setFixedWidth(90);
        charac_slots[i]->charac_item->setFixedHeight(90);
        charac_slots[i]->charac_item->move(0 + i * 90, 360);
        charac_slots[i]->attack_text->move(25 + i * 90, 315);
        bullet[i]->bullet_item->move(45 + i * 90, 360);
        bullet[i]->bullet_item->hide();
    }

    //set enemy and hp
    enemy.resize(3);
    enemy_hp.resize(3);
    for (int i = 0; i < 3; i++){
        delete enemy[i];
        delete enemy_hp[i];
        enemy[i] = new Enemy(this);
        enemy[i]->enemy_item->setFixedWidth(120);
        enemy[i]->enemy_item->setFixedHeight(120);
        enemy[i]->enemy_item->move(0,1000);
        enemy[i]->show(1,i);
        enemy_hp[i] = new Enemy_hp(this);
        enemy_hp[i]->changeImageColor(1,i);
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

    // Combo text
    combo_text = new QLabel(this);
    QFont combo_text_font("Consolas", 35, QFont::Bold);
    combo_text->setFont(combo_text_font);
    combo_text->setStyleSheet("color: yellow");
    combo_text->resize(300, 200);
    combo_text->move(540-300, 960-200);
    combo_text->hide();

    combo_cd = new QTimer;
    connect(combo_cd, SIGNAL(timeout()), this, SLOT(combo_eliminate()));

    icon_bar->heal_text->show();
    icon_bar->heal_text->setText("+" + QString::number(init_heal));

    // 結算時的暗下畫面
    darken = new QLabel(this);
    darken_pic.load(":/dataset/ui/darken.png");
    darken_pic = darken_pic.scaled(1000, 510);

    darken->setPixmap(darken_pic);
    darken->hide();
    darken->move(0, 510);
    darken->setFixedSize(darken_pic.width(), darken_pic.height());

    // 設計成半透明
    darken_opacityEffect = new QGraphicsOpacityEffect;
    darken_opacityEffect->setOpacity(0.5); // 50% transparency
    darken->setGraphicsEffect(darken_opacityEffect);

    // 設定介面
    setting_pic.load(":/dataset/setting_pic.png");
    setting_pic = setting_pic.scaled(setting_pic.width()*2.5, setting_pic.height()*2.5);
    setting = new QLabel(this);
    setting->setPixmap(setting_pic);
    setting->resize(setting_pic.size());
    setting->move(540-setting_pic.width(), 0);
    setting->hide();

    surrender_button = new QPushButton(this);
    surrender_button_pic.load(":/dataset/ui/surrender button.jpg");
    surrender_button->setIcon(QIcon(surrender_button_pic));
    surrender_button->setIconSize(surrender_button_pic.size());
    surrender_button_item = new ButtonItem(surrender_button);
    surrender_button->setFixedWidth(180);
    surrender_button->setFixedHeight(83);
    surrender_button->move(540/2 - surrender_button->width()/2,960/4);
    surrender_button->hide();
    connect(surrender_button, SIGNAL(clicked()), this, SLOT(on_surrender_button_clicked()));

    full_darken = new QLabel(this);
    full_darken_pic.load(":/dataset/ui/full_darken.png");
    full_darken_pic = full_darken_pic.scaled(540, 960);
    full_darken->setPixmap(full_darken_pic);
    full_darken->setFixedSize(full_darken_pic.width(), full_darken_pic.height());
    full_darken->move(0,0);
    full_darken_opacityEffect = new QGraphicsOpacityEffect;
    full_darken_opacityEffect->setOpacity(0.5);
    full_darken->setGraphicsEffect(full_darken_opacityEffect);
    full_darken->hide();

    back_button = new QPushButton(this);
    back_button_pic.load(":/dataset/ui/back button.jpg");
    back_button->setIcon(QIcon(back_button_pic));
    back_button->setIconSize(back_button_pic.size());
    back_button_item = new ButtonItem(back_button);
    back_button->setFixedWidth(180);
    back_button->setFixedHeight(83);
    back_button->move(540/2 - back_button->width()/2, 960/4 + 150);
    back_button->hide();
    connect(back_button, SIGNAL(clicked()), this, SLOT(on_back_button_clicked()));

    // Restart Button
    restart_button = new QPushButton(this);
    restart_button_pic.load(":/dataset/ui/restart button.jpg");
    restart_button->setIcon(QIcon(restart_button_pic));
    restart_button->setIconSize(restart_button_pic.size());
    restart_button_item = new ButtonItem(restart_button);
    restart_button->setFixedWidth(180);
    restart_button->setFixedHeight(83);
    restart_button->move(540/2 - restart_button->width()/2, 960/4*3 - restart_button->height()/2);
    restart_button->hide();
    connect(restart_button, SIGNAL(clicked()), this, SLOT(on_restart_button_clicked()));

    start_button->raise();
    bg->menu_bgm->play();
}

void MainWindow::on_start_button_clicked() {
    qDebug() << "Start Button Clicked!";

    // 至少要有隊長和副隊長才能開始
    if(charac_slots[0]->charac_ID == -1 || charac_slots[5]->charac_ID == -1)
        return;

    if (charac_slots[0]->charac_ID > 4) bg->set_bgm(1);
    else bg->set_bgm(0);
    bg->menu_bgm->stop();
    bg->battle_bgm->play();

    start_button->hide();
    game_status = 1; // game start

    double_combo = false;
    burn_road = false; // 燃燒軌跡技能生效
    move_free = false; // 排珠
    skill = 0;

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

    if (!basic){
        enemy[0]->cd = 3;
        enemy[1]->cd = 1;
        enemy[2]->cd = 2;
    }

    cd_text1->move(70,150);
    cd_text1->show();
    cd_text1->setText("CD" +  QString::number(enemy[0]->cd));
    cd_text2->move(210,150);
    cd_text2->show();
    cd_text2->setText("CD" +  QString::number(enemy[1]->cd));
    cd_text3->move(345,150);
    cd_text3->show();
    cd_text3->setText("CD" +  QString::number(enemy[2]->cd));


    setting->show();

    ui_button->hide();
    ui_text->hide();
    ui_button->move(180, 250);
    ui_text->move(190, 200);

    skill_text->hide();

    // 清除空的隊伍欄位
    init_heal = 0;
    init_hp = 0;
    harm = 0;
    for (int i = 0; i < 6; i++) {
        if (!basic) charac_slots[i]->attack_text->move(10 + i * 90, 315);

        if(charac_slots[i]->charac_ID == -1)
            charac_slots[i]->charac_item->move(0, 1000); // 只是移出螢幕
        else {
            charac_slots[i]->attack_text->hide();
            // 結算選擇的角色的 init_heal、init_hp 總值
            init_heal += charac_slots[i]->charac_heal[charac_slots[i]->charac_ID];
            init_hp += charac_slots[i]->charac_hp[charac_slots[i]->charac_ID];
        }
    }

    // 若隊長、副隊長為GBC，則三圍提升
    if (charac_slots[0]->charac_ID > 4){
        init_hp *= 1.5;
    }

    // 觸發隊長技能
    if (!basic) {
        if (charac_slots[0]->charac_ID < 5){
            for (int i = 0; i < 6; i++)
                if(charac_slots[i]->charac_ID != -1){
                    charac_slots[i]->CD = 0;
                    charac_slots[i]->charac_item->move(0 + i * 90, 350);
                }
        }
        else if (charac_slots[0]->charac_ID == 8) {
            charac_slots[0]->CD = 0;
            charac_slots[0]->charac_item->move(0, 350);
        }
        if (charac_slots[0]->charac_ID == 7) time_limit = 20;
    }

    // 基礎模式則置成1
    if (basic){
        init_heal = 5;
        init_hp = 2000;
        time_limit = 10;
    }
    icon_bar->hp_text->setText(QString::number(init_hp) + "/" + QString::number(init_hp));
    icon_bar->heal_text->hide();
    hp = init_hp;
    burn_road = false;
}

void MainWindow::on_surrender_button_clicked() {
    skill_text->hide();
    game_status = 5;
    QSound::play(":/dataset/setting_select.wav");
    full_darken->hide();
    surrender_button->hide();
    back_button->hide();
    game_over();
}

void MainWindow::on_back_button_clicked() {
    skill_text->hide();
    game_status = 1;
    can_move_runestone = 1;
    QSound::play(":/dataset/setting_select.wav");
    full_darken->hide();
    surrender_button->hide();
    back_button->hide();
}

void MainWindow::on_restart_button_clicked() {
    game_status = 0;
    for (int i = 0; i < 3; i++){
        enemy[i]->enemy_item->hide();
        enemy_hp[i]->hp_bar->hide();
    }
    for (int i = 0; i < int(runestones.size()); i++){
        for (int j = 0; j < int(runestones[i].size()); j++){
            runestones[i][j]->change_color("empty", 0);
        }
    }
    skill_text->setText("Mission 1:\n  Level 1: No Skill\n  Level 2: Weather 2 Stones After\n\n  Level 3: Burn While Move\n");
    skill_text->show();
    harm = 0; // 所受傷害
    damage = 0;
    burning.clear();
    survive = {0,1,2};
    attack_wait_count = -1;
    level = 1;
    init_hp = 2000;
    icon_bar->hp_text->setText(QString::number(init_hp) + "/" + QString::number(init_hp));
    icon_bar->heal_text->setText("+0");
    icon_bar->heal_text->setStyleSheet("color: lime");
    basic = true;
    full_darken->hide();
    gameover_text->hide();
    restart_button->hide();
    darken->hide();
    setting->hide();
    start_button->show();
    ui_button->show();
    ui_button->move(10, 520);
    ui_text->show();
    ui_text->move(32, 461);
    ui_text->setText("Mission 1");
    for (int i = 0; i < 6; i++) {
        charac_slots[i]->reset();
        charac_slots[i]->charac_item->move(0 + i * 90, 360);
        charac_slots[i]->attack_text->move(25 + i * 90, 315);
    }
    //set enemy and hp
    enemy.resize(3);
    enemy_hp.resize(3);
    for (int i = 0; i < 3; i++){
        enemy[i]->enemy_item->setFixedWidth(120);
        enemy[i]->enemy_item->setFixedHeight(120);
        enemy[i]->enemy_item->move(0,1000);
        enemy[i]->show(1,i);
        enemy_hp[i]->changeImageColor(1,i);
        enemy_hp[i]->hp_bar->move(0,1000);
        enemy[i]->enemy_item->show();
        enemy_hp[i]->reset(level,i);
        enemy_hp[i]->hp_bar->show();
    }
    can_move_runestone = true;
    runestone_selected = false; // is cursor select a runestone ?
    runestone_swap = false;
    runestone_drift = false;
    ms_elapsed = 0;
    drift_timer_started = false;
    icon_bar->change_status("hp", 0.0);
    icon_bar->initial_hp = init_hp;
    bg->menu_bgm->play();
    start_button->raise();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    if (runestone_drift) {
        //qDebug() << ms_elapsed;
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
            int leader = charac_slots[0]->charac_ID;
            if((event->x() / 90) == 0) leader = -2;
            charac_slots[event->x() / 90] -> change_charac(leader, basic);
            // 技能敘述
            if (!basic) skill_text->setText(skill_descript[charac_slots[event->x() / 90]->charac_ID]);
        }
        // 更換任務
        if (event->x() >= 10 && event->x() <= 190 && event->y() >= 520 && event->y() <= 603) {
            if (basic){
                skill_text->setText("Mission 2:\n  Level 1: No Skill\n  Level 2: Attack first\n  Weather 2 Stones After;\n  Burn 2 Places x2\n  Level 3: Burn While Move,\n  10+combo or Hits");
                basic = false;
                ui_text->setText("Mission 2");
            }
            else{
                skill_text->setText("Mission 1:\n  Level 1: No Skill\n  Level 2: Weather 2 Stones After\n\n  Level 3: Burn While Move\n");
                basic = true;
                ui_text->setText("Mission 1");
            }
            for (int i = 0; i < int(charac_slots.size()); i++){
                int charac_ID = charac_slots[i]->charac_ID;
                int leader = charac_slots[0]->charac_ID;
                if(i == 0) leader = -2;
                if(charac_slots[i]->charac_ID != -1){
                    charac_slots[i] -> change_charac(leader, basic);
                    while (charac_slots[i]->charac_ID != charac_ID)
                        charac_slots[i] -> change_charac(leader, basic);
                }
            }
        }

        // 結算選擇的角色的 init_heal、hp 總值
        init_heal = 0;
        init_hp = 0;
        for (int i = 0; i < 6; i++) {
            if(charac_slots[i]->charac_ID != -1){
                init_heal += charac_slots[i]->charac_heal[charac_slots[i]->charac_ID];
                init_hp += charac_slots[i]->charac_hp[charac_slots[i]->charac_ID];
            }
        }

        // 基礎模式則置成1、2000
        if (basic){
            init_heal = 5;
            init_hp = 2000;
        }
        icon_bar->heal_text->setText("+" + QString::number(init_heal));
        icon_bar->hp_text->setText(QString::number(init_hp) + "/" + QString::number(init_hp));

        return;
    }
    if (game_status == 1 && event->button() == Qt::LeftButton) {
        // 開啟設定介面
        if (can_move_runestone && event->y() <= setting->height() && event->x() >= 540 - setting->width()) {
            game_status = -1;
            can_move_runestone = 0;
            qDebug() << "setting";
            QSound::play(":/dataset/setting_select.wav");
            full_darken->show();
            full_darken->raise();
            surrender_button->show();
            surrender_button->raise();
            back_button->show();
            back_button->raise();
            skill_text->raise();
            if (basic){
                skill_text->setText("Mission 1:\n  Level 1: No Skill\n  Level 2: Weather 2 Stones After\n\n  Level 3: Burn While Move\n");
                skill_text->show();
            }
            else{
                skill_text->setText("Mission 2:\n  Level 1: No Skill\n  Level 2: Attack first\n  Weather 2 Stones After;\n  Burn 2 Places x2\n  Level 3: Burn While Move,\n  10+combo or Hits");
                skill_text->show();
            }
        }
        // 開啟技能
        else if (event->y() >= 350 && event->y() <= 440) {
            if (charac_slots[event->x() / 90]->CD == 0){
                if (charac_slots[event->x() / 90]->charac_ID< 5){
                    for (int i=0; i<3; i++){
                        runestones[0][i]->change_color("water", 0);
                        runestones[1][i]->change_color("fire", 0);
                        runestones[2][i]->change_color("earth", 0);
                        runestones[3][i]->change_color("light", 0);
                        runestones[4][i]->change_color("dark", 0);
                        runestones[0][i+3]->change_color("dark", 0);
                        runestones[1][i+3]->change_color("water", 0);
                        runestones[2][i+3]->change_color("fire", 0);
                        runestones[3][i+3]->change_color("earth", 0);
                        runestones[4][i+3]->change_color("light", 0);
                    }
                }
                if (charac_slots[event->x() / 90]->charac_ID == 5){
                    for (int i=0; i<5; i++){
                        runestones[i][0]->change_color("dark", runestones[i][0]->status);
                        runestones[i][1]->change_color("water", runestones[i][1]->status);
                        runestones[i][2]->change_color("earth", runestones[i][2]->status);
                        runestones[i][3]->change_color("dark", runestones[i][3]->status);
                        runestones[i][4]->change_color("water", runestones[i][4]->status);
                        runestones[i][5]->change_color("earth", runestones[i][5]->status);
                    }
                    time_limit = 20;
                    burn_road = false;
                }
                if (charac_slots[event->x() / 90]->charac_ID == 6)
                    for (int i=3; i<6; i++)
                        runestones[2][i]->change_color("heart", runestones[2][i]->status);
                if (charac_slots[event->x() / 90]->charac_ID == 7) {
                    move_free = true;
                    if ((event->x() / 90) == 0){
                        if (charac_slots[5]->CD>0)charac_slots[5]->CD--;
                        if (charac_slots[1]->CD>0)charac_slots[1]->CD--;
                    }
                    else if ((event->x() / 90) == 5){
                        if (charac_slots[4]->CD>0)charac_slots[4]->CD--;
                        if (charac_slots[0]->CD>0)charac_slots[0]->CD--;
                    }
                    else{
                        if (charac_slots[(event->x() / 90) + 1]->CD>0)charac_slots[(event->x() / 90) + 1]->CD--;
                        if (charac_slots[(event->x() / 90) - 1]->CD>0)charac_slots[(event->x() / 90) - 1]->CD--;
                    }
                    // 檢查技能好了嗎
                    for (int i=0; i < int(charac_slots.size()); i++){
                        if (charac_slots[i]->CD == 0){
                            charac_slots[i]->charac_item->move(0 + i * 90, 350);
                        }
                    }
                }
                if (charac_slots[event->x() / 90]->charac_ID == 8) {
                    if (double_combo) return;
                    double_combo = true;
                    charac_slots[event->x() / 90]->hit_more = 2;
                }
                if (charac_slots[event->x() / 90]->charac_ID == 9) skill = 9;
                charac_slots[event->x() / 90]->CD_reset();

                charac_slots[event->x() / 90]->charac_item->move(0 + (event->x() / 90) * 90, 360);
            }
            else if (!basic){
                charac_slots[event->x() / 90]->attack_text->show();
                charac_slots[event->x() / 90]->attack_text->setText(QString::number(charac_slots[event->x() / 90]->CD));
            }
        }
        // 正常轉珠
        else if (event->y() >= 510) {
            if (can_move_runestone) {
                for (int i=0;i<int(charac_slots.size());i++) charac_slots[i]->attack_text->hide();
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
        // 關閉燃燒路徑的位置
        for (int i = 0; i < int(runestones.size()); i++){
            for (int j = 0; j < int(runestones[i].size()); j++){
                if (runestones[i][j]->status == 1) runestones[i][j]->change_color(runestones[i][j]->get_color(), 0);
            }
        }
        // 還原本來就有燃燒的位置
        if (!basic && level == 2){
            if (enemy[0]->dead == false){
                runestones[0][0]->change_color(runestones[0][0]->get_color(), 1);
                runestones[0][5]->change_color(runestones[0][5]->get_color(), 1);
            }
            if (enemy[2]->dead == false){
                runestones[4][0]->change_color(runestones[4][0]->get_color(), 1);
                runestones[4][5]->change_color(runestones[4][5]->get_color(), 1);
            }
        }
        runestone_selected = false;
        runestones[selected_runestone.first][selected_runestone.second]->move(selected_runestone.first, selected_runestone.second);
        runestones[selected_runestone.first][selected_runestone.second]->set_opacity(1);
        if (runestone_drift && !move_free) {
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
            int heat;
            if (basic) heat = 30;
            else if (skill == 9) heat = -100;
            else heat = 100;

            if (runestones[(event->y()-510)/90][event->x()/90]->status == 1){
                harm += heat;
                hp = hp - heat;
                icon_bar->heal_text->setStyleSheet("color: red");
                icon_bar->heal_text->show();
                icon_bar->heal_text->setText("-" + QString::number(harm));
                if (skill == 9){
                    icon_bar->heal_text->setStyleSheet("color: lime");
                    icon_bar->heal_text->setText("+" + QString::number(-harm));
                }
                burned = true;
                if (hp <= 0) game_over();
                if (hp >= init_hp) hp = init_hp;
            }
            if (runestones[selected_runestone.first][selected_runestone.second]->status == 1){
                burned_before = true;
            }

            if (runestones[(event->y()-510)/90][event->x()/90]->status == 2 || runestones[selected_runestone.first][selected_runestone.second]->status == 2){
                harm += heat;
                hp = hp - heat;
                icon_bar->heal_text->setStyleSheet("color: red");
                icon_bar->heal_text->show();
                icon_bar->heal_text->setText("-" + QString::number(harm));
                if (skill == 9){
                    icon_bar->heal_text->setStyleSheet("color: lime");
                    icon_bar->heal_text->setText("+" + QString::number(-harm));
                }

                if (hp <= 0) game_over();
                if (hp >= init_hp) hp = init_hp;

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
            runestones[selected_runestone.first][selected_runestone.second]->set_opacity(1);
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
        runestones[selected_runestone.first][selected_runestone.second]->set_opacity(0.5);
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
        for (auto row : runestones)
            for (auto i : row)
                i->set_opacity(1);
        QTimer::singleShot(105, [&](){
            if (combo_counter.count(runestones).empty()) {
                qDebug() << "final combo : " << combo;
                combo_text->hide();
                can_move_runestone = true;

                // 暗下畫面
                darken->show();
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
                    enemy[2]->enemy_item->hide();
                    ui_text->setText("  VICTORY!");
                    bg->battle_bgm->stop();
                    bg->win_bgm->play();
                    full_darken->show();
                    full_darken->raise();
                    ui_button->raise();
                    ui_text->raise();
                    restart_button->show();
                    restart_button->raise();
                    return;
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
        if (attack_wait_count == -1){ // 還沒要攻擊，乘上combo數
            double combo_power = combo;
            if (!basic && combo > 1) combo_power = combo * 0.5;
            int more_multi = 1; // 全隊倍率

            if (!basic){
                if (charac_slots[0]->charac_ID < 5) more_multi *= 2;
                if (skill == 9) more_multi *= 2;
            }
            attack_wait_count ++;
            for(int i = 0; i < int(charac_slots.size()); i++){
                if (charac_slots[i]->attack == 0) continue;
                charac_slots[i]->attack *= combo_power;
                if (!basic){ // 隊長、技能額外倍率
                    charac_slots[i]->attack *= more_multi;
                    charac_slots[i]->attack *= charac_slots[i]->extra_atk;
                }
                charac_slots[i]->attack_text->setText(QString::number(charac_slots[i]->attack));
            }

            // 角色10隊長技能
            if (!basic){
                if (charac_slots[0]->charac_ID == 9) {
                    int highest_power = 0;
                    for(int i = 0; i < int(charac_slots.size()); i++)
                        if (charac_slots[i]->attack > highest_power)
                            highest_power = charac_slots[i]->attack;
                    for(int i = 0; i < int(charac_slots.size()); i++){
                        if (charac_slots[i]->charac_ID != -1){
                            charac_slots[i]->attack = highest_power;
                            charac_slots[i]->attack_text->setText(QString::number(charac_slots[i]->attack));
                        }
                    }
                } else if (charac_slots[0]->charac_ID == 8) { // 角色9隊長技能
                    if (charac_slots[1]->attack != 0){
                        charac_slots[0]->attack += charac_slots[1]->attack;
                        charac_slots[1]->attack = 0;
                        charac_slots[0]->attack_text->setText(QString::number(charac_slots[0]->attack));
                        charac_slots[1]->attack_text->setText(QString::number(charac_slots[1]->attack));
                    }
                }
                // 關掉角色9的追擊技能
                for(int i = 0; i < int(charac_slots.size()); i++){
                    if (charac_slots[i]->attack == 0) {
                        if (charac_slots[i]->hit_more == 2){
                            charac_slots[i]->hit_more = -1;
                            double_combo = false;
                        }
                    }
                }
            }

            heal *= combo_power;
            icon_bar->heal_text->setText("+" + QString::number(heal));

            // 回復血量
            hp += heal;
            if (hp > init_hp) hp = init_hp;
            icon_bar->change_status("hp", 1.0 - hp/init_hp);
            icon_bar->hp_text->setText(QString::number(hp) + "/" + QString::number(init_hp));

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

        //子彈放回
        for (int i = 0; i < 6; i++) {
            bullet[i]->bullet_item->move(45 + i * 90, 360);
            bullet[i]->bullet_item->hide();
        }

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

            float advantage_attribute = 1; // 直接代表克制屬性倍率

            if (charac_slots[attack_wait_count]->attribute == "water" && enemy[attack_enemy]->enemy_clr == "fire")
                advantage_attribute = 2;
            else if (charac_slots[attack_wait_count]->attribute == "water" && enemy[attack_enemy]->enemy_clr == "earth")
                advantage_attribute = 0.5;
            else if (charac_slots[attack_wait_count]->attribute == "fire" && enemy[attack_enemy]->enemy_clr == "earth")
                advantage_attribute = 2;
            else if (charac_slots[attack_wait_count]->attribute == "fire" && enemy[attack_enemy]->enemy_clr == "water")
                advantage_attribute = 0.5;
            else if (charac_slots[attack_wait_count]->attribute == "earth" && enemy[attack_enemy]->enemy_clr == "water")
                advantage_attribute = 2;
            else if (charac_slots[attack_wait_count]->attribute == "earth" && enemy[attack_enemy]->enemy_clr == "fire")
                advantage_attribute = 0.5;
            else if (charac_slots[attack_wait_count]->attribute == "light" && enemy[attack_enemy]->enemy_clr == "dark")
                advantage_attribute = 2;
            else if (charac_slots[attack_wait_count]->attribute == "dark" && enemy[attack_enemy]->enemy_clr == "light")
                advantage_attribute = 2;

            // 角色8的技能
            if (charac_slots[attack_wait_count]->skill_power == 7) advantage_attribute = 2;
            if (charac_slots[attack_wait_count]->skill_power >= 0 &&
                    charac_slots[attack_wait_count]->skill_power <= 4) advantage_attribute = 2;

            damage = advantage_attribute * charac_slots[attack_wait_count]->attack;
            qDebug()<<charac_slots[attack_wait_count]->attack<<damage;

            //attack animation

            bullet[attack_wait_count]->bullet_item->show();
            bullet[attack_wait_count]->shoot(enemy[attack_enemy]->enemy_item->x()+60,enemy[attack_enemy]->enemy_item->y()+60);


            charac_slots[attack_wait_count]->damage_text->show();
            charac_slots[attack_wait_count]->damage_text->setText(QString::number(damage));
            charac_slots[attack_wait_count]->damage_text->move(enemy[attack_enemy]->enemy_item->x()+damage_text_pos[attack_wait_count][0],enemy[attack_enemy]->enemy_item->y()+damage_text_pos[attack_wait_count][1]);
            show_damage(charac_slots[attack_wait_count]->damage_text, 500);

            if (!basic) damage *= 0.001;
            if (!basic && (level == 3) && (combo < 10)) damage = 0; // 10+ combo盾

            qDebug()<<damage;
            enemy[attack_enemy]->hp = enemy[attack_enemy]->hp - damage;
            enemy_hp[attack_enemy]->hp_lost(level,attack_enemy,damage);


            if ((enemy[attack_enemy]->hp) <= 0){
                enemy[attack_enemy]->dead = true;
                enemy_hp[attack_enemy]->hp_bar->move(0,1000);
                if (attack_enemy == 0)
                    cd_text1->hide();
                else if (attack_enemy == 1)
                    cd_text2->hide();
                else if (attack_enemy == 2)
                    cd_text3->hide();
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

        if (charac_slots[attack_wait_count]->hit_more < 1){
            charac_slots[attack_wait_count]->new_round();
            attack_wait_count ++;
        }
        else charac_slots[attack_wait_count]->hit_more--;


        combo_cd->start(190);
    } else if (game_status == 3){ // 結算準備下一局

        for (int i = 0; i < 6; i++)
            bullet[i]->bullet_item->hide();

        attack_wait_count = -1;
        burn_road = false;
        harm = 0;
        icon_bar->heal_text->hide();

        // 風化指定符石位置
        int weather_amount = 0;
        if (level == 2 && enemy[1]->dead == false){
            //QSound::play(":/dataset/setting_select.wav");
            vector<int> sel;
            for (int row = 0; row < 5; row++)
                for (int col = 0; col < 6; col++)
                    if (runestones[row][col]->status == 0)
                        sel.push_back(row*6+col);
            uniform_int_distribution<> _dist(0, int(sel.size())-1);
            vector<bool> seled(int(sel.size()), false);
            int rd_num;
            for (int i = 0; i < 2; i++) {
                weather_amount++;
                do {
                    rd_num = _dist(gen);
                } while(seled[rd_num]);
                seled[rd_num] = true;
                int row = sel[rd_num] / 6;
                int col = sel[rd_num] % 6;
                runestones[row][col]->change_color(runestones[row][col]->get_color(), 2);
            }
        }

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
                        enemy_hp[i]->changeImageColor(2,i);
                        enemy_hp[i]->hp_bar->move(100 +  133*i, 330);
                        survive.push_back(0);
                        survive.push_back(1);
                        survive.push_back(2);
                        enemy[i]->cd = 3;
                        cd_text1->show();
                        cd_text2->show();
                        cd_text3->show();
                        cd_text1->move(70,120);
                        cd_text2->move(210,120);
                        cd_text3->move(345,120);

                    }
                    if (!basic) {
                        enemy[0]->cd = 1;
                        enemy[1]->cd = 0;
                        enemy[2]->cd = 1;
                    }
                }
                if (level == 3){
                    enemy[0]->enemy_item->show();
                    enemy[0]->show(3,0);
                    enemy[0]->enemy_item->move(142, 50);
                    enemy[1]->enemy_item->move(0, 1000);
                    enemy[2]->enemy_item->move(0, 1000);
                    enemy_hp[0]->reset(3,0);
                    enemy_hp[0]->changeImageColor(3,0);
                    enemy_hp[0]->hp_bar->move(140, 330);
                    survive.push_back(0);
                    enemy[0]->cd = 5;
                    cd_text1->show();
                    if (!basic) enemy[0]->cd = 2;
                }

            }
            else{
                for (int i = 0; i < 3; i++){
                    if (enemy[i]->cd > 0)
                        enemy[i]->cd--;
                    if ((enemy[i]->dead) == true){
                        enemy[i]->enemy_item->move(0,1000); //將死亡敵人移除
                        enemy[i]->cd = 1000;
                    }
                }
            }
        }else {
            for (int i = 0; i < 3; i++){
                if (enemy[i]->cd > 0)
                    enemy[i]->cd--;
                if ((enemy[i]->dead) == true){
                    enemy[i]->enemy_item->move(0,1000); //將死亡敵人移除
                    enemy[i]->cd = 1000;
                }
            }
        }

        // 啟動燃燒軌跡
        if (level == 3){
            burn_road = true;
        }

        // 燃燒指定位置
        if (!basic && level == 2){
            if (enemy[0]->dead == false){
                burning.push_back({0, 0});
                burning.push_back({0, 5});
                runestones[0][0]->change_color(runestones[0][0]->get_color(), 1);
                runestones[0][5]->change_color(runestones[0][5]->get_color(), 1);
            }
            if (enemy[2]->dead == false){
                burning.push_back({4, 0});
                burning.push_back({4, 5});
                runestones[4][0]->change_color(runestones[4][0]->get_color(), 1);
                runestones[4][5]->change_color(runestones[4][5]->get_color(), 1);
            }
        }

        // 受傷倍率
        double magnify = 1;
        if (!basic) {
            magnify = 7.2;
            if (skill == 9) magnify = 0;
        }

        // 敵人的攻擊
        for (int i = 0; i < 3; i++){
            if (enemy[i]->cd == 0){
                if (level == 1){
                    if (enemy[i]->dead == false){
                        harm += enemy[i]->attack * magnify;
                        qDebug()<<"enemy attack!";
                        enemy[i]->cd = 3;
                        if (!basic) enemy[i]->cd = 1;
                    }
                }
                if (level == 2){
                    if (enemy[i]->dead == false){
                        harm += enemy[i]->attack * magnify;
                        qDebug()<<"enemy attack!";
                        enemy[i]->cd = 3;
                        if (!basic) enemy[i]->cd = 2;
                    }
                }
            }

        }
        if (level == 3){
            if (enemy[0]->dead == false && enemy[0]->cd == 0){
                harm += enemy[0]->attack * magnify;
                if (!basic && (combo < 10)) harm *= (10 - combo); // 追擊
                qDebug()<<"enemy attack!";
                enemy[0]->cd = 5;
                if (!basic) enemy[0]->cd = 2;
            }
        }
        if (level == 1 || level == 2){
            cd_text1->setText("CD" +  QString::number(enemy[0]->cd));
            cd_text2->setText("CD" +  QString::number(enemy[1]->cd));
            cd_text3->setText("CD" +  QString::number(enemy[2]->cd));
        }
        else if (level == 3){
            cd_text1->move(130,40);
            cd_text2->hide();
            cd_text3->hide();
            cd_text1->setText("CD" +  QString::number(enemy[0]->cd));
        }

        hp -= harm;
        move_free = false;
        skill = 0;
        time_limit = 10;

        // 扣血傷害
        if (harm){
            icon_bar->heal_text->setStyleSheet("color: red");
            icon_bar->heal_text->show();
            icon_bar->heal_text->setText("-" + QString::number(harm));
        }
        if (hp <= 0){
            game_over();
        }

        // 隊伍技能
        if (!basic && charac_slots[0]->charac_ID == 7) time_limit = 20;
        if (!basic && charac_slots[0]->charac_ID == 6)
            for (int i=0;i<5;i++) runestones[i][0]->change_color("heart", runestones[i][0]->status);

        if (game_status != 5){ // gameover = 5
            if (weather_amount != 0 || harm != 0) { // 風化符石或受傷延遲動畫
                QTimer::singleShot(1000, [&](){
                    // 畫面亮回，準備下一局
                    darken->hide();
                    heal = 0;
                    harm = 0;
                    icon_bar->heal_text->setStyleSheet("color: lime");
                    icon_bar->heal_text->hide();

                    // 檢查技能好了嗎
                    for (int i=0; i < int(charac_slots.size()); i++){
                        if (charac_slots[i]->CD == 0)
                            charac_slots[i]->charac_item->move(0 + i * 90, 350);
                        if (charac_slots[i]->hit_more == 0)
                            charac_slots[i]->hit_more = 2;
                    }

                    // 更新血條
                    icon_bar->change_status("hp", 1.0 - hp/init_hp);
                    icon_bar->hp_text->setText(QString::number(hp) + "/" + QString::number(init_hp));
                    game_status = 1; // 開始下一局
                    can_move_runestone = true;


                });
            } else {
                // 畫面亮回，準備下一局
                darken->hide();
                heal = 0;
                harm = 0;

                // 檢查技能好了嗎
                for (int i=0; i < int(charac_slots.size()); i++){
                    if (charac_slots[i]->CD == 0)
                        charac_slots[i]->charac_item->move(0 + i * 90, 350);
                    if (charac_slots[i]->hit_more == 0)
                        charac_slots[i]->hit_more = 2;
                }

                // 更新血條
                icon_bar->change_status("hp", 1.0 - hp/init_hp);
                icon_bar->hp_text->setText(QString::number(hp) + "/" + QString::number(init_hp));
                game_status = 1; // 開始下一局
                can_move_runestone = true;
            }
        }
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
        darken->show();
        darken->raise();

        // 關閉燃燒路徑的位置
        for (int i = 0; i < int(runestones.size()); i++){
            for (int j = 0; j < int(runestones[i].size()); j++){
                if (runestones[i][j]->status == 1) runestones[i][j]->change_color(runestones[i][j]->get_color(), 0);
            }
        }
        // 還原本來就有燃燒的位置
        if (!basic && level == 2){
            if (enemy[0]->dead == false){
                runestones[0][0]->change_color(runestones[0][0]->get_color(), 1);
                runestones[0][5]->change_color(runestones[0][5]->get_color(), 1);
            }
            if (enemy[2]->dead == false){
                runestones[4][0]->change_color(runestones[4][0]->get_color(), 1);
                runestones[4][5]->change_color(runestones[4][5]->get_color(), 1);
            }
        }
        return;
    }
    combo_text->show();
    combo_text->raise();
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

    // 消除一組8顆延遲
    if (!basic && charac_slots[0]->charac_ID == 7)
        if (cur_pair.pair.size() > 7){
            int j = 1;
            while (true){
                bool found = false;
                for (int i = 0; i < 3; i++){
                    if (enemy[i]->cd == j){
                        found = true;
                        enemy[i]->cd++;
                        break;
                    }
                }
                if (j>100 || found) break;
                else j++;
            }
            if (level == 1 || level == 2){
                cd_text1->setText("CD" +  QString::number(enemy[0]->cd));
                cd_text2->setText("CD" +  QString::number(enemy[1]->cd));
                cd_text3->setText("CD" +  QString::number(enemy[2]->cd));
            }
            else if (level == 3)
                cd_text1->setText("CD" +  QString::number(enemy[0]->cd));
        }



    if (double_combo) combo++;
    cur_pair_num++;
    combo++;
    combo_text->setText(QString::number(combo) + " Combo");
}

void MainWindow::drop_detect() {
    tmp_runestones = vector<vector<Runestone*>>(10, vector<Runestone*>(6, nullptr));
    result_colors = vector<vector<pair<QString,int>>>(5, vector<pair<QString,int>>(6, make_pair("empty", 0)));
    col_bottom = vector<int>(6, 4);
    for (int col = 0; col < 6; col++) {
        // 找最下面的空白珠
        while(col_bottom[col] >= 0 && runestones[col_bottom[col]][col]->get_color() != "empty") {
            result_colors[col_bottom[col]][col] = make_pair(runestones[col_bottom[col]][col]->get_color(), ((runestones[col_bottom[col]][col]->get_status()==2)? 2 : 0));
            col_bottom[col]--;
        }
        int count_empty = (col_bottom[col]>=0)? 1:0;
        for (int row = col_bottom[col] - 1; row >= 0; row--) {
            if (runestones[row][col]->get_color()!="empty") {
                tmp_runestones[row+5][col] = new Runestone(this, row, col, runestones[row][col]->get_color(), ((runestones[row][col]->get_status()==2)? 2 : 0));
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
            result_colors[col_bottom[col]][col] = make_pair(tmp_runestones[row][col]->get_color(), tmp_runestones[row][col]->get_status());
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
                runestones[i][j]->change_color(result_colors[i][j].first, result_colors[i][j].second);
            }
        }
        result_colors.clear();
        combo_count_and_drop(false); // count combo and drop again
        return;
    });
}

void MainWindow::game_over(){
    bg->battle_bgm->stop();
    QSound::play(":/dataset/game_lose.wav");
    full_darken->show();
    full_darken->raise();
    game_status = 5;
    cd_text1->move(0,1000);
    cd_text2->move(0,1000);
    cd_text3->move(0,1000);
    for (int i = 0; i < int(runestones.size()); i++){
        for (int j = 0; j < int(runestones[i].size()); j++){
            runestones[i][j]->game_over_drop();
        }
    }

    for (int i = 0; i < 3; i++){
        enemy[i]->enemy_item->hide();
        enemy_hp[i]->hp_bar->hide();
    }

    icon_bar->change_status("hp", 1.0);
    icon_bar->hp_text->setText(QString::number(0) + "/" + QString::number(init_hp));
    gameover_text->show();
    gameover_text->raise();

    restart_button->show();
    restart_button->raise();

    for (int i = 0; i < 6; i++){
        bullet[i]->bullet_item->hide();
    }
}

void MainWindow::show_damage(QLabel *text, int seconds){
    QTimer *timer = new QTimer(text); // 设置parent以便自动清理

   // 连接QTimer的timeout信号到lambda函数，用于隐藏label
   QObject::connect(timer, &QTimer::timeout, [text, timer]() {
       text->hide();  // 隐藏文本
       timer->deleteLater();  // 删除定时器
   });

   // 启动QTimer，设置延迟时间为milliseconds毫秒
   timer->start(seconds);
}


