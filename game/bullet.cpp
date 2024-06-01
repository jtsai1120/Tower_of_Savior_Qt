#include "bullet.h"
#include <QPainter>
#include <QTimer>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Bullet::Bullet(QPointF start, QPointF end, QWidget *parent)
    : QWidget(parent), start(start), end(end), progress(0) {

    // 计时器，用于更新绘制进度
    QTimer *updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &Bullet::updateProgress);
    updateTimer->start(30); // 每30ms更新一次绘制进度

    // 计时器，用于隐藏曲线
    QTimer *hideTimer = new QTimer(this);
    hideTimer->setSingleShot(true);
    connect(hideTimer, &QTimer::timeout, this, &Bullet::hideCurve);
    // 计算曲线绘制完成后，0.5秒开始隐藏曲线
    connect(updateTimer, &QTimer::timeout, [this, hideTimer]() {
        if (progress >= 1.0) {
            hideTimer->start(500);
        }
    });
}

void Bullet::paintEvent(QPaintEvent *event) {
    if (progress >= 1.0) return; // 绘制完成后不再重绘

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 根据进度绘制曲线
    painter.setPen(QPen(Qt::black, 2));
    QPainterPath path(start);
    for (qreal t = 0; t <= progress; t += 0.01) {
        qreal x = start.x() + (end.x() - start.x()) * t;
        qreal y = start.y() + (end.y() - start.y()) * std::sin(M_PI * t); // 使用sin函数生成曲线
        path.lineTo(x, y);
    }
    painter.drawPath(path);
}

void Bullet::updateProgress() {
    progress += 0.01;
    if (progress >= 1.0) {
        progress = 1.0;
    }
    update();
}

void Bullet::hideCurve() {
    progress = 1.0;
    update(); // 重绘以隐藏曲线
}
