#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#include <math.h>
#include <stdlib.h>
#include "constants.h"

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




#include "sprite.h"
#include "prisoner.h"
#include <time.h>
#include "guard.h"


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

void offerInteract(sprite &eKey, Vector2 playerPos, int &eBreak)
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


prisoner player;

sprite BLUE_DOOR, GREEN_DOOR, exitDoor, ground;

int frame = 0;

bool touchingGuard;
Vector2 toGuard;

int knockbackFrame = 0;
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
bool pause = false;
bool locked = false;
bool started = false;

sprite key, play, lock, eKey, continueButton;


int eBreak = 0;

int pauseFrames = MAX_PAUSE_FRAMES;

static void UpdateDrawFrame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (!started)
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
    else {
        if (health > 0) //Playing the game
        {



            drawGround(ground, roundNo);
            exitDoor.draw(false);
            lock.setPos(Vector2{ (float)(exitDoor.getX()), (float)(exitDoor.getY())});

            if (locked)
            {
                lock.draw(false);
                key.draw(false);
            }

            //Draw the guards
            for (int guardIndex = 0; guardIndex < getNumGuards(roundNo); guardIndex++)
            {
                enemies[guardIndex].draw(false);
            }

            playerCollider = player.getCollider();
            exitCollider = exitDoor.getCollider();


            bool hitExit = CheckCollisionRecs(playerCollider, exitCollider);

            if (roundNo == 1) DrawText("Head for the door", HINT_TEXT_POS.x, HINT_TEXT_POS.y, HINT_TEXT_SIZE, HINT_COLOR);
            if (roundNo == 2) DrawText("Watch out for the guard!", HINT_TEXT_POS.x, HINT_TEXT_POS.y, HINT_TEXT_SIZE, HINT_COLOR);
            if (roundNo == 3) DrawText("The guards are fast, try not to let\n them see you", HINT_TEXT_POS.x, HINT_TEXT_POS.y, HINT_TEXT_SIZE, HINT_COLOR);
            if (roundNo == 4) DrawText("You can freeze time by holding P", HINT_TEXT_POS.x, HINT_TEXT_POS.y, HINT_TEXT_SIZE, HINT_COLOR);
            if (roundNo == 5) DrawText("Be sparing with your time freeze; it runs out\n fast", HINT_TEXT_POS.x, HINT_TEXT_POS.y, HINT_TEXT_SIZE, HINT_COLOR);
            if (roundNo == 6) DrawText("You see the key... you can guess what to do", HINT_TEXT_POS.x, HINT_TEXT_POS.y, HINT_TEXT_SIZE, HINT_COLOR);

            if (!knockbackFrame)
            {
                player.move();
                if (!IsKeyDown(KEY_P) || pauseFrames < 1)
                {
                    for (int i = 0; i < getNumGuards(roundNo); i++)
                    {
                        guardCollider = enemies[i].getCollider();
                        toGuard = Vector2{ (float)(enemies[i].getX() - player.getX()), (float)(enemies[i].getY() - player.getY()) };

                        enemies[i].checkSpotted(playerCollider);


                        touchingGuard = CheckCollisionRecs(playerCollider, guardCollider);
                        //DrawRectangleRec(playerCollider, Color{0,0,0,255});// --Debug--
                        //DrawRectangleRec(guardCollider, Color{255, 0, 0, 255});



                        if (!pause)
                        {
                            if (touchingGuard)
                            {
                                enemies[i].setSpotted(true);
                                health--;
                                player.setFrame(1);
                                knockbackFrame = KNOCK_BACK_FRAMES;
                                playerTrajectory = toGuard;
                                touchedWall = false;
                                lastHitIndex = i;
                            }

                            enemies[i].move(player.getX(), player.getY());
                        }
                    }

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
                }
                if ((hitExit && !locked) || hitKey && locked)
                {
                    offerInteract(eKey, Vector2{ (float)(player.getX()), (float)(player.getY()) }, eBreak);
                }
                player.animate();

            }
            else
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


            //Draw a heart for each of our lives
            for (int heartIndex = 0; heartIndex < health; heartIndex++)
            {
                healthBar[heartIndex].draw(true);
            }

            //Display our score
            DrawText(FormatText("Score: %6i", score), BORDER_SIZE, SCREEN_HEIGHT - BORDER_SIZE, 25, RED);

        }
        else // Game Over Screen
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
    BLUE_DOOR = sprite{ "resources/door.png", HEALTH_BAR_POS, DOOR_SIZE };
    GREEN_DOOR = sprite{ "resources/doorGreen.png", HEALTH_BAR_POS, DOOR_SIZE };
    exitDoor = BLUE_DOOR;

    for (int index = 0; index < START_HEALTH; index++)
    {
        hpPos.x += HEART_WIDTH;
        healthBar[index] = sprite{ "resources/newHeart.png", hpPos, Vector2{(float)(HEART_WIDTH), (float)(HEART_HEIGHT)} };
    }
    lock = sprite{ "resources/lock.png", HEALTH_BAR_POS, DOOR_SIZE };
    key = sprite{ "resources/key.png", HEALTH_BAR_POS, KEY_SIZE };
    play = sprite{ "resources/newPlay.png", PLAY_POS, PLAY_SIZE };
    eKey = sprite{ "resources/neweKey.png", PLAY_POS, E_SIZE };
    continueButton = sprite{ "resources/newContinue.png", CONTINUE_POS, CONTINUE_SIZE };
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