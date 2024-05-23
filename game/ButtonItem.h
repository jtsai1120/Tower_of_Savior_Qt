#ifndef BUTTONITEM_H
#define BUTTONITEM_H

#include<QPushButton>
#include<QGraphicsProxyWidget>
#include<QGraphicsScene>

class ButtonItem : public QGraphicsProxyWidget {
public:
    ButtonItem(QPushButton *button, QGraphicsItem *parent = nullptr) : QGraphicsProxyWidget(parent) {
        setWidget(button);
    }
};

#endif // BUTTONITEM_H
