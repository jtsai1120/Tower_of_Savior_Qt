#include "runestone.h"
#include <QDebug>

Runestone::Runestone(QWidget *parent, int _row, int _col, int clr) {
    dark_stone_pic.load(":/dataset/runestone/dark_stone.png");
    earth_stone_pic.load(":/dataset/runestone/earth_stone.png");
    fire_stone_pic.load(":/dataset/runestone/fire_stone.png");
    heart_stone_pic.load(":/dataset/runestone/heart_stone.png");
    light_stone_pic.load(":/dataset/runestone/light_stone.png");
    water_stone_pic.load(":/dataset/runestone/water_stone.png");

    runestone = new QLabel(parent);
    runestone->setFixedSize(dark_stone_pic.width(), dark_stone_pic.height());
    move(_row, _col);
    switch(clr) {
    case 0:
        change_color("dark");
        break;
    case 1:
        change_color("earth");
        break;
    case 2:
        change_color("fire");
        break;
    case 3:
        change_color("heart");
        break;
    case 4:
        change_color("light");
        break;
    case 5:
        change_color("water");
        break;
    }
    runestone->show();
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
}

void Runestone::stick_cursor(int _x, int _y) {
    runestone->move(_x-dark_stone_pic.width()/2-10, _y-dark_stone_pic.height()/2-10);
}





