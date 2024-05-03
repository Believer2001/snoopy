#ifndef GAME_H
#define GAME_H
typedef struct game Game;
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "level.h"
#include <time.h>
#include "object.h"
#define GAME_PASSED  1
#define GAME_LOST  0
#define GAME_EXIT   -1

#define PLAYER_N 3

#define object_at(x,y) (game->box[(x)-1][(y)-1])


struct  game  {
    Level      level;
    Snoopy    snoopy;
    int       player;
    int        score;
    int    move_ball;
    int  change_time;
    int   time_spend;
    int       paused;
    int   scores[PLAYER_N][3];
    Ball balls[BALLS_MAX_NBR];
    Object  box[X_MAX][Y_MAX];
};


void Initialize_Game();


#endif