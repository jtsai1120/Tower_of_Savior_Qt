#include "enemy_hp.h"

Enemy_hp::Enemy_hp(QWidget *parent){
    hp_bar_icon.load(":/dataset/ui/hp_bar.png");
    hp_bar = new QLabel(parent);
    hp_bar->setPixmap(hp_bar_icon);
    hp_bar->setFixedSize(90,5);
    hp_list = {{100,100,100},
               {100,300,100},
               {700}};
    current_length = 90;
    max_length = 90;
}

void Enemy_hp::hp_lost(int level,int enemy,float damage){
    total_hp = hp_list[level-1][enemy];
    float percent = damage/total_hp;
    if (current_hp - damage <= 0)
        hp_bar->move(0,1000);
    else{
        if (current_length - max_length*percent > 0){
            hp_bar->setFixedWidth(current_length - max_length*percent);
            current_length -= max_length*percent;
        }
        else
            hp_bar->setFixedWidth(0);
        current_hp -= damage;

    }
}

void Enemy_hp::reset(int level,int enemy){
    total_hp = hp_list[level-1][enemy];
    current_hp = total_hp;
    if (level == 1 || level ==2){
        current_length = 90;
        hp_bar->setFixedSize(90,5);
        max_length = 90;
    }
    else {
        current_length = 260;
        hp_bar->setFixedSize(260,5);
        max_length = 260;
    }

}
