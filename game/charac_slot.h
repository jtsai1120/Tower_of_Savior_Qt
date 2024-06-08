#ifndef CHARAC_SLOT_H
#define CHARAC_SLOT_H

#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QPixmap>
#include <vector>
#include <QDebug>

#include "runestone.h"
#include "runestone_pair.h"

using namespace std;

class Charac_slot{
public:
    Charac_slot(QWidget *parent);
    QLabel *charac_item;

    void change_charac(bool basic);
    int charac_ID;
    int CD;
    int skill_power;
    int extra_atk;

    void add_attack(Runestone_pair rp);
    void new_round();
    void CD_reset();
    void reset();

    vector <int> charac_heal;
    vector <int> charac_hp;

    QLabel *attack_text;
    int attack = 0;
    QString attribute; //紀錄角色屬性

    QLabel *damage_text;

    int hit_more;
    int leader;
    bool all_atk = false; // 若消除一組>5顆，則攻擊為全體攻擊

private:
    QPixmap empty_slot;
    vector <QPixmap> charac_pics;
    QPixmap Bocchi_skill_pic;
    QPixmap Bocchi_skill_pic_1;

    vector <QString> charac_clr;
    vector <int> charac_atk;
    vector <int> charac_CD;


    bool basic;

};

#endif // CHARAC_SLOT_H
