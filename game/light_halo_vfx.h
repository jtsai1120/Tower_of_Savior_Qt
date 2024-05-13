#ifndef LIGHT_HALO_VFX_H
#define LIGHT_HALO_VFX_H

#include "runestone.h"
#include "runestone_pair.h"
#include <QPixmap>
#include <vector>
#include <QTimer>
using namespace std;

class Light_halo_vfx : public QObject {
    Q_OBJECT
public:
    Light_halo_vfx(QWidget *parent);
    void show(Runestone_pair rp);
private slots:
    void change();

private:
    QPixmap dark_halo_pic;
    QPixmap earth_halo_pic;
    QPixmap fire_halo_pic;
    QPixmap heart_halo_pic;
    QPixmap light_halo_pic;
    QPixmap water_halo_pic;

    QString clr;

    vector<QLabel*> halos;
    QTimer *change_timer;
    int ms_elapsed;
    Runestone_pair runestone_pair;

    QWidget *mainwindow;
};

#endif // LIGHT_HALO_VFX_H
