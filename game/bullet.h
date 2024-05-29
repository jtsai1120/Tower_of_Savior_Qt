#ifndef BULLET_H
#define BULLET_H

#include <QWidget>
#include <QTimer>
#include <QPoint>

class Bullet : public QWidget {
    Q_OBJECT

public:
    Bullet(const QPoint &start, const QPoint &end, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateArc();

private:
    int currentSpanAngle;
    bool showArc;
    QTimer *timer;
    QPoint startPoint;
    QPoint endPoint;
    QRectF arcRect;
    int startAngle;
};

#endif // BULLET_H
