#ifndef RUNESTONE_H
#define RUNESTONE_H

#include <QPixmap>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsOpacityEffect>
using namespace std;

class Runestone {
public:
    Runestone(QWidget *parent, int _row, int _col, QString clr, int get_status=0);
    ~Runestone();

    QString get_color() const { return color; }
    int get_status() const { return status; }

    void move(int row, int col);
    void change_color(QString clr, int status);

    void drop(int row, int _row);

    void stick_cursor(int _x, int _y);

    //void set_opacity(float n);

    void game_over_drop();

    int status;

private:
    QPixmap dark_stone_pic;
    QPixmap earth_stone_pic;
    QPixmap fire_stone_pic;
    QPixmap heart_stone_pic;
    QPixmap light_stone_pic;
    QPixmap water_stone_pic;
    QPixmap burn_dark_stone_pic;
    QPixmap burn_earth_stone_pic;
    QPixmap burn_fire_stone_pic;
    QPixmap burn_heart_stone_pic;
    QPixmap burn_light_stone_pic;
    QPixmap burn_water_stone_pic;
    QPixmap weathered_dark_stone_pic;
    QPixmap weathered_earth_stone_pic;
    QPixmap weathered_fire_stone_pic;
    QPixmap weathered_heart_stone_pic;
    QPixmap weathered_light_stone_pic;
    QPixmap weathered_water_stone_pic;

    QLabel *runestone;

    QString color;
    QString speicial_status;
    int row, col;

    QPropertyAnimation *drop_animation;
    QPropertyAnimation *game_over_drop_animation;

   // QGraphicsOpacityEffect *opacityEffect;
};

#endif // RUNESTONE_H
