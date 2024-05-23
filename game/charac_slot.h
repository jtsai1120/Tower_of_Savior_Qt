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

    void change_charac();
    int charac_ID = -1;

    void add_attack(Runestone_pair rp);
    void new_round();

    vector <int> charac_heal;
    //vector <int> charac_hp;

    QLabel *attack_text;
    int attack = 0;
    QString attribute; //紀錄角色屬性

private:
    QPixmap empty_slot;
    vector <QPixmap> charac_pics;

    vector <QString> charac_clr;
    vector <int> charac_atk;

    bool all_atk = false; // 若消除一組>5顆，則攻擊為全體攻擊
};

#endif // CHARAC_SLOT_H
