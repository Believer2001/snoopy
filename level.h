#ifndef LEVEL_H
#define LEVEL_H
typedef struct level_data Level;
#include "screen.h"
#include "object.h"
#define LEVEL_NUMB 3

struct level_data {
    Object  box[X_MAX][Y_MAX];
    int      level_num;
    int     balls_numb;
    float ball_latence;
    int      game_time;
};


int load_level(char *code);

void _load_level(int i);

#endif