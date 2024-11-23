#include "GameField.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QMessageBox>
#include <QInputDialog>
#include <fstream>
#include <algorithm>

GameField::GameField(QWidget* parent)
    : QWidget(parent), snake(10, 10), food(FIELD_WIDTH, FIELD_HEIGHT), gameOver(false), score(0),
    isPaused(false), elapsedTime(0), currentLevel(1), applesEaten(0), snakeSpeed(140), showLevelText(false) {

    setFixedSize(FIELD_WIDTH * TILE_SIZE, FIELD_HEIGHT * TILE_SIZE + 50);

    helpField = new HelpField(this);
    helpField->setGeometry(0, FIELD_HEIGHT * TILE_SIZE, FIELD_WIDTH * TILE_SIZE, 50);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameField::UpdateGame);
    timer->start(snakeSpeed);

    levelTextTimer = new QTimer(this);
    levelTextTimer->setSingleShot(true);
    connect(levelTextTimer, &QTimer::timeout, [this]() {
        showLevelText = false;
        update();
        });

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, [this]() {
        if (!isPaused && !gameOver) {
            elapsedTime++;
            update();
        }
        });
    gameTimer->start(1000);

    LoadLeaderboard();
}


GameField::~GameField() {
    SaveLeaderboard();
}

void GameField::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    if (gameOver) {
        painter.setPen(Qt::red);
        painter.setFont(QFont("Arial", 15, QFont::Bold));

        painter.drawText(rect(), Qt::AlignCenter, "Game Over! Press R to Restart\nPress L to view Leaderboard");
        return;
    }

    DrawBorders(painter);
    DrawInfoInBorders(painter);

    DrawField(painter);
    DrawSnake(painter);
    DrawFood(painter);

    helpField->setScore(score);
    helpField->setPauseStatus(isPaused);

    painter.setPen(Qt::darkBlue);
    painter.setFont(QFont("Arial", 10, QFont::Bold));

    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;

    QString timeString = QString::asprintf("%02d:%02d", minutes, seconds);

    painter.drawText(10, FIELD_HEIGHT * TILE_SIZE + 40, QString("Time: %1").arg(timeString));

    if (showLevelText) {
        painter.setPen(Qt::yellow);
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        QString levelText = (currentLevel == maxLevel) ? "Final Level" : QString("Level %1").arg(currentLevel);
        painter.drawText(rect(), Qt::AlignCenter, levelText);
    }
}


void GameField::keyPressEvent(QKeyEvent* event) {
    if (gameOver) {
        if (event->key() == Qt::Key_R) {
            snake = Snake(10, 10);
            food.Respawn(FIELD_WIDTH, FIELD_HEIGHT);
            gameOver = false;
            isPaused = false;
            elapsedTime = 0;
            score = 0;
            currentLevel = 1;
            applesEaten = 0;
            snakeSpeed = 140;
            timer->start(snakeSpeed);
            update();
        }
        else if (event->key() == Qt::Key_L) {
            ShowLeaderboard(); 
        }
        return;
    }

    if (event->key() == Qt::Key_Up && snake.GetDirection() != Direction::DOWN) {
        snake.SetDirection(Direction::UP);
    }
    if (event->key() == Qt::Key_Down && snake.GetDirection() != Direction::UP) {
        snake.SetDirection(Direction::DOWN);
    }
    if (event->key() == Qt::Key_Left && snake.GetDirection() != Direction::RIGHT) {
        snake.SetDirection(Direction::LEFT);
    }
    if (event->key() == Qt::Key_Right && snake.GetDirection() != Direction::LEFT) {
        snake.SetDirection(Direction::RIGHT);
    }

    if (event->key() == Qt::Key_P) {
        TogglePause();
    }
}

void GameField::TogglePause() {
    isPaused = !isPaused;
    if (isPaused) {
        timer->stop();
    }
    else {
        timer->start(snakeSpeed); 
    }
    update();
}

void GameField::UpdateGame() {
    if (gameOver || isPaused) {
        return;
    }

    snake.Move();

    if (snake.GetX() == food.GetX() && snake.GetY() == food.GetY()) {
        snake.Grow();
        score++;
        applesEaten++;
        food.Respawn(FIELD_WIDTH, FIELD_HEIGHT);

        if (applesEaten % 5 == 0 && currentLevel < maxLevel) {
            currentLevel++;
            snakeSpeed -= 10; 
            timer->start(snakeSpeed);
            showLevelText = true;

            levelTextTimer->start(2000); 
        }
    }

    if (snake.GetX() < 0 || snake.GetX() >= FIELD_WIDTH || snake.GetY() < 0 || snake.GetY() >= FIELD_HEIGHT) {
        if (snake.GetX() < 0) {
            snake.SetPosition(FIELD_WIDTH - 1, snake.GetY());
        }
        else if (snake.GetX() >= FIELD_WIDTH) {
            snake.SetPosition(0, snake.GetY());
        }
        if (snake.GetY() < 0) {
            snake.SetPosition(snake.GetX(), FIELD_HEIGHT - 1);
        }
        else if (snake.GetY() >= FIELD_HEIGHT) {
            snake.SetPosition(snake.GetX(), 0);
        }
    }

    if (snake.IsDead()) {
        gameOver = true;
        UpdateLeaderboard();
        score = 0;
    }

    update();
}


