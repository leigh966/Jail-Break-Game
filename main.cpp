#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "constants.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "prisoner.h"
#include "guard.h"

prisoner player;
sprite BLUE_DOOR, GREEN_DOOR, exitDoor, ground;

int frame = 0;
int knockbackFrame = 0;

Vector2 toGuard;
Rectangle playerCollider, guardCollider;
Vector2 playerTrajectory;
bool touchedWall;
int roundNo = 1;
guard enemies[100];

sprite healthBar[3];
Vector2 hpPos = HEALTH_BAR_POS;
int health = START_HEALTH;

int lastHitIndex = 0;

int score = 0;

Rectangle exitCollider;
bool locked = false, started = false;

sprite key, lock;
animatedsprite play, eKey, continueButton;

int eBreak = 0;

int pauseFrames = MAX_PAUSE_FRAMES;

/*
void normalise(Vector2 &vector)
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

void scale(Vector2 &vector, int scale)
{
    vector.x *= scale;
    vector.y *= scale;
}
*/

Vector2 generateRandomPos()
{
    return Vector2{(float)(rand() % (SCREEN_WIDTH - BORDER_SIZE  * 4) + BORDER_SIZE*2), (float)(rand() % (SCREEN_HEIGHT - BORDER_SIZE * 4) + BORDER_SIZE*2)};
}

void drawGround(sprite &ground, int roundNo)
{
    for(int x = 0; x < SCREEN_WIDTH; x += 300)
    {
        for(int y = 0; y < SCREEN_HEIGHT; y += 300)
        {
            ground.setPos(Vector2{float(x), float(y)});
            ground.draw(true);
        }       
    }
}

int getNumGuards(int round)
{
    if(round < 6)
    {
       return round - 1; 
    }
    if(round < 11)
    {
        return round - 2;
    }
    if(round < 21)
    {
        return round - 3;
    }
    return round - 4;
}

void startRound(int roundNo, prisoner &player, guard enemies[100], sprite &exit)
{
    Vector2 entrancePos = generateRandomPos();
    player.setPos(entrancePos);
    
    Vector2 exitPos = entrancePos;
    while(abs(exitPos.x - entrancePos.x) < SCREEN_WIDTH/3)
    {
        exitPos = generateRandomPos();
        exit.setPos(exitPos);
    }
    
    int numGuards = getNumGuards(roundNo);
    for(int index = 0; index < numGuards; index++)
    {
        enemies[index].start(player.getX(), player.getY());
    }
}

void offerInteract(animatedsprite &eKey, Vector2 playerPos, int &eBreak)
{
    eBreak--;
    if(eBreak < 1)
    {
        eBreak = E_ANIMATION_BREAK;
        eKey.setFrame(!eKey.getFrame());
    }
    playerPos.x += E_OFFSET.x;
    playerPos.y += E_OFFSET.y;
    eKey.setPos(playerPos);
    eKey.draw(true);
}

void startMenu()
{
    DrawText("Jail", 300, 50, 70, RED);
    DrawText("Break", 400, 110, 70, RED);
    if (CheckCollisionPointRec(GetMousePosition(), play.getCollider()))
    {
        play.setFrame(0);
        if (IsMouseButtonPressed(0)) started = true;
    }
    else
    {
        play.setFrame(1);
    }
    play.draw(false);
}

