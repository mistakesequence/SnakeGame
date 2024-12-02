#pragma once
#include "Food.h"

class SpecialFood : public Food {
public:
    SpecialFood(int fieldWidth, int fieldHeight);
    void Respawn(int fieldWidth, int fieldHeight);
};