void GameField::DrawField(QPainter& painter) {
    painter.setBrush(Qt::gray);
    painter.drawRect(0, 0, FIELD_WIDTH * TILE_SIZE, FIELD_HEIGHT * TILE_SIZE);
}

void GameField::DrawSnake(QPainter& painter) {
    painter.setBrush(QBrush(QColor(101, 198, 45)));
    painter.drawEllipse(snake.GetX() * TILE_SIZE, snake.GetY() * TILE_SIZE, TILE_SIZE, TILE_SIZE);

    painter.setBrush(QBrush(QColor(101, 198, 45)));
    for (const auto& segment : snake.GetTail()) {
        painter.drawEllipse(segment.first * TILE_SIZE, segment.second * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    }
}

void GameField::DrawFood(QPainter& painter) {
    painter.setBrush(QBrush(QColor(250, 19, 19)));
    painter.drawEllipse(food.GetX() * TILE_SIZE, food.GetY() * TILE_SIZE, TILE_SIZE, TILE_SIZE);
}

void GameField::DrawBorders(QPainter& painter) {
    painter.setBrush(QBrush(Qt::darkGray));
    painter.drawRect(0, 0, FIELD_WIDTH * TILE_SIZE, 50);
    painter.drawRect(0, FIELD_HEIGHT * TILE_SIZE + 50, FIELD_WIDTH * TILE_SIZE, 50);
}

void GameField::DrawInfoInBorders(QPainter& painter) {
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(10, 20, QString("Score: %1").arg(score));

    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;
    QString timeString = QString::asprintf("%02d:%02d", minutes, seconds);
    painter.drawText(10, 40, QString("Time: %1").arg(timeString));
    painter.drawText(10, 60, QString("Speed: %1 ms").arg(snakeSpeed));

}



void GameField::ShowLeaderboard() {
    QString leaderboardText;
    for (const auto& record : leaderboard) {
        int score = std::get<0>(record);
        QString name = std::get<1>(record);
        int duration = std::get<2>(record);

        int minutes = duration / 60;
        int seconds = duration % 60;
        QString durationStr = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));

        leaderboardText += QString("Name: %1\nScore: %2\nDuration: %3\n\n")
            .arg(name)
            .arg(score)
            .arg(durationStr);
    }

    if (leaderboardText.isEmpty()) {
        leaderboardText = "No leaderboard data available.";
    }

    QMessageBox::information(this, "Leaderboard", leaderboardText);
}


void GameField::LoadLeaderboard() {
    std::ifstream file("leaderboard.txt");
    leaderboard.clear();

    if (file.is_open()) {
        std::string line, name;
        int score = 0, duration = 0;

        while (std::getline(file, line)) {
            if (line.find("Score:") == 0) {
                score = std::stoi(line.substr(6));
            }
            else if (line.find("Name:") == 0) {
                name = line.substr(6);
            }
            else if (line.find("Duration Time:") == 0) {
                std::string timeStr = line.substr(15); 
                int minutes = std::stoi(timeStr.substr(0, timeStr.find(":")));
                int seconds = std::stoi(timeStr.substr(timeStr.find(":") + 1));
                duration = minutes * 60 + seconds;
            }
            else if (line.empty()) {
                leaderboard.emplace_back(score, QString::fromStdString(name), duration);
            }
        }
    }
}



void GameField::SaveLeaderboard() {
    std::ofstream file("leaderboard.txt");
    if (file.is_open()) {
        for (const auto& record : leaderboard) {
            int score = std::get<0>(record);
            QString name = std::get<1>(record);
            int duration = std::get<2>(record);

            int minutes = duration / 60;
            int seconds = duration % 60;
            QString durationStr = QString("%1:%2")
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0'));

            file << "Score: " << score << "\n"
                << "Name: " << name.toStdString() << "\n"
                << "Duration Time: " << durationStr.toStdString() << "\n\n";
        }
    }
}




void GameField::UpdateLeaderboard() {
    QString name;

    while (name.isEmpty()) {
        name = QInputDialog::getText(this, "Game Over", "Enter your name:");

        if (name.isEmpty() && QMessageBox::question(this, "No Name Entered", "Do you want to skip saving your score?") == QMessageBox::Yes) {
            return;
        }
    }

    leaderboard.emplace_back(score, name, elapsedTime);
    std::sort(leaderboard.begin(), leaderboard.end(), [](const auto& a, const auto& b) {
        return std::get<0>(a) > std::get<0>(b);
        });

    if (leaderboard.size() > 5) {
        leaderboard.resize(5);
    }

    SaveLeaderboard();
}