void giveHint()
{
    if (roundNo == 1) DrawText("Head for the door", HINT_TEXT_POS.x, HINT_TEXT_POS.y, HINT_TEXT_SIZE, HINT_COLOR);
    if (roundNo == 2) DrawText("Watch out for the guard!", HINT_TEXT_POS.x, HINT_TEXT_POS.y, HINT_TEXT_SIZE, HINT_COLOR);
    if (roundNo == 3) DrawText("The guards are fast, try not to let\n them see you", HINT_TEXT_POS.x, HINT_TEXT_POS.y, HINT_TEXT_SIZE, HINT_COLOR);
    if (roundNo == 4) DrawText("You can freeze time by holding P", HINT_TEXT_POS.x, HINT_TEXT_POS.y, HINT_TEXT_SIZE, HINT_COLOR);
    if (roundNo == 5) DrawText("Be sparing with your time freeze; it runs out\n fast", HINT_TEXT_POS.x, HINT_TEXT_POS.y, HINT_TEXT_SIZE, HINT_COLOR);
    if (roundNo == 6) DrawText("You see the key... you can guess what to do", HINT_TEXT_POS.x, HINT_TEXT_POS.y, HINT_TEXT_SIZE, HINT_COLOR);
}

void hurtPlayer(int enemyIndex)
{
    enemies[enemyIndex].setSpotted(true);
    health--;
    player.setFrame(1);
    knockbackFrame = KNOCK_BACK_FRAMES;
    playerTrajectory = toGuard;
    touchedWall = false;
    lastHitIndex = enemyIndex;
    enemies[enemyIndex].move(player.getX(), player.getY());
}

void moveGuards()
{
    for (int enemyIndex = 0; enemyIndex < getNumGuards(roundNo); enemyIndex++)
    {
        enemies[enemyIndex].move(player.getX(), player.getY());
        guardCollider = enemies[enemyIndex].getCollider();
        toGuard = Vector2{ (float)(enemies[enemyIndex].getX() - player.getX()), (float)(enemies[enemyIndex].getY() - player.getY()) };
        enemies[enemyIndex].checkSpotted(playerCollider);
        bool touchingGuard = CheckCollisionRecs(playerCollider, guardCollider);
        //DrawRectangleRec(playerCollider, Color{0,0,0,255});// --Debug--
        //DrawRectangleRec(guardCollider, Color{255, 0, 0, 255});
        if (touchingGuard)
        {
            hurtPlayer(enemyIndex); // Refactor to be a method of the prisoner class
        }
        
    }
}

void nextRound()
{
    roundNo++;
    score += DOOR_SCORE;
    if (rand() % 2) exitDoor = GREEN_DOOR;
    else exitDoor = BLUE_DOOR;
    if (roundNo > 5)
    {
        locked = true;
        key.setPos(generateRandomPos());
    }
    startRound(roundNo, player, enemies, exitDoor);
}

void playNormal()
{
    bool hitExit = CheckCollisionRecs(playerCollider, exitCollider);
    player.move();
    bool paused = IsKeyDown(KEY_P) && pauseFrames > 0;
    if (!paused)
    {
        moveGuards();
    }
    else
    {
        for (int index = 0; index < getNumGuards(roundNo); index++)
        {
            enemies[index].setSpotted(false);
        }
        pauseFrames--;
    }
    bool hitKey = CheckCollisionRecs(key.getCollider(), playerCollider);
    if (IsKeyDown(KEY_E))
    {
        if (hitKey)
        {
            locked = false;
            score += KEY_SCORE;
        }

        if (hitExit && !locked)
        {
            nextRound();
        }
    }
    if ((hitExit && !locked) || (hitKey && locked))
    {
        offerInteract(eKey, Vector2{ (float)(player.getX()), (float)(player.getY()) }, eBreak);
    }
    player.animate();

}

void knockBack()
{
    knockbackFrame--;
    if (!touchedWall)
    {
        player.transform(-playerTrajectory.x / (KNOCK_BACK_FRAMES / KNOCK_BACK_DISTANCE), (-playerTrajectory.y / (KNOCK_BACK_FRAMES / KNOCK_BACK_DISTANCE)) / 2);
    }
    touchedWall = touchedWall || player.isTouchingWall();
    if (touchedWall)
    {
        //player.transform(toGuard.x/(KNOCK_BACK_FRAMES/KNOCK_BACK_DISTANCE), (toGuard.y/(KNOCK_BACK_FRAMES/KNOCK_BACK_DISTANCE))/2); -- For bounce but too broken XD
        enemies[lastHitIndex].transform(ENEMY_BOUNCE * toGuard.x / abs(toGuard.x), ENEMY_BOUNCE * toGuard.y / abs(toGuard.y));
    }

    player.draw(false);
}

