#include "bullet.h"
#include <QPainter>
#include <QTimer>
#include <cmath>
#include <QPixmap>


Bullet::Bullet(QWidget *parent){
    bullet_pic.load(":/dataset/ui/attack.png");

    bullet_item = new QLabel(parent);
    bullet_item->setPixmap(bullet_pic);

    bullet_item->setFixedSize(100,100);
    shoot_animation = new QPropertyAnimation(bullet_item, "geometry");

}

void Bullet::shoot(qreal endx, qreal endy){
    shoot_animation->setDuration(350); // 持續時間0.35秒
    shoot_animation->setStartValue(QRect(bullet_item->x(), bullet_item->y(), bullet_item->width(), bullet_item->height()));
    shoot_animation->setEndValue(QRect(endx, endy, bullet_item->width(), bullet_item->height())); // 終點位置
    shoot_animation->start();
}




