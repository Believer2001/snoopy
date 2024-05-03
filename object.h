#ifndef OBJECT_H
#define OBJECT_H
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include "pthread.h"
typedef uint8_t coord_t;
typedef uint8_t Object;
typedef uint8_t uint;
#define BALLS_MAX_NBR 4


enum Object_Types{
    SNOOPY           = 0,
    BIRD             = 1,
    BALL             = 2,
    BLOCK_TOP        = 3,      
    BLOCK_RIGHT      = 4,    
    BLOCK_BOTTOM     = 5,   
    BLOCK_LEFT       = 6, 
    BLOCK_TO_BREAK   = 7,
    BLOCK_TO_PUSH    = 8, 
    EMPTY            = 9,
    BLOCK_FIXED      = 10,
    BOMB             = 11,
    EXPLOSION        = 12
};

enum Direction {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};

enum Auto_Move {
    AUTO_M_NONE,
    AUTO_M_TOP,
    AUTO_M_BOTTOM,
    AUTO_M_LEFT,
    AUTO_M_RIGHT
};


typedef struct ball {
    coord_t   x, y;
    int direction_x;
    int direction_y;
} Ball;


typedef struct snoopy {
    coord_t      x, y;
    uint    life_numb;
    uint  game_is_over;
    uint    birds_numb;
} Snoopy;


void print_object(Object object);

void draw_object(Object object, int x, int y);

Object move_object(Object object, int x, int y, int direction);

void move_balls();

void set_snoopy_position();

void anim_snoopy();

void set_ball_infos(int i, coord_t x, coord_t y, int direction_x, int direction_y);


    
#endif