#pragma once

class Food {
public:
    Food(int fieldWidth, int fieldHeight);
    void Respawn(int fieldWidth, int fieldHeight);
    int GetX() const;
    int GetY() const;

private:
    int x, y;
};
