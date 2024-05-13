#include "runestone.h"
#include <QDebug>

Runestone::Runestone(QWidget *parent, int _row, int _col, QString clr) {
    dark_stone_pic.load(":/dataset/runestone/dark_stone.png");
    earth_stone_pic.load(":/dataset/runestone/earth_stone.png");
    fire_stone_pic.load(":/dataset/runestone/fire_stone.png");
    heart_stone_pic.load(":/dataset/runestone/heart_stone.png");
    light_stone_pic.load(":/dataset/runestone/light_stone.png");
    water_stone_pic.load(":/dataset/runestone/water_stone.png");

    runestone = new QLabel(parent);
    runestone->setFixedSize(dark_stone_pic.width(), dark_stone_pic.height());
    move(_row, _col);
    change_color(clr);
    runestone->show();

    animation = new QPropertyAnimation(runestone, "pos");
}

Runestone::~Runestone() {
    runestone->hide();
}

void Runestone::move(int _row, int _col) {
    row = _row;
    col = _col;
    runestone->move(col*90, 510+row*90);
}

void Runestone::change_color(QString clr) {
    color = clr;
    if (clr == "dark") runestone->setPixmap(dark_stone_pic);
    else if (clr == "earth") runestone->setPixmap(earth_stone_pic);
    else if (clr == "fire") runestone->setPixmap(fire_stone_pic);
    else if (clr == "heart") runestone->setPixmap(heart_stone_pic);
    else if (clr == "light") runestone->setPixmap(light_stone_pic);
    else if (clr == "water") runestone->setPixmap(water_stone_pic);
    else if (clr == "empty") runestone->setPixmap(QPixmap());
}

void Runestone::stick_cursor(int _x, int _y) {
    runestone->move(_x-dark_stone_pic.width()/2-10, _y-dark_stone_pic.height()/2-10);
}

void Runestone::drop(QString after_drop_change_clr, int drop_speed) {
    change_color(after_drop_change_clr);
    animation->setDuration(drop_speed-30);
    animation->setStartValue(QPoint(runestone->x(), runestone->y()-90));
    animation->setEndValue(QPoint(runestone->x(), runestone->y()));
    animation->start();
}




