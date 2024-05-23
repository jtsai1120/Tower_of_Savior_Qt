#ifndef ICON_BAR_H
#define ICON_BAR_H

#include <QPixmap>
#include <QLabel>

class Icon_bar {
public:
    Icon_bar(QWidget *parent, double init_hp);
    void change_status(QString status, double prop);
    QLabel *hp_text;
    QLabel *heal_text;

    double initial_hp;

private:
    QPixmap cd_icon_pic;
    QPixmap hp_icon_pic;
    QLabel *cd_hp_icon;

    QPixmap cd_bar_pic;
    QPixmap hp_bar_pic;
    QLabel *cd_hp_bar;

    double hp_bar_length;

    QString cur_status;
};

#endif // ICON_BAR_H
