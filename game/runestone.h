#ifndef RUNESTONE_H
#define RUNESTONE_H

#include <QPixmap>
#include <QLabel>
#include <QPropertyAnimation>
using namespace std;

class Runestone {
public:
    Runestone(QWidget *parent, int _row, int _col, QString clr);
    ~Runestone();

    QString get_color() const { return color; }

    void move(int row, int col);
    void change_color(QString clr, int status);

    void drop(int row, int _row);

    void stick_cursor(int _x, int _y);

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

    QPropertyAnimation *animation;
};

#endif // RUNESTONE_H
