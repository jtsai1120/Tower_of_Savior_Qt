#include "charac_slot.h"

Charac_slot::Charac_slot(QWidget *parent){
    empty_slot.load(":/dataset/ui/charac slot.jpg");
    vector<QString> paths;
    paths = {
        {":/dataset/character/ID1.png"},
        {":/dataset/character/ID2.png"},
        {":/dataset/character/ID3.png"},
        {":/dataset/character/ID4.png"},
        {":/dataset/character/ID5.png"}
    };

    charac_clr = {
        {"water"},
        {"fire"},
        {"earth"},
        {"light"},
        {"dark"}
    };

    charac_atk = {80, 200, 120, 130, 180};
    charac_heal = {20, 2, 15, 11, 8};

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
}

void Charac_slot::change_charac(bool is_basic){
    basic = is_basic;

    if (charac_ID + 1 < int(charac_pics.size()))
        charac_ID++;
    else
        charac_ID = 0;
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
    QString clr = rp.color;

    if (charac_clr[charac_ID] == clr){
        if(basic) attack += rp.pair.size();
        else attack += charac_atk[charac_ID] * rp.pair.size();
        if (rp.pair.size() > 5)
            all_atk = true;
        qDebug()<< "all_atk = "<<all_atk;
    }
}

void Charac_slot::new_round(){
    attack = 0;
    all_atk = false;
}

