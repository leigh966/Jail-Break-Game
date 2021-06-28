#pragma once

#include "animatedsprite.h"

class guard : public animatedsprite
{
public:
    guard();
    void ready();
    void move(int,int);
    void reverse();
    void checkSpotted(Rectangle);
    void start(int, int);
    void setSpotted(bool);
protected:
    int moveFrames;
    Vector2 direction;
    void generateDirection();
    int pause;
    bool spotted;
    sprite alert;
};

