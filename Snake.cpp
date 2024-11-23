#include "Snake.h"

Snake::Snake(int startX, int startY)
    : headX(startX), headY(startY), direction(Direction::RIGHT) {}

void Snake::Move() {
    tail.push_back({ headX, headY });

    switch (direction) {
    case Direction::UP:    --headY; break;
    case Direction::DOWN:  ++headY; break;
    case Direction::LEFT:  --headX; break;
    case Direction::RIGHT: ++headX; break;
    }

    if (tail.size() > 1) {
        tail.erase(tail.begin());
    }
}

void Snake::Grow() {
    tail.push_back(tail.back());
}

bool Snake::IsDead() const {
    for (const auto& segment : tail) {
        if (segment.first == headX && segment.second == headY) {
            return true;  
        }
    }
    return false;  
}

void Snake::SetDirection(Direction dir) {
    direction = dir;
}

void Snake::SetPosition(int x, int y) {
    headX = x;
    headY = y;
}

int Snake::GetX() const {
    return headX;
}

int Snake::GetY() const {
    return headY;
}

const std::vector<std::pair<int, int>>& Snake::GetTail() const {
    return tail;
}

Direction Snake::GetDirection() const {
    return direction;
}
