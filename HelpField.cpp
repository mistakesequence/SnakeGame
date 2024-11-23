#include "HelpField.h"
#include <QPainter>
#include <QFont>

HelpField::HelpField(QWidget* parent)
    : QWidget(parent), score(0), isPaused(false) {
    setFixedHeight(50);  
}

void HelpField::setScore(int newScore) {
    score = newScore;
    update();
}

void HelpField::setPauseStatus(bool pauseStatus) {
    isPaused = pauseStatus;
    update();
}

void HelpField::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::darkBlue));  
    painter.setFont(QFont("Arial", 10, QFont::Bold));  
    painter.drawText(10, 20, QString("Score: %1").arg(score));


    if (isPaused) {
        painter.setBrush(QBrush(QColor(233, 218, 3)));
        painter.setFont(QFont("Arial", 15, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "Paused");
        return;
    }
}