void drawUI()
{
    for (int heartIndex = 0; heartIndex < health; heartIndex++)
    {
        healthBar[heartIndex].draw(true);
    }
    DrawText(FormatText("Score: %6i", score), BORDER_SIZE, SCREEN_HEIGHT - BORDER_SIZE, 25, RED);
}

void playGame()
{
    drawGround(ground, roundNo);
    exitDoor.draw(false);
    lock.setPos(Vector2{ (float)(exitDoor.getX()), (float)(exitDoor.getY()) });

    if (locked)
    {
        lock.draw(false);
        key.draw(false);
    }

    giveHint();

    
    for (int guardIndex = 0; guardIndex < getNumGuards(roundNo); guardIndex++) enemies[guardIndex].draw(false); // Draw the guards

    playerCollider = player.getCollider();
    exitCollider = exitDoor.getCollider();

    if (!knockbackFrame)
    {
        playNormal();
    }
    else
    {
        knockBack();
    }

    drawUI();

}

void endScreen()
{
    DrawText("Game Over!", 250, 170, 80, RED);
    //Display our score
    DrawText(FormatText("Score: %6i", score), 260, 250, 60, RED);
    continueButton.draw(true);

    if (CheckCollisionPointRec(GetMousePosition(), continueButton.getCollider()))
    {
        continueButton.setFrame(1);
        if (IsMouseButtonPressed(0))
        {
            started = false;
            health = START_HEALTH;
            roundNo = 1;
            score = 0;
        }
    }
    else
    {
        continueButton.setFrame(0);
    }
}

static void UpdateDrawFrame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (!started) //Start Menu
    {
        startMenu();
    }
    else 
    {
        if (health > 0) //Playing the game
        {
            playGame();
        }
        else // Game Over Screen
        {
            endScreen();
        }
    }
    //if(IsKeyPressed(KEY_SPACE)) startRound(roundNo, player, enemies, exit);// --Debug--
    //if(IsKeyPressed(KEY_SPACE)) pause = !pause;

    EndDrawing();
}


int main(int argc, char* argv[])
{

    srand(time(NULL));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mini Jam 60 Entry - ItzTheNikolai");

    for (int index = 0; index < 100; index++) enemies[index].ready();
    ground = sprite{ "resources/ground.png", Vector2{0.0f, 0.0f}, Vector2{303.0f, 303.0f} };
    player = prisoner{ Vector2{0.0f, 0.0f} };
    BLUE_DOOR = sprite{ "resources/door_blue.png", HEALTH_BAR_POS, DOOR_SIZE };
    GREEN_DOOR = sprite{ "resources/door_green.png", HEALTH_BAR_POS, DOOR_SIZE };
    exitDoor = BLUE_DOOR;

    for (int index = 0; index < START_HEALTH; index++)
    {
        hpPos.x += HEART_WIDTH;
        healthBar[index] = sprite{ "resources/heart.png", hpPos, Vector2{(float)(HEART_WIDTH), (float)(HEART_HEIGHT)} };
    }
    lock = sprite{ "resources/lock.png", HEALTH_BAR_POS, DOOR_SIZE };
    key = sprite{ "resources/key.png", HEALTH_BAR_POS, KEY_SIZE };
    play = animatedsprite{ "resources/play.png", PLAY_POS, PLAY_SIZE };
    eKey = animatedsprite{ "resources/e.png", PLAY_POS, E_SIZE };
    continueButton = animatedsprite{ "resources/continue.png", CONTINUE_POS, CONTINUE_SIZE };
    startRound(roundNo, player, enemies, exitDoor);


#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif  


    CloseWindow();        // Close window and OpenGL context

    return 0;
}