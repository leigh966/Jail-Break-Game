#pragma once

#include "animatedsprite.h"

enum spriteState
{
    idled, running
};

class prisoner : public animatedsprite
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