#pragma once

#include <QWidget>
#include <QPainter>

class HelpField : public QWidget {
    Q_OBJECT

public:
    explicit HelpField(QWidget* parent = nullptr);
    void setScore(int score);
    void setPauseStatus(bool isPaused);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int score;
    bool isPaused;
};