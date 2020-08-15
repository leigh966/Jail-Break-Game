

guard::guard()
{

}

void guard::ready()
{
    start(0, 0);
    width = ENEMY_WIDTH;
    height = ENEMY_HEIGHT;
    normalTexture = LoadTexture(ENEMY_FILE);
    flippedTexture = LoadTexture(COP_PNG_FLIPPED);
    frame = 0;
    generateDirection();
    pause = ENEMY_PAUSE;
    alert = sprite{ "resources/newAlert.png", Vector2{(float)(x), (float)(y)}, ALERT_SIZE };
}

void guard::start(int playerX, int playerY)
{
    bool inRangeX= true;
    bool inRangey = true;
    spotted = false;
    
    while(inRangeX && inRangey)
    {
        x = rand() % (SCREEN_WIDTH - BORDER_SIZE - ENEMY_WIDTH) + BORDER_SIZE;
        y = rand() % (SCREEN_HEIGHT - BORDER_SIZE - ENEMY_HEIGHT) + BORDER_SIZE;
        int xDif = abs(playerX + prisoner_WIDTH/2 - x);
        int yDif = abs(playerY + prisoner_HEIGHT/2 - y);
        inRangeX = xDif < MIN_SPAWN_DISTANCE_X;
        inRangey = yDif < MIN_SPAWN_DISTANCE_Y;
    }
    
    if(inRangeX) x = SCREEN_WIDTH - x;
    if(inRangey) y = SCREEN_HEIGHT - y;
}

void guard::move(int playerX, int playerY)
{
    pause--;
    if(moveFrames <= 0)
    {
        generateDirection();
    }
    moveFrames--;
    if(!(abs(direction.x) || abs(direction.y)))
    {
        frame = IDLE_FRAME;
    }
    else
    {
        if(pause <= 0)
        {
            frame++;
            if(frame >= RUNNING_END_FRAME)
            {
                frame = RUNNING_START_FRAME;
            }
            pause = ENEMY_PAUSE;
            if(spotted) pause = 3 * pause / 4;                
        }
    }
    if(spotted)
    {
        direction.x = playerX - x;
        direction.y = playerY - y;
        normalise(direction);
        scale(direction, ENEMY_RUN_SPEED);
        alert.setPos(Vector2{ (float)(x + (width/7)), (float)(y + ALERT_OFFSET.y)});
        alert.draw(true);
    }
    transform(direction.x, direction.y);
    if(direction.x < 0)
    {
        setXFlip(true);
    }
    if(direction.x > 0)
    {
        setXFlip(false);
    }
    if(touchingWall)
    {
        reverse();
        transform(direction.x, direction.y);
    }
}

void guard::generateDirection()
{
    moveFrames = rand() % (ENEMY_MAX_MOVE_FRAMES - ENEMY_MIN_MOVE_FRAMES) + ENEMY_MIN_MOVE_FRAMES;
        int xDir = rand() % 3 - 1;
        int yDir = rand() % 3 - 1;
        direction = Vector2{float(xDir), float(yDir)};
}

void guard::reverse()
{
    direction.x *= -1;
    direction.y *= -1;
}

void guard::checkSpotted(Rectangle playerCollider)
{
    Rectangle spottedCollider = getCollider();
    spottedCollider.width = 2*ENEMY_VIEW;
    spottedCollider.height = ENEMY_VIEW;
    spottedCollider.y = y;
    if(flipped)
    {
        spottedCollider.x = x - spottedCollider.width;
    }
    else
    {
        spottedCollider.x = x + width;
    }
    if(!spotted)
    {
        spotted = CheckCollisionRecs(playerCollider, spottedCollider);
    }
    
    //DrawRectangleRec(spottedCollider, RED);// --Debug--
}

void guard::setSpotted(bool s)
{
    spotted = s;
}