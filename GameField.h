#pragma once

#include <QWidget>
#include <QTimer>
#include "Snake.h"
#include "Food.h"
#include "HelpField.h"
#include <vector>
#include <QString>

class GameField : public QWidget {
    Q_OBJECT

public:
    explicit GameField(QWidget* parent = nullptr);
    ~GameField();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void UpdateGame();
    void TogglePause();

private:
    const int TILE_SIZE = 20;
    const int FIELD_WIDTH = 20;
    const int FIELD_HEIGHT = 20;


    QTimer* timer;
    QTimer* gameTimer; 
    int elapsedTime;   
    int currentLevel; 
    const int maxLevel = 6; 
    int applesEaten; 
    int snakeSpeed; 
    bool showLevelText; 
    QTimer* levelTextTimer; 

    Snake snake;
    Food food;
    bool gameOver;

    int score;
    bool isPaused;

    HelpField* helpField;

    std::vector<std::tuple<int, QString, int>> leaderboard; 


    void DrawField(QPainter& painter);
    void DrawSnake(QPainter& painter);
    void DrawFood(QPainter& painter);
    void DrawBorders(QPainter& painter);
    void DrawInfoInBorders(QPainter& painter);

    void LoadLeaderboard();
    void ShowLeaderboard();
    void SaveLeaderboard();
    void UpdateLeaderboard();
};
