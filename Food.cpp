#include "Food.h"
#include <cstdlib>  

Food::Food(int fieldWidth, int fieldHeight)
{
    Respawn(fieldWidth, fieldHeight);
}

void Food::Respawn(int fieldWidth, int fieldHeight)
{
    x = rand() % fieldWidth;
    y = rand() % fieldHeight;
}

int Food::GetX() const
{
    return x;
}

int Food::GetY() const
{
    return y;
}
