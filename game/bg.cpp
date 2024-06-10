#include "bg.h"
#include <QDebug>

Bg::Bg(QWidget *parent) {
    battle_bg = new QLabel(parent);
    battle_bg_pic.load(":/dataset/ui/battle_bg.png");
    //qDebug() << battle_bg_pic.width() << " " << battle_bg_pic.height();
    battle_bg_pic = battle_bg_pic.scaled(1000, 510);

    battle_bg->setPixmap(battle_bg_pic);
    battle_bg->move(-230, 0);
    battle_bg->setFixedSize(battle_bg_pic.width(), battle_bg_pic.height());

    runestone_bg = new QLabel(parent);
    runestone_bg_pic.load(":/dataset/ui/runestone_bg.png");
    //qDebug() << runestone_bg_pic.width() << " " << runestone_bg_pic.height();
    runestone_bg_pic = runestone_bg_pic.scaled(540, 450);
    runestone_bg->setPixmap(runestone_bg_pic);
    runestone_bg->move(0, 510);
    runestone_bg->setFixedSize(runestone_bg_pic.width(), runestone_bg_pic.height());

    menu_bgm = new QSound(":/dataset/menu_bgm.wav");
    menu_bgm->setLoops(-1);

    win_bgm = new QSound(":/dataset/game_win.wav");
    //win_bgm->setLoops(-1);
}

void Bg::set_bgm(int what_song){
    if (what_song == 0) battle_bgm = new QSound(":/dataset/battle_bgm.wav");
    else if (what_song == 1) battle_bgm = new QSound(":/dataset/GBC bgm.WAV");
    else if (what_song == 2) battle_bgm = new QSound(":/dataset/Bocchi BGM.WAV");

    battle_bgm->setLoops(-1);
}
