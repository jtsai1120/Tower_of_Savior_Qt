#include "enemy.h"

Enemy::Enemy(QWidget *parent){
    vector<vector<QString>> enemy;

    water1.load(":/dataset/enemy/96n.png");
    fire1.load(":/dataset/enemy/98n.png");
    earth1.load(":/dataset/enemy/100n.png");
    light2.load(":/dataset/enemy/102n.png");
    earth2.load(":/dataset/enemy/267n.png");
    dark2.load(":/dataset/enemy/104n.png");
    fire3.load(":/dataset/enemy/180n.png");

    enemy_item = new QLabel(parent);
    enemy_item->setPixmap(water1);

    hp_list = {{100,100,100},
               {100,300,100},
               {700}};
    attack_list = {{200,200,200},
                   {200,200,200},
                   {400}};
    cd_list = {3,3,5};

    enemy_clr_list = {{"water","fire","earth"},
                      {"light","earth","dark"},
                      {"fire"}};

}

void Enemy::show(int level,int enemy_photo){ //level starts from 1
    if (level == 1){
        if (enemy_photo == 0)
            enemy_item->setPixmap(water1);
        if (enemy_photo == 1)
            enemy_item->setPixmap(fire1);
        if (enemy_photo == 2)
            enemy_item->setPixmap(earth1);
    }
    if (level == 2){
        if (enemy_photo == 0)
            enemy_item->setPixmap(light2);
        if (enemy_photo == 1)
            enemy_item->setPixmap(earth2);
        if (enemy_photo == 2)
            enemy_item->setPixmap(dark2);
    }
    if (level == 3){
        if (enemy_photo == 0){
            enemy_item->setFixedSize(256,256);
            enemy_item->setPixmap(fire3);
        }
    }
    hp = hp_list[level-1][enemy_photo];
    attack = attack_list[level-1][enemy_photo];
    cd = cd_list[level-1];
    enemy_clr = enemy_clr_list[level-1][enemy_photo];
    dead = false;
}

void Enemy::cd_reset(int level){
    cd = cd_list[level-1];
}
