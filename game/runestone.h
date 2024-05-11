#ifndef RUNESTONE_H
#define RUNESTONE_H

#include <QPixmap>
#include <QLabel>
using namespace std;

class Runestone {
public:
    Runestone(QWidget *parent, int _row, int _col, int clr);
    ~Runestone();

    QString get_color() const { return color; }

    void move(int row, int col);
    void change_color(QString clr);

    void stick_cursor(int _x, int _y);

private:
    QPixmap dark_stone_pic;
    QPixmap earth_stone_pic;
    QPixmap fire_stone_pic;
    QPixmap heart_stone_pic;
    QPixmap light_stone_pic;
    QPixmap water_stone_pic;
    QLabel *runestone;

    QString color;
    QString speicial_status;
    int row, col;
};

#endif // RUNESTONE_H
