#ifndef BULLET_H
#define BULLET_H

#include <QWidget>
#include <QPointF>

class Bullet : public QWidget {
    Q_OBJECT

public:
    Bullet(QPointF start, QPointF end, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateProgress();
    void hideCurve();

private:
    QPointF start;
    QPointF end;
    qreal progress; // 记录绘制进度
};

#endif // BULLET_H
