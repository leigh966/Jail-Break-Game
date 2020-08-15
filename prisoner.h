#pragma once

enum spriteState
{
    idled, running
};

class prisoner : public sprite
{
public:
    prisoner();
    prisoner(Vector2 pos);
    void move();
    void animate();
    void setState(spriteState s);
    bool isTouchingWall();
protected:
    int speed;
    spriteState state;
    int pause;
};

#include "prisoner.cpp"