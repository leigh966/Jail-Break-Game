#pragma once

#include "sprite.h"

class animatedsprite : public sprite
{
    public:
        animatedsprite();
        animatedsprite(char files[], Vector2 pos, Vector2 size);
        void setFrame(int);
        int getFrame();
        void nextFrame();
    protected:
        int frame;
}