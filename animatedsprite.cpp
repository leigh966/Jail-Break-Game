#include "animatedsprite.h"

animatedsprite::animatedsprite()
: sprite()
{

}

animatedsprite::animatedsprite(char files[], Vector2 pos, Vector2 size)
: sprite(files, pos, size)
{
    frame = 0;
}

void animatedsprite::setFrame(int num)
{
    frame = num;
}

int animatedsprite::getFrame()
{
  return frame;
}

void animatedsprite::nextFrame()
{
    frame++;
}

void animatedsprite::draw(bool allowedOutOfBounds)
{
    if(!allowedOutOfBounds)
    {
        touchingWall = false;
        if(x < BORDER_SIZE){ x = BORDER_SIZE; touchingWall = true; }
        if(x > SCREEN_WIDTH - BORDER_SIZE - width){ x = SCREEN_WIDTH - BORDER_SIZE - width; touchingWall = true; }
        if(y < BORDER_SIZE){ y = BORDER_SIZE;  touchingWall = true; }
        if(y > SCREEN_HEIGHT - BORDER_SIZE - height){ y = SCREEN_HEIGHT - BORDER_SIZE - height; touchingWall = true; }
    }
    if (!flipped)
    {
        DrawTextureRec(normalTexture,
            Rectangle{ (float)(frame * width), 0, (float)(width), (float)(height) },
            Vector2{ (float)(x), (float)(y) },
            tint);
    }
    else
    {
        DrawTextureRec(flippedTexture,
            Rectangle{ (float)(frame * width), 0, (float)(width), (float)(height) },
            Vector2{ (float)(x), (float)(y) },
            tint);
    }
}