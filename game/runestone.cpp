#include "runestone.h"
#include <QDebug>

Runestone::Runestone(QWidget *parent, int _row, int _col, QString clr) {
    dark_stone_pic.load(":/dataset/runestone/dark_stone.png");
    earth_stone_pic.load(":/dataset/runestone/earth_stone.png");
    fire_stone_pic.load(":/dataset/runestone/fire_stone.png");
    heart_stone_pic.load(":/dataset/runestone/heart_stone.png");
    light_stone_pic.load(":/dataset/runestone/light_stone.png");
    water_stone_pic.load(":/dataset/runestone/water_stone.png");

    burn_dark_stone_pic.load(":/dataset/runestone/burning_dark_stone.png");
    burn_earth_stone_pic.load(":/dataset/runestone/burnibg_earth_stone.png");
    burn_fire_stone_pic.load(":/dataset/runestone/burning_fire_stone.png");
    burn_heart_stone_pic.load(":/dataset/runestone/burning_heart_stone.png");
    burn_light_stone_pic.load(":/dataset/runestone/burning_light_stone.png");
    burn_water_stone_pic.load(":/dataset/runestone/burning_water_stone.png");

    weathered_dark_stone_pic.load(":/dataset/runestone/weathered_dark_stone.png");
    weathered_earth_stone_pic.load(":/dataset/runestone/weathered_earth_stone.png");
    weathered_fire_stone_pic.load(":/dataset/runestone/weathered_fire_stone.png");
    weathered_heart_stone_pic.load(":/dataset/runestone/weathered_heart_stone.png");
    weathered_light_stone_pic.load(":/dataset/runestone/weathered_light_stone.png");
    weathered_water_stone_pic.load(":/dataset/runestone/weathered_water_stone.png");

    runestone = new QLabel(parent);
    runestone->setFixedSize(dark_stone_pic.width(), dark_stone_pic.height());
    move(_row, _col);
    change_color(clr, 0);
    runestone->show();

    status = 0; // 0為正常, 1為燃燒, 2為風化

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

void Runestone::change_color(QString clr, int get_status) {
    color = clr;
    status = get_status;
    // 正常符石
    if (status == 0){
        if (clr == "dark") runestone->setPixmap(dark_stone_pic);
        else if (clr == "earth") runestone->setPixmap(earth_stone_pic);
        else if (clr == "fire") runestone->setPixmap(fire_stone_pic);
        else if (clr == "heart") runestone->setPixmap(heart_stone_pic);
        else if (clr == "light") runestone->setPixmap(light_stone_pic);
        else if (clr == "water") runestone->setPixmap(water_stone_pic);
        else if (clr == "empty") runestone->setPixmap(QPixmap());
    } else if (status == 1){ // 燃燒符石
        if (clr == "dark") runestone->setPixmap(burn_dark_stone_pic);
        else if (clr == "earth") runestone->setPixmap(burn_earth_stone_pic);
        else if (clr == "fire") runestone->setPixmap(burn_fire_stone_pic);
        else if (clr == "heart") runestone->setPixmap(burn_heart_stone_pic);
        else if (clr == "light") runestone->setPixmap(burn_light_stone_pic);
        else if (clr == "water") runestone->setPixmap(burn_water_stone_pic);
        else if (clr == "empty") runestone->setPixmap(QPixmap());
    } else if (status == 2){ // 風化符石
        if (clr == "dark") runestone->setPixmap(weathered_dark_stone_pic);
        else if (clr == "earth") runestone->setPixmap(weathered_earth_stone_pic);
        else if (clr == "fire") runestone->setPixmap(weathered_fire_stone_pic);
        else if (clr == "heart") runestone->setPixmap(weathered_heart_stone_pic);
        else if (clr == "light") runestone->setPixmap(weathered_light_stone_pic);
        else if (clr == "water") runestone->setPixmap(weathered_water_stone_pic);
        else if (clr == "empty") runestone->setPixmap(QPixmap());
    }

}

void Runestone::stick_cursor(int _x, int _y) {
    runestone->move(_x-dark_stone_pic.width()/2-10, _y-dark_stone_pic.height()/2-10);
}

void Runestone::drop(QString after_drop_change_clr, int drop_speed) {
    change_color(after_drop_change_clr, status);
    animation->setDuration(drop_speed-30);
    animation->setStartValue(QPoint(runestone->x(), runestone->y()-90));
    animation->setEndValue(QPoint(runestone->x(), runestone->y()));
    animation->start();
}




