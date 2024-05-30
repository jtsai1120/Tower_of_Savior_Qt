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

    int level = 1;

public slots:
    void combo_eliminate();
    void on_start_button_clicked();

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
    const int time_limit = 10;


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

    // 燃燒位置
    vector<pair<int,int>> burning;
    vector<int> survive = {0,1,2};

    QLabel *ui_text;
    QLabel *skill_text;

    QLabel *gameover_text;

    double init_hp = 2000;
    int hp;
    int heal = 0; // 回復量
    int init_heal = 0; // 初始回復量
    int harm; // 所受傷害
    bool burn_road; // 燃燒軌跡技能生效
    int attack_enemy;
    double damage;

    vector<QString> skill_descript = {
        {"隊長技能: atk 2倍"},
        {"隊長技能: atk 2倍"},
        {"隊長技能: atk 2倍"},
        {"隊長技能: atk 2倍"},
        {"隊長技能: atk 2倍"},
        {"我要證明，我們並沒有錯! CD 8\n  還原、轉換固定版面，\n  一回合內消除所有燃燒效果\n\n隊長技能: 水火暗互相兼具，hp 1.5倍"},
        {"技能敘述: 無"},
        {"技能敘述: 無"},
        {"技能敘述: 無"},
        {"隊長技能: 每combo將提升atk 30%； hp 1.5倍"},
    };
};

#endif // MAINWINDOW_H
