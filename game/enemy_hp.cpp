#include "enemy_hp.h"

Enemy_hp::Enemy_hp(QWidget *parent){
    hp_bar_icon.load(":/dataset/ui/hp_bar.png");
    hp_bar = new QLabel(parent);
    hp_bar->setPixmap(hp_bar_icon);
    hp_bar->setFixedSize(90,5);
    hp_list = {{100,100,100},
               {100,300,100},
               {700}};
    current_length = 90;
    max_length = 90;
}

void Enemy_hp::hp_lost(int level,int enemy,float damage){
    total_hp = hp_list[level-1][enemy];
    float percent = damage/total_hp;
    if (current_hp - damage <= 0)
        hp_bar->move(0,1000);
    else{
        if (current_length - max_length*percent > 0){
            hp_bar->setFixedWidth(current_length - max_length*percent);
            current_length -= max_length*percent;
        }
        else
            hp_bar->setFixedWidth(0);
        current_hp -= damage;

    }
}

void Enemy_hp::reset(int level,int enemy){
    total_hp = hp_list[level-1][enemy];
    current_hp = total_hp;
    if (level == 1 || level ==2){
        current_length = 90;
        hp_bar->setFixedSize(90,5);
        max_length = 90;
    }
    else {
        current_length = 260;
        hp_bar->setFixedSize(260,5);
        max_length = 260;
    }

}

void Enemy_hp::changeImageColor(int level, int enemy){
    QPixmap coloredPixmap = hp_bar_icon;
    // 創建用於繪製圖像的Qpainter
    QPainter painter(&coloredPixmap);
    // 根據敵人屬性上色
    QColor newColor(255, 0, 0); // 預設為紅色
    if (level == 1){
        if (enemy == 0) newColor.setRgb(0,0,139); //blue
        if (enemy == 1) newColor.setRgb(139,0,0); //red
        if (enemy == 2) newColor.setRgb(0,100,0); //green
    }
    if (level == 2){
        if (enemy == 0) newColor.setRgb(204,204,0);//light
        if (enemy == 1) newColor.setRgb(0,100,0);//green
        if (enemy == 2) newColor.setRgb(75,0,130);//dark
    }
    if (level == 3){
        if (enemy == 0) newColor.setRgb(139,0,0); //red
    }

    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(coloredPixmap.rect(), newColor);
    painter.end();

    // 在 QLabel 上显示修改后的图像
    hp_bar->setPixmap(coloredPixmap);
}
