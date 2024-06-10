#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPixmap>
#include <QPushButton>
#include <QEvent>
#include <QPoint>
#include <QMouseEvent>
#include <QLabel>
#include <QSound>
#include <vector>
#include <random>
#include <chrono>
#include <QApplication>

#include "ButtonItem.h"
#include "charac_slot.h"
#include "bg.h"
#include "icon_bar.h"
#include "runestone.h"
#include "runestone_pair.h"
#include "combo_counter.h"
#include "light_halo_vfx.h"
#include "enemy.h"
#include "enemy_hp.h"
#include "bullet.h"
using namespace std;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    QString random_runestone_color();

    Combo_counter combo_counter;
    void combo_count_and_drop(bool is_first_count = true);
    void combo_count();
    void drop_detect();
    void game_over();
    void show_damage(QLabel *text, int seconds);


    int level = 1;


public slots:
    void combo_eliminate();
    void on_start_button_clicked();
    void on_surrender_button_clicked();
    void on_back_button_clicked();
    void on_restart_button_clicked();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    int game_status;
    int attack_wait_count = -1; // 為了讓角色可以延遲攻擊

    Bg *bg; // battle_bg, runestone_bg, battle_bgm
    Icon_bar *icon_bar; // cd_icon, hp_icon, cd_bar, hp_bar

    long long seed;
    mt19937 gen;
    uniform_int_distribution<int> dist;

    vector<vector<Runestone*>> runestones;

    bool can_move_runestone;
    pair<int,int> selected_runestone;
    bool runestone_selected;
    bool runestone_swap;
    bool runestone_drift;
    QTimer *drift_timer;
    bool drift_timer_started;
    int ms_elapsed;
    int time_limit = 10;


    vector<Light_halo_vfx*> light_halo_vfxs;

    vector<Runestone_pair> combo_counter_result;
    int combo, cur_stage_combo;
    int cur_pair_num;
    QTimer *combo_cd;

    vector<int> col_bottom;

    QLabel *combo_text;

    vector<vector<Runestone*>> tmp_runestones;
    vector<vector<pair<QString,int>>> result_colors;

    // start btn
    QPushButton *start_button = new QPushButton;
    QPixmap start_button_pic;
    ButtonItem *start_button_item;

    QPixmap ui_button_pic;
    QLabel *ui_button;
    bool basic = true;

    // characters
    vector<Charac_slot*> charac_slots;

    //enemies
    vector<Enemy*> enemy;

    //enemy_hp
    vector<Enemy_hp*> enemy_hp;

    // darken
    QPixmap darken_pic;
    QLabel *darken;
    QGraphicsOpacityEffect *darken_opacityEffect;

    // 燃燒位置
    vector<pair<int,int>> burning;
    vector<int> survive = {0,1,2};

    //show bullet
    vector<Bullet*> bullet;

    //attack all
    QLabel *attack_all;
    QPixmap attack_all_pic;
    QGraphicsOpacityEffect *attack_opacityEffect;

    QLabel *ui_text;
    QLabel *skill_text;
    QLabel *cd_text1;
    QLabel *cd_text2;
    QLabel *cd_text3;
    QLabel *gameover_text;

    vector<vector<int>> damage_text_pos = {{10,10},{15,20},{-50,-30},{-27,-47},{36,-20},{-18,30}};

    // 設定介面
    QPixmap setting_pic;
    QLabel *setting;

    QPushButton *surrender_button;
    QPixmap surrender_button_pic;
    ButtonItem *surrender_button_item;

    QPixmap full_darken_pic;
    QLabel *full_darken;
    QGraphicsOpacityEffect *full_darken_opacityEffect;

    QPushButton *back_button;
    QPixmap back_button_pic;
    ButtonItem *back_button_item;

    // Restart Button
    QPushButton *restart_button;
    QPixmap restart_button_pic;
    ButtonItem *restart_button_item;

    double init_hp = 2000;
    int hp;
    int heal = 0; // 回復量
    int init_heal = 0; // 初始回復量
    int harm; // 所受傷害
    bool burn_road; // 燃燒軌跡技能生效
    bool move_free = false; // 排珠
    int attack_enemy;
    double damage;

    int skill; // 技能號碼
    bool double_combo; // combo兩倍技能效果

    /*
    vector<QString> skill_descript = {
        {"元素爆擊 CD 5 \n  還原、轉換固定版面\n  自身必定剋制\n\n隊長技能: atk 2倍，CD減至0"},
        {"元素爆擊 CD 5 \n  還原、轉換固定版面\n  自身必定剋制\n\n隊長技能: atk 2倍，CD減至0"},
        {"元素爆擊 CD 5 \n  還原、轉換固定版面\n  自身必定剋制\n\n隊長技能: atk 2倍，CD減至0"},
        {"元素爆擊 CD 5 \n  還原、轉換固定版面\n  自身必定剋制\n\n隊長技能: atk 2倍，CD減至0"},
        {"元素爆擊 CD 5 \n  還原、轉換固定版面\n  自身必定剋制\n\n隊長技能: atk 2倍，CD減至0"},
        {"我要證明，我們並沒有錯! CD 6 \n  轉換固定版面，\n  一回合內atk 3倍，消除燃燒軌跡，轉珠延長至20秒\n\n隊長技能: 水木暗互相兼具，hp 1.5倍"},
        {"錯的人是我。 CD 4 \n  根據消除的心數量向上提升atk\n\n隊長技能: 心兼具所有屬性效果，\n  回合開始轉換固定位置心，hp 1.5倍"},
        {"不要再叫我騙子了! CD 6 \n  一回合內排珠，自身必定剋制，\n  身旁隊友CD-1\n\n隊長技能: 消除8相連延遲CD最少敵人並增加8combo，hp 1.5倍"},
        {"你們再吵架我就退出! CD 7 \n  combo數 2倍及額外追擊2次，\n  效果持續至自身沒有攻擊\n\n隊長技能: 自身CD減至0, 吸收右方隊員atk，hp 1.5倍"},
        {"因此我也需要搖滾。 CD 8 \n  全隊atk 2倍，免疫\n  燃燒轉回血\n\n隊長技能: 所有人跟隨最高atk,所有技能CD縮減1, hp1.5倍"},
        {"我不想要就這樣! CD 4 \n  根據消除的心數量向上提升atk\n\n隊長技能: 心兼具所有屬性效果，\n  回合開始轉換固定位置心，hp 1.5倍"},
        {"先保持秘密吧! CD 8 \n  全隊atk 2倍，免疫\n  燃燒轉回血\n\n隊長技能: 所有人跟隨最高atk,所有技能CD縮減1, hp1.5倍"},
        {"我這個人向來說到做到。 CD 6 \n  轉換固定版面，\n  一回合內atk 3倍，消除燃燒軌跡，轉珠延長至20秒\n\n隊長技能: 水火光暗互相兼具，hp 1.5倍"},

    };
    */

    vector<QString> skill_descript = {
        {"Element Explode CD 5 \n  Return, turn fixed layout\n  Self attack advance attribute\n\nLeader: atk x2, CD cut down to 0"},
        {"Element Explode CD 5 \n  Return, turn fixed layout\n  Self attack advance attribute\n\nLeader: atk x2, CD cut down to 0"},
        {"Element Explode CD 5 \n  Return, turn fixed layout\n  Self attack advance attribute\n\nLeader: atk x2, CD cut down to 0"},
        {"Element Explode CD 5 \n  Return, turn fixed layout\n  Self attack advance attribute\n\nLeader: atk x2, CD cut down to 0"},
        {"Element Explode CD 5 \n  Return, turn fixed layout\n  Self attack advance attribute\n\nLeader: atk x2, CD cut down to 0"},

        {"I'm gonna prove we're not wrong!\n CD 6 \n  Turn fixed layout, atk x3,\n  Eliminate 'burn while move'\n  Drift time 20s\n\nLeader: dark, water, earth\n  has others' effect, hp x1.5"},
        {"The only problem is me. CD 5 \n  atk xamount of heart eliminates\n\nLeader:\n  Heart shares all attributes,\n  Fixed heart every round, hp x1.5"},
        {"Stop calling me a liar! CD 6 \n  Move free,\n  Self attack advance attribute, \n  Teammates aside CD-1\n\nLeader:\n  Eliminate 8+ connected,\n  enemy with shortest CD+1,\n  combo +8, hp x1.5"},
        {"Stop fighting or I'll quit! CD 7 \n  combo x2, Hits +2 times,\n  Effect continues until\n  self atk = 0\n\nLeader:\n  Self CD cut down to 0,\n  Eats up right side's atk,\n  hp x1.5"},
        {"This is why I need to rock. CD 8 \n  Whole team atk x2,\n  Enemy atk = 0,\n  Burning heals instead\n\nLeader:\n  Whole team's atk copies highest,\n  All CD shrink 1, hp x1.5"},

        {"I don't want to stay this way!\n CD 5 \n  atk xamount of heart eliminates\n\nLeader:\n  Heart shares all attributes,\n  Fixed heart every round, hp x1.5"},
        {"Keep it secret for now! CD 8 \n  Whole team atk x2,\n  Enemy atk = 0,\n  Burning heals instead\n\nLeader:\n  Whole team's atk copies highest,\n  All CD shrink 1, hp x1.5"},
        {"I always do what I say.\n CD 6 \n  Turn fixed layout, atk x3,\n  Eliminate 'burn while move'\n  Drift time 20s\n\nLeader: water, fire, light, dark\n  has others' effect, hp x1.5"},
        {"Fuwa Fuwa~! Buwa Buwa! CD 6 \n  Move free,\n  Self attack advance attribute, \n  Teammates aside CD-1\n\nLeader:\n  Eliminate 8+ connected,\n  enemy with shortest CD+1,\n  combo +8, hp x1.5"},
        {"Likes! I want more likes! CD 7 \n  combo x2, Hits +2 times,\n  Effect continues until\n  self atk = 0\n\nLeader:\n  Self CD cut down to 0,\n  Eats up right side's atk,\n  hp x1.5"},
    };

};

#endif // MAINWINDOW_H
