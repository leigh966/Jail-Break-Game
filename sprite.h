#pragma once
#include "raylib.h"

class sprite
{
public:
    sprite(char files[], Vector2 pos, Vector2 size);
    void draw(bool allowedOutOfBounds);
    void setFrame(int);
    int getFrame();
    void nextFrame();
    void setPosition(int xPos, int yPos);
    void transform(int deltaX, int deltaY);
    void rotate(int degrees);
    void setRotation(int);
    void flipX();
    void setXFlip(bool);
    Vector2 getSize();
    Rectangle getCollider();
    int getX();
    int getY();
    void setPos(Vector2);
    sprite();
    void setTint(Color);
    void setFlippedTexture(char file[]);
protected:
    bool flipped;
    Texture2D normalTexture, flippedTexture;
    int frame, x, y, width, height, rotation;
    int pause;
    bool touchingWall;
    Color tint;
    
};

#include "sprite.cpp"