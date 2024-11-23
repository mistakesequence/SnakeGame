#pragma once
#include <vector>

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Snake {
public:
    Snake(int startX, int startY);
    void Move();
    void Grow();
    bool IsDead() const;
    void SetDirection(Direction dir);
    void SetPosition(int x, int y);  
    int GetX() const;
    int GetY() const;
    const std::vector<std::pair<int, int>>& GetTail() const;
    Direction GetDirection() const;

private:
    int headX, headY;
    Direction direction;
    std::vector<std::pair<int, int>> tail;
};
