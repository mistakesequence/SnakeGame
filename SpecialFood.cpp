#include "SpecialFood.h"
#include <cstdlib>
#include <ctime>

SpecialFood::SpecialFood(int fieldWidth, int fieldHeight)
    : Food(fieldWidth, fieldHeight) {
    std::srand(std::time(0));
}

void SpecialFood::Respawn(int fieldWidth, int fieldHeight) {
    x = std::rand() % fieldWidth;
    y = std::rand() % fieldHeight;
}
