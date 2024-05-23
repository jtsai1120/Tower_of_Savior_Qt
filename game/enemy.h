#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QPixmap>
#include <vector>
#include <QDebug>

using namespace std;

class Enemy
{
public:
    Enemy(QWidget *parent);
    QLabel *enemy_item;

    void cd_reset(int level);

    void new_round();

    vector <int> charac_heal;
    //vector <int> charac_hp;

    QLabel *attack_text;

    void show(int level,int enemy_photo);

    QPixmap water1;
    QPixmap fire1;
    QPixmap earth1;
    QPixmap light2;
    QPixmap earth2;
    QPixmap dark2;
    QPixmap fire3;

    vector<vector<int>> hp_list;
    vector<vector<int>> attack_list;
    vector<int> cd_list;
    vector<vector<QString>> enemy_clr_list;

    int hp;
    int attack;
    int cd;
    QString enemy_clr;

    bool dead = false;

private:


    bool all_atk = false; // 若消除一組>5顆，則攻擊為全體攻擊
};

#endif // ENEMY_H
