#include "guard.h"
//#include <Math.h>

guard::guard()
{

}

// Prepare the guard -- Run at the start of the level
void guard::ready()
{
    start(0, 0);
    width = ENEMY_WIDTH;
    height = ENEMY_HEIGHT;
    normalTexture = LoadTexture(ENEMY_FILE);
    flippedTexture = LoadTexture(COP_PNG_FLIPPED);
    setFrame(0);
    generateDirection();
    pause = ENEMY_PAUSE;
    alert = sprite{ "resources/alert.png", Vector2{(float)(x), (float)(y)}, ALERT_SIZE };
}


// Pick a random starting position for the guard away from the player
void guard::start(int playerX, int playerY)
{
    bool inRangeX = true;
    bool inRangey = true;
    spotted = false;
    
    // If we're too close to the player, generate a new random position
    while(inRangeX && inRangey)
    {
        // Generate random position
        x = rand() % (SCREEN_WIDTH - BORDER_SIZE - ENEMY_WIDTH) + BORDER_SIZE;
        y = rand() % (SCREEN_HEIGHT - BORDER_SIZE - ENEMY_HEIGHT) + BORDER_SIZE;
        
        // Calculate distance from player on each axis
        int xDif = abs(playerX + prisoner_WIDTH/2 - x);
        int yDif = abs(playerY + prisoner_HEIGHT/2 - y);
        
        //Work out if our x and y positions are far enough from the player
        inRangeX = xDif < MIN_SPAWN_DISTANCE_X;
        inRangey = yDif < MIN_SPAWN_DISTANCE_Y;
    }
    
    // If we're too close the player on one axis but not another, flip to the other end of that axis (This creates a grouping effect of guards away from the player)
    if(inRangeX) x = SCREEN_WIDTH - x;
    if(inRangey) y = SCREEN_HEIGHT - y;
}


// Move -- called every frame
void guard::move(int playerX, int playerY)
{
    // Count down to the next frame in the animation
    pause--;
    
    // If we have finished the last random action, generate another
    if(moveFrames <= 0)
    {
        generateDirection();
    }
    
    // Count down to the end of this action 
    moveFrames--;
    
    // If random action is to stand still for a while
    if(!(abs(direction.x) || abs(direction.y))) 
    {
        setFrame(IDLE_FRAME);
    }
    else
    {
        // Update animation if it is time
        if(pause <= 0)
        {
            nextFrame();
            // If we have reached the end of the run animation, restart it
            if(getFrame() >= RUNNING_END_FRAME)
            {
                setFrame(RUNNING_START_FRAME);
            }
            // Reset the frame pause counter
            pause = ENEMY_PAUSE;
            
            // Reduce pause if chasing player (Making animation faster)
            if(spotted) pause = 3 * pause / 4;                
        }
    }
    
    // Change direction to follow player if we've spotted them
    if(spotted)
    {
        direction.x = playerX - x;
        direction.y = playerY - y;
        normalise(direction);
        scale(direction, ENEMY_RUN_SPEED);
        alert.setPos(Vector2{ (float)(x + (width/7)), (float)(y + ALERT_OFFSET.y)});
        alert.draw(true);
    }
    
    //Move in the determined direction
    transform(direction.x, direction.y);
    
    // Face in the direction of movement
    if(direction.x != 0) //Don't change facing direction if only moving on y axis
    {
        setXFlip(direction.x < 0); //1=right, -1=left
    }

    // Move away from the wall if we hit it
    if(touchingWall)
    {
        reverse();
        transform(direction.x, direction.y);
    }
}


// Generate a 2d direction vector to follow and a number of frames to do so
void guard::generateDirection()
{
    moveFrames = rand() % (ENEMY_MAX_MOVE_FRAMES - ENEMY_MIN_MOVE_FRAMES) + ENEMY_MIN_MOVE_FRAMES;
        int xDir = rand() % 3 - 1;
        int yDir = rand() % 3 - 1;
        direction = Vector2{float(xDir), float(yDir)};
}

// Reverse the current direction
void guard::reverse()
{
    direction.x *= -1;
    direction.y *= -1;
}

// If we can see the player, set spotted = true
void guard::checkSpotted(Rectangle playerCollider)
{
    // We don't need to check if we can see the player if they have already been spotted
    if(spotted)
    {
        return;
    }
    
    Rectangle spottedCollider = getCollider();
    spottedCollider.width = 2*ENEMY_VIEW;
    spottedCollider.height = ENEMY_VIEW;
    spottedCollider.y = y;
    
    // Move the spotted collider to the side that the guard is facing
    if(flipped)
    {
        spottedCollider.x = x - spottedCollider.width;
    }
    else
    {
        spottedCollider.x = x + width;
    }
    

    spotted = CheckCollisionRecs(playerCollider, spottedCollider);
    
    
    //DrawRectangleRec(spottedCollider, RED);// --Debug--
}

// Alows external entities to effect whether a guard is pursuing the player (example: pause set spotted = false)
void guard::setSpotted(bool s)
{
    spotted = s;
}