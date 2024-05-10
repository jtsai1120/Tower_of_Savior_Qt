#ifndef EVENTFILTER_H
#define EVENTFILTER_H

#include <QObject>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>

class EventFilter : public QObject {
    Q_OBJECT
public:
    explicit EventFilter(QObject *parent = nullptr) : QObject(parent) {}

protected:
    bool eventFilter(QObject *watched, QEvent *event) override {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            qDebug() << "Mouse release event in MainWindow:"
                     << "Position:" << mouseEvent->pos()
                     << "Button:" << mouseEvent->button();
            return true;
        }
        return QObject::eventFilter(watched, event);
    }
};

#endif // EVENTFILTER_H
