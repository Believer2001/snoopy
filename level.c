#include "level.h"
#include "game.h"
#include <string.h>
#include <stdarg.h>
extern Game *game;

static void set(Object object, int x, int y) {
    game->level.box[x-1][y-1] = object;
}

static void set_basics(int level_num, int game_time, float ball_latence, int balls_num) {
    game->level.level_num = level_num;
    game->level.ball_latence = ball_latence;
    game->level.balls_numb = balls_num;
    game->level.game_time = game_time;
}

static void set_object_positions(Object object, int n, ...) {
    va_list args;
    va_start(args, n);
    int coord_x, coord_y;
    for(int i=0; i<n; i++) {
        coord_x = va_arg(args, int);
        coord_y = va_arg(args, int);
        set(object, coord_x, coord_y);
    }
    va_end(args);
}


//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//


void load_level_1();
void load_level_2();
void load_level_3();


int load_level(char *code) {
    int len = strlen(code);
    int level = 0;
    if(!strcmp(code, "#1"))
        level = 1;
    if(!strcmp(code, "#2"))
        level = 2;
    if(!strcmp(code, "#3"))
        level = 3;
    if(level)
        _load_level(level);
    return level;
}


void _load_level(int i) {
    for(int x=0; x<X_MAX; x++)
        for(int y=0; y<Y_MAX; y++)
            game->level.box[x][y] = EMPTY;
    switch(i) {
        case 1: load_level_1();
            break;
        case 2: load_level_2(); 
            break;
        case 3: load_level_3(); 
            break;
    }
}


//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//


void load_level_1() {
    //définition des paramètres de base du niveau
    set_basics(1, 120, 0.2, 1);
    //on place la balle
    set_ball_infos(0, 5, 5, 1, -1);
    //on place 8 blocs fixes aux positions spécifiées
    set_object_positions(BLOCK_TOP, 1, 
        8,8
    );
    set_object_positions(BLOCK_FIXED, 8, 
        3, 2,
        X_MAX-2, 2,
        3, Y_MAX-1,
        X_MAX-2, Y_MAX-1,
        7, 4,
        X_MAX-6, 4,
        7, Y_MAX-3,
        X_MAX-6, Y_MAX-3
    );
    //on place les 4 oiseaux aux coins
    set_object_positions(BIRD, 4,
        1, 1,
        1, Y_MAX,
        X_MAX, 1,
        X_MAX, Y_MAX
    );
}



void load_level_2() {
    //définition des paramètres de base du niveau
    set_basics(2, 120, 0.1, 1);
    //on place la balle
    set_ball_infos(0, 9, 3, 1, -1);
    //on place 8 blocs fixes aux positions spécifiées
    set_object_positions(BLOCK_FIXED, 8, 
        3, 2,
        X_MAX-2, 2,
        3, Y_MAX-1,
        X_MAX-2, Y_MAX-1,
        7, 4,
        X_MAX-6, 4,
        7, Y_MAX-3,
        X_MAX-6, Y_MAX-3
    );
    set_object_positions(BLOCK_TO_BREAK, 8,
        2, 1,
        1, 2,
        X_MAX-1, 1,
        X_MAX-1, 2,
        1, Y_MAX-1,
        2, Y_MAX,
        X_MAX-1, Y_MAX,
        X_MAX, Y_MAX-1
    );
    set_object_positions(BOMB, 2,
        3, 6,
        X_MAX-2, 4
    );
    //on place les 4 oiseaux aux coins
    set_object_positions(BIRD, 4,
        1, 1,
        1, Y_MAX,
        X_MAX, 1,
        X_MAX, Y_MAX
    );
}


void load_level_3() {
    //définition des paramètres de base du niveau
    set_basics(2, 120, 0.05, 1);
    //on place la balle
    set_ball_infos(0, 9, 3, 1, -1);
    //on place 8 blocs fixes aux positions spécifiées
    set_object_positions(BLOCK_FIXED, 8, 
        3, 2,
        X_MAX-2, 2,
        3, Y_MAX-1,
        X_MAX-2, Y_MAX-1,
        7, 4,
        X_MAX-6, 4,
        7, Y_MAX-3,
        X_MAX-6, Y_MAX-3
    );
    set_object_positions(BLOCK_TO_BREAK, 8,
        2, 1,
        1, 2,
        X_MAX-1, 1,
        X_MAX-1, 2,
        1, Y_MAX-1,
        2, Y_MAX,
        X_MAX-1, Y_MAX,
        X_MAX, Y_MAX-1
    );
    set_object_positions(BOMB, 4,
        3, 6,
        X_MAX-2, 4,
        7, 5,
        9, Y_MAX-1
    );
    //on place les 4 oiseaux aux coins
    set_object_positions(BIRD, 4,
        1, 1,
        1, Y_MAX,
        X_MAX, 1,
        X_MAX, Y_MAX
    );
}

