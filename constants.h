#pragma once

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;
const int BORDER_SIZE = 40;
const int IDLE_FRAME = 0;
const int RUNNING_START_FRAME = 1;
const int RUNNING_END_FRAME = 3;
const int ENEMY_BOUNCE = 4;
const int HEART_WIDTH = 215/5;
const int HEART_HEIGHT = 43;
const Vector2 HEALTH_BAR_POS = Vector2{ (float)((SCREEN_WIDTH * 3)/4), 15.0f};
const int KNOCK_BACK_FRAMES = 25;
const int FPS = 60;
const int KNOCK_BACK_DISTANCE = 2;
const int START_HEALTH = 3;
const int prisoner_WIDTH = 60;
const int prisoner_HEIGHT = 94;
const int BASE_SPEED = 3;

const int PRISONER_PAUSE = 8;
const int MIN_SPAWN_DISTANCE_X = 200;
const int MIN_SPAWN_DISTANCE_Y = 150;
const int ENEMY_WIDTH = 60;
const int ENEMY_HEIGHT = 87;

const int ENEMY_MAX_MOVE_FRAMES = 80;
const int ENEMY_MIN_MOVE_FRAMES = 60;
const int ENEMY_PAUSE = 10;
const int ENEMY_VIEW = 100;
const int ENEMY_WALK_SPEED = 1;
const int ENEMY_RUN_SPEED = 2;
const Vector2 ALERT_SIZE = Vector2{33.0f, 60.0f};
const Vector2 ALERT_OFFSET = Vector2{0.0f, -50.0f};
const Vector2 DOOR_SIZE = Vector2{52.0f, 100.0f};
const Vector2 KEY_SIZE = Vector2{50.0f, 22.0f};
const Vector2 PLAY_SIZE = Vector2{500.0f, 167.0f};
const Vector2 PLAY_POS = Vector2{230.0f, 230.0f};
const Vector2 E_SIZE = Vector2{25.0f, 23.0f};
const Vector2 E_OFFSET = Vector2{ (float)(prisoner_WIDTH/2 - E_SIZE.x/2), -30.0f};
const int E_ANIMATION_BREAK = 45;
const int MAX_PAUSE_FRAMES = 5 * FPS;
const Vector2 HINT_TEXT_POS = Vector2{10.0f, 10.0f};
const int HINT_TEXT_SIZE = 30;
const Color HINT_COLOR = RAYWHITE;
const int KEY_SCORE = 500;
const int DOOR_SCORE = 1000;
const Vector2 CONTINUE_POS = Vector2{300.0f, 330.0f};
const Vector2 CONTINUE_SIZE = Vector2{300.0f, 100.0f};

//Sprites
const char prisoner_FILE[] = "resources/prisoner.png";
const char PRISONER_PNG_FLIPPED[] = "resources/prisoner_flipped.png";
const char ENEMY_FILE[] = "resources/cop.png";
const char COP_PNG_FLIPPED[] = "resources/cop_flipped.png";