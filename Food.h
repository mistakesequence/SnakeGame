// Food.h
#pragma once

class Food {
public:
    Food(int fieldWidth, int fieldHeight); 
    virtual void Respawn(int fieldWidth, int fieldHeight); 
    int GetX() const;  
    int GetY() const;  

protected:
    int x, y;  
};
