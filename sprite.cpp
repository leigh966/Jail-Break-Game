#include "sprite.h"

sprite::sprite(char files[], Vector2 pos, Vector2 size)
{
    x = pos.x;
    y = pos.y;
    width = size.x;
    height = size.y;
    normalTexture = LoadTexture(files);
    flippedTexture = LoadTexture(files);
    frame = 0;
    rotation = 0;
    tint = RAYWHITE;
}

void sprite::setFlippedTexture(char file[])
{
    flippedTexture = LoadTexture(file);
}

void sprite::draw(bool allowedOutOfBounds)
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

void sprite::setFrame(int num)
{
    frame = num;
}

void sprite::nextFrame()
{
    frame++;
}

void sprite::setPosition(int xPos, int yPos)
{
    x = xPos;
    y = yPos;
}

void sprite::transform(int deltaX, int deltaY)
{
    x += deltaX;
    y += deltaY;
}

void sprite::rotate(int degrees)
{
    rotation += degrees;
}

void sprite::setRotation(int degrees)
{
    rotation = degrees;
}

int sprite::getFrame()
{
  return frame;  
}

sprite::sprite()
{
    tint = RAYWHITE;
}

void sprite::flipX()
{
    flipped = !flipped;
}

void sprite::setXFlip(bool newFlipped)
{
    flipped = newFlipped;
}
Vector2 sprite::getSize()
{
    return Vector2{ (float)(width), (float)(height)};
}

Rectangle sprite::getCollider()
{
    return Rectangle{float(x), float(y + abs(height/4)), (float)(abs(width)), (float)(abs(height/2))};
}

int sprite::getX()
{
    return x;
}

int sprite::getY()
{
    return y;
}

void sprite::setPos(Vector2 pos)
{
    x = (int)(pos.x);
    y = (int)(pos.y);
}

void sprite::setTint(Color c)
{
    tint = c;
}

void sprite::normalise(Vector2 &vector)
{      
    double x = vector.x, y = vector.y;
    double max;
    if(abs(x) > abs(y))
    {
        max = x;
    }
    else
    {
        max = y;
    }
    vector.x = round(x/abs(max));
    vector.y = round(y/abs(max));
    //DrawText(FormatText("X: %02.02f", vector.x), 200, 80, 20, RED); --Debug--
    //DrawText(FormatText("Y: %02.02f", vector.y), 200, 100, 20, RED);
    
}

void sprite::scale(Vector2 &vector, int scale)
{
    vector.x *= scale;
    vector.y *= scale;
}