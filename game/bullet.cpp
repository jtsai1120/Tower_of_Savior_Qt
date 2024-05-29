#include "bullet.h"
#include <QPainter>
#include <QtMath>

Bullet::Bullet(const QPoint &start, const QPoint &end, QWidget *parent)
    : QWidget(parent), currentSpanAngle(0), showArc(true), startPoint(start), endPoint(end) {
    setFixedSize(400, 400); // 你可以根据需要调整窗口大小

    // 计算绘制弧线的矩形框和起始角度
    qreal centerX = (start.x() + end.x()) / 2.0;
    qreal centerY = (start.y() + end.y()) / 2.0;
    qreal radius = qSqrt(qPow(end.x() - start.x(), 2) + qPow(end.y() - start.y(), 2)) / 2.0;

    arcRect = QRectF(centerX - radius, centerY - radius, radius * 2, radius * 2);
    startAngle = qRadiansToDegrees(qAtan2(start.y() - centerY, start.x() - centerX)) * 16;

    // 设置定时器，每100ms更新一次弧线
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Bullet::updateArc);
    timer->start(100); // 每100ms触发一次
}

void Bullet::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    if (showArc) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(Qt::white, 2));

        // 绘制弧线
        painter.drawArc(arcRect, startAngle, currentSpanAngle);
    }
}

void Bullet::updateArc() {
    currentSpanAngle += 16; // 每次增加16个单位（相当于1度）

    if (currentSpanAngle >= 120 * 16) { // 如果弧线已经绘制完成
        timer->stop(); // 停止定时器
        showArc = false; // 隐藏弧线
    }

    update(); // 触发重绘
}
