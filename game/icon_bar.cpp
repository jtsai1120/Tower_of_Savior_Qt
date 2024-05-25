#include "icon_bar.h"
#include <QDebug>

Icon_bar::Icon_bar(QWidget *parent, double init_hp) {
    cd_icon_pic.load(":/dataset/ui/cd_icon.png");
    hp_icon_pic.load(":/dataset/ui/hp_icon.png");
    cd_hp_icon = new QLabel(parent);
    cd_hp_icon->setFixedSize(cd_icon_pic.width(), cd_icon_pic.height());
    cd_hp_icon->move(0,510-cd_hp_icon->height());

    cd_bar_pic.load(":/dataset/ui/cd_bar.png");
    hp_bar_pic.load(":/dataset/ui/hp_bar.png");
    cd_bar_pic = cd_bar_pic.scaledToWidth(cd_bar_pic.width()+10);
    hp_bar_pic = hp_bar_pic.scaledToWidth(hp_bar_pic.width()+10);
    hp_bar_length = hp_bar_pic.width();
    cd_hp_bar = new QLabel(parent);
    cd_hp_bar->setFixedSize(cd_bar_pic.width(), cd_bar_pic.height());
    cd_hp_bar->move(cd_hp_icon->width()-10, 510-cd_hp_bar->height()-cd_hp_icon->height()/4);
    cd_hp_icon->raise();

    change_status("hp", 0.0);

    // set hp text
    initial_hp = init_hp;
    hp_text = new QLabel(parent);
    QFont hp_text_font("Consolas", 15, QFont::Bold);
    hp_text->setFont(hp_text_font);
    hp_text->setStyleSheet("color: pink");
    hp_text->resize(200, 200);
    hp_text->move(360, 385);
    hp_text->show();
    hp_text->setText(QString::number(initial_hp) + "/" + QString::number(initial_hp));

    // set healing text
    heal_text = new QLabel(parent);
    QFont heal_text_font("Consolas", 15, QFont::Bold);
    heal_text->setFont(heal_text_font);
    heal_text->setStyleSheet("color: lime");
    heal_text->resize(200, 200);
    heal_text->move(250, 380);
    heal_text->show();

}

void Icon_bar::change_status(QString status, double prop) {
    cur_status = status;
    if (status == "cd") {
        cd_hp_icon->setPixmap(cd_icon_pic);
        cd_hp_bar->setPixmap(cd_bar_pic);
        cd_hp_bar->setFixedWidth(double(cd_bar_pic.width())*(1.0-prop));
        hp_text->hide();
    } else {
        cd_hp_icon->setPixmap(hp_icon_pic);
        cd_hp_bar->setPixmap(hp_bar_pic);
        cd_hp_bar->setFixedWidth(double(hp_bar_pic.width())*(1.0-prop));
    }
}
