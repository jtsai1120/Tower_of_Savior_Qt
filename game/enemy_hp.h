#ifndef ENEMY_HP_H
#define ENEMY_HP_H

#include <QLabel>
#include <QPixmap>
#include <vector>
#include <QDebug>
using namespace std;

class Enemy_hp{

public:
    Enemy_hp(QWidget *parent);
    QPixmap hp_bar_icon;
    QLabel *hp_bar;

    void hp_lost(int level,int enemy,float damage);
    void reset(int level,int enemy);
    vector<vector<float>> hp_list;
    float total_hp;
    int current_hp = 90;
    int current_length;
    int max_length = 90;

};

#endif // ENEMY_HP_H
