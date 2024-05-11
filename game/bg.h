#ifndef BG_H
#define BG_H

#include <QPixmap>
#include <QLabel>
#include <QSound>

class Bg {
public:
    Bg(QWidget *parent = nullptr);
private:
    QSound *battle_bgm;

    QPixmap battle_bg_pic;
    QLabel *battle_bg;
    QPixmap runestone_bg_pic;
    QLabel *runestone_bg;
};

#endif // BG_H
