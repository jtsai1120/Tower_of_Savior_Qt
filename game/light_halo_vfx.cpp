#include "light_halo_vfx.h"
#include <QDebug>

Light_halo_vfx::Light_halo_vfx(QWidget *parent) : mainwindow(parent) {
    dark_halo_pic.load(":/dataset/halo_vfx/dark_halo_pic.png");
    dark_halo_pic = dark_halo_pic.scaled(90, 90);
    earth_halo_pic.load(":/dataset/halo_vfx/earth_halo_pic.png");
    earth_halo_pic = earth_halo_pic.scaled(90, 90);
    fire_halo_pic.load(":/dataset/halo_vfx/fire_halo_pic.png");
    fire_halo_pic = fire_halo_pic.scaled(90, 90);
    heart_halo_pic.load(":/dataset/halo_vfx/heart_halo_pic.png");
    heart_halo_pic = heart_halo_pic.scaled(90, 90);
    light_halo_pic.load(":/dataset/halo_vfx/light_halo_pic.png");
    light_halo_pic = light_halo_pic.scaled(90, 90);
    water_halo_pic.load(":/dataset/halo_vfx/water_halo_pic.png");
    water_halo_pic = water_halo_pic.scaled(90, 90);
    change_timer = new QTimer;
    connect(change_timer, SIGNAL(timeout()), this, SLOT(change()));
}

void Light_halo_vfx::show(Runestone_pair rp) {
    runestone_pair = rp;
    halos.clear();
    clr = runestone_pair.color;
    for (pair<int,int> pii : runestone_pair.pair) {
        halos.push_back(new QLabel(mainwindow));
        int me = int(halos.size()) - 1;
        halos[me]->setFixedSize(90, 90);
        halos[me]->move(pii.second*90, 510+pii.first*90);
        if (clr == "dark") halos[me]->setPixmap(dark_halo_pic);
        else if (clr == "earth") halos[me]->setPixmap(earth_halo_pic);
        else if (clr == "fire") halos[me]->setPixmap(fire_halo_pic);
        else if (clr == "heart") halos[me]->setPixmap(heart_halo_pic);
        else if (clr == "light") halos[me]->setPixmap(light_halo_pic);
        else if (clr == "water") halos[me]->setPixmap(water_halo_pic);
        halos[me]->setScaledContents(true);
        halos[me]->setAlignment(Qt::AlignCenter);
        halos[me]->show();
    }
    // 開始計時變化
    ms_elapsed = 0;
    change_timer->start(1);
}

void Light_halo_vfx::change() {
    const int changing_unit = 85;
    ms_elapsed++;
    if (ms_elapsed < changing_unit) {
        for (int i = 0; i < int(runestone_pair.pair.size()); i++) {
            double size = 90.0*double(ms_elapsed)/double(changing_unit);
            halos[i]->setFixedSize(size, size);
            halos[i]->move(runestone_pair.pair[i].second*90 + (90-size)/2.0, 510+runestone_pair.pair[i].first*90 + (90-size)/2.0);
        }
    } else if (ms_elapsed < changing_unit*2) {
        for (int i = 0; i < int(runestone_pair.pair.size()); i++) {
            double size = 90.0*double(changing_unit*2-ms_elapsed)/double(changing_unit);
            halos[i]->setFixedSize(size, size);
            halos[i]->move(runestone_pair.pair[i].second*90 + (90-size)/2.0, 510+runestone_pair.pair[i].first*90 + (90-size)/2.0);
        }
    } else {
        change_timer->stop();
        for (auto ptr : halos) {
            ptr->hide();
            delete ptr;
        }
        halos.clear();
    }
}
