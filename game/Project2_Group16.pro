QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bg.cpp \
    charac_slot.cpp \
    combo_counter.cpp \
    enemy.cpp \
    enemy_hp.cpp \
    icon_bar.cpp \
    light_halo_vfx.cpp \
    main.cpp \
    mainwindow.cpp \
    runestone.cpp

HEADERS += \
    ButtonItem.h \
    bg.h \
    charac_slot.h \
    combo_counter.h \
    enemy.h \
    enemy_hp.h \
    icon_bar.h \
    light_halo_vfx.h \
    mainwindow.h \
    runestone.h \
    runestone_pair.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data.qrc
