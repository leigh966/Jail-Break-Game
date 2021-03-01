#include "prisoner.h"
//#include <Math.h>

prisoner::prisoner()
{
    return;
}

prisoner::prisoner(Vector2 pos)
{
    x = pos.x;
    y = pos.y;
    width = prisoner_WIDTH;
    height = prisoner_HEIGHT;
    normalTexture = LoadTexture(prisoner_FILE);
    flippedTexture = LoadTexture(PRISONER_PNG_FLIPPED);
    setFrame(0);
    rotation = 0;
    speed = BASE_SPEED;
    pause = PRISONER_PAUSE;
}

void prisoner::move()
{
    state = running;
    bool moved = false;
    bool leftPressed = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A), rightPressed = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    bool upPressed = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W), downPressed = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
    if(upPressed && !downPressed)
    {
        transform(0, -speed);
        moved = true;
    }
    if(downPressed && !upPressed)
    {
        transform(0, speed);
        moved = true;
    }
    if(leftPressed && !rightPressed)
    {
        transform(-speed, 0);
        setXFlip(true);
        transform(speed/2*moved, 0);
        moved = true;
    }
    if(rightPressed && !leftPressed)
    {
        transform(speed, 0);
        setXFlip(false);
        transform(-speed/2*moved, 0);
        moved = true;
    }
    if(!moved)
    {
        state = idled;
    }
}

void prisoner::animate()
{
    pause--;
    if(pause <= 0)
    {
        nextFrame();
        if(state == idled && getFrame() >= IDLE_FRAME) setFrame(IDLE_FRAME);
        if (state == running && getFrame() >= RUNNING_END_FRAME && width > 0) setFrame(RUNNING_START_FRAME);
        else if (state == running && getFrame() >= RUNNING_END_FRAME + 1) setFrame(RUNNING_START_FRAME + 1);
        pause = PRISONER_PAUSE;
    }
    //DrawText(FormatText("Frame: %2i", getFrame()), 200, 80, 20, RED); // --Debug--
    draw(false);
}

void prisoner::setState(spriteState s)
{
    state = s;
    if(s == idled)
    {
        setFrame(IDLE_FRAME);
        
    }
    if(s == running)
    {
        setFrame(RUNNING_START_FRAME);
    }
}

bool prisoner::isTouchingWall()
{
    return x < BORDER_SIZE || y < BORDER_SIZE || x > SCREEN_WIDTH - BORDER_SIZE - width || y > (SCREEN_HEIGHT - BORDER_SIZE) - abs(height);
}