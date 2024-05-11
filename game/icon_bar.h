#ifndef ICON_BAR_H
#define ICON_BAR_H

#include <QPixmap>
#include <QLabel>

class Icon_bar {
public:
    Icon_bar(QWidget *parent);
    void change_status(QString status, double prop);

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
