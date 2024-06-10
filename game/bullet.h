#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QPixmap>
#include <vector>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsOpacityEffect>

using namespace std;

class Bullet{
public:
    Bullet(QWidget *parent);
    QPixmap bullet_pic;

    QLabel *bullet_item;

    void shoot(qreal endx, qreal endy);
    void finish();

    QPropertyAnimation *shoot_animation;

};

#endif // BULLET_H
