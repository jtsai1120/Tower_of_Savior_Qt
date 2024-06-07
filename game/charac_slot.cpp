#include "charac_slot.h"

Charac_slot::Charac_slot(QWidget *parent){
    charac_ID = -1;
    empty_slot.load(":/dataset/ui/charac slot.jpg");
    vector<QString> paths;
    paths = {
        {":/dataset/character/ID1.png"},
        {":/dataset/character/ID2.png"},
        {":/dataset/character/ID3.png"},
        {":/dataset/character/ID4.png"},
        {":/dataset/character/ID5.png"},
        {":/dataset/character/ID6.png"},// GBC
        {":/dataset/character/ID7.png"},
        {":/dataset/character/ID8.png"},
        {":/dataset/character/ID9.png"},
        {":/dataset/character/ID10.png"},
        {":/dataset/character/ID11.png"},// bocchi
        {":/dataset/character/ID12.png"},
        {":/dataset/character/ID13.png"},
        {":/dataset/character/ID14.png"},
        {":/dataset/character/ID15.png"}
    };
    Bocchi_skill_pic.load(":/dataset/character/ID11-2.png");
    Bocchi_skill_pic_1.load(":/dataset/character/ID15-2.png");

    charac_clr = {
        {"water"},
        {"fire"},
        {"earth"},
        {"light"},
        {"dark"},
        {"dark"},// GBC
        {"water"},
        {"earth"},
        {"fire"},
        {"light"},
        {"dark"},// bocchi
        {"light"},
        {"water"},
        {"fire"},
        {"earth"},
    };

    charac_atk = {80, 220, 120, 130, 280,
                  330, 280, 180, 260, 230,
                 330, 280, 180, 260, 230};
    charac_heal = {20, 2, 15, 11, 8,
                   12, 18, 36, 13, 24,
                  12, 18, 36, 13, 24};
    charac_hp = {600, 300, 450, 350, 180,
                 380, 500, 800, 410, 620,
                380, 500, 800, 410, 620};
    charac_CD = {5, 5, 5, 5, 5,
                 6, 5, 6, 7, 8,
                5, 8, 6, 6, 7};
    CD = -1;
    skill_power = -1;
    extra_atk = 1;
    hit_more = -1;

    charac_pics.resize(paths.size());
    for (int i = 0; i < int(paths.size()); i++){
        charac_pics[i].load(paths[i]);
    }

    charac_item = new QLabel(parent);
    charac_item->setPixmap(empty_slot);

    // 攻擊力顯示
    attack_text = new QLabel(parent);
    QFont attack_text_font("Consolas", 15, QFont::Bold);
    attack_text->setFont(attack_text_font);
    attack_text->resize(200, 200);

    damage_text = new QLabel(parent);
    QFont damage_text_font("Consolas", 10, QFont::Bold);
    damage_text->setStyleSheet("color: white");
    damage_text->setFont(damage_text_font);
    damage_text->resize(200, 200);
    damage_text->show();
    damage_text->move(0,1000);
}

void Charac_slot::change_charac(int is_lead, bool is_basic){
    basic = is_basic;
    leader = is_lead;

    if (charac_ID == -1) charac_ID = 5;
    else if (charac_ID + 1 < int(charac_pics.size()))
        charac_ID++;
    else
        charac_ID = 0;
    if (leader == -2) leader = charac_ID;

    if (!basic){
        CD = charac_CD[charac_ID]; // 設定CD
        if (leader == 9 || leader == 11) CD--;
    }

    charac_item->setPixmap(charac_pics[charac_ID]);

    attribute = charac_clr[charac_ID];

    if (charac_clr[charac_ID] == "water") attack_text->setStyleSheet("color: aqua");
    else if (charac_clr[charac_ID] == "fire") attack_text->setStyleSheet("color: red");
    else if (charac_clr[charac_ID] == "earth") attack_text->setStyleSheet("color: lime");
    else if (charac_clr[charac_ID] == "light") attack_text->setStyleSheet("color: yellow");
    else if (charac_clr[charac_ID] == "dark") attack_text->setStyleSheet("color: purple");
    attack_text->setText(QString::number(charac_atk[charac_ID]));
    if(basic)attack_text->setText("1");
    attack_text->show();
}


void Charac_slot::add_attack(Runestone_pair rp){
    int temp_atk = charac_atk[charac_ID];
    if (skill_power == 5 || skill_power == 12)
        temp_atk *= 3;

    QString clr = rp.color;
    if (charac_clr[charac_ID] == clr){
        if(basic) attack += rp.pair.size();
        else attack += temp_atk * rp.pair.size();
        if (rp.pair.size() > 4)
            all_atk = true;
        qDebug()<< "all_atk = "<<all_atk;
    }
    else if (!basic){
        if (leader == 5){
            if ((charac_clr[charac_ID] == "water" || charac_clr[charac_ID] == "earth" || charac_clr[charac_ID] == "dark")
                    && (clr == "water" || clr == "earth" || clr == "dark")){
                attack += temp_atk * rp.pair.size();
                if (rp.pair.size() > 4)
                    all_atk = true;
                qDebug()<< "all_atk = "<<all_atk;
            }
        }
        if (leader == 6 || leader == 10){
            if (clr == "heart"){
                attack += temp_atk * rp.pair.size();
                if (rp.pair.size() > 4)
                    all_atk = true;
                qDebug()<< "all_atk = "<<all_atk;
            }
        }
        if (leader == 12){
            if (charac_clr[charac_ID] != "earth" && clr != "earth" && clr != "heart"){
                attack += temp_atk * rp.pair.size();
                if (rp.pair.size() > 4)
                    all_atk = true;
                qDebug()<< "all_atk = "<<all_atk;
            }
        }
    }

    if (skill_power == 6 || skill_power == 10)
        if (clr == "heart")
            extra_atk += rp.pair.size();
}

void Charac_slot::new_round(){
    if (charac_ID == 10) charac_item->setPixmap(charac_pics[10]);
    if (charac_ID == 14 && hit_more == -1) charac_item->setPixmap(charac_pics[14]);
    attack = 0;
    if (CD > 0 && !basic) CD--;
    skill_power = -1;
    extra_atk = 1;
    all_atk = false;
}

void Charac_slot::CD_reset(){
    if (charac_ID == 10) charac_item->setPixmap(Bocchi_skill_pic);
    else if (charac_ID == 14) charac_item->setPixmap(Bocchi_skill_pic_1);

    CD = charac_CD[charac_ID];
    skill_power = charac_ID;
    if (leader == 9 || leader == 11) CD--;
    qDebug()<<"character"<<(charac_ID+1)<<"skill activated";
}

void Charac_slot::reset(){
    leader = -1;
    charac_ID = -1;
    hit_more = -1;
    CD = -1;
    skill_power = -1;
    extra_atk = 1;
    attack = 0;
    all_atk = false;
    charac_item->setPixmap(empty_slot);
}

