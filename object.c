#include "object.h"
#include "screen.h"
#include "game.h"
extern Game *game;
extern HANDLE console;

WCHAR objects_unicode[][10] = {
    L"\U0001f436",
    L"\U0001f426",//L"\U0001f425",
    L"\U0001f535",//L"\u26AA",//L"\u26AB",
    L"\u2B06\uFE0F",
    L"\u27A1\uFE0F",
    L"\u2B07\uFE0F",
    L"\u2B05\uFE0F",
    L"\U0001f9f1",
    L"\U0001f532",
    L" ",
    L"\U0001f6a7",
    L"\U0001f4a3",
    L"\U0001f4a5"
};


/*
dessiner le caractère unicode lié
à l'objet spécifié
*/
void print_object(Object object) {
 WriteConsoleW(console, objects_unicode[object], wcslen(objects_unicode[object]), NULL, NULL);
}


/*
dessiner l'objet spécifié à la position x,y
*/
void draw_object(Object object, int x, int y) {
    go_to(x, y);
    game->box[x-1][y-1] = object;
    WriteConsoleW(console, objects_unicode[object], wcslen(objects_unicode[object]), NULL, NULL);
}


/*
trouve une position aléatoire
à snoopy
*/
void set_snoopy_position() {
    int is_empty;
    coord_t x, y;
    do {
        x = (rand()%(X_MAX))+1;
        y = (rand()%(Y_MAX))+1;
        is_empty = object_at(x,y)==EMPTY;
    } while(!is_empty);
    game->snoopy.x = x;
    game->snoopy.y = y;
}


/*
Animation d'apparition et de disparition
de Snoopy */
void anim_snoopy() {
    for(int i=100; i>40; i-=10) {
        draw_object(EMPTY, game->snoopy.x, game->snoopy.y);
        Sleep(i);
        draw_object(SNOOPY, game->snoopy.x, game->snoopy.y);
        Sleep(i);
    }
}


void set_ball_infos(int i, coord_t x, coord_t y, int direction_x, int direction_y) {
    game->balls[i].x = x;
    game->balls[i].y = y;
    game->balls[i].direction_x = direction_x;
    game->balls[i].direction_y = direction_y;
}


/* 
object -> désigne l'objet qui souhaite se deplacer
direction -> identifiant qui indique dans quelle direction le deplacement est souhaité
_x,_y -> position dans la matrice de l'objet qui cherche à se deplacer
x,y  -> position dans la matrice de l'objet qui est en face de lui */
Object move(Object object, int _x, int _y, int x, int y, int direction) {
    switch(object_at(x, y)) {

        case BIRD:
            if(object!=SNOOPY)
                return BIRD;
            game->snoopy.birds_numb++;
        /* pas de break pour qu'il puisse bouffer l'oiseau
            et avancer dans la nouvelle position */
        case EMPTY:
            if(object!=SNOOPY)
                return EMPTY;
            draw_object(EMPTY, _x, _y);
            draw_object(object, x, y);
            game->snoopy.x = x;
            game->snoopy.y = y;
            return EMPTY;

        case BALL:
            if(object==SNOOPY)
                game->snoopy.game_is_over = 1;
            return BALL;

        case SNOOPY:
            if(object==BALL)
                game->snoopy.game_is_over = 1;
            return SNOOPY;

        case BLOCK_TO_PUSH:
            if((object==SNOOPY) && (move_object(BLOCK_TO_PUSH, x, y, direction)==EMPTY)) {
                draw_object(EMPTY, _x, _y);
                draw_object(object, x, y);
                game->snoopy.x = x;
                game->snoopy.y = y;
            }
            return BLOCK_TO_PUSH;

        case BLOCK_TO_BREAK:
            if(object!=SNOOPY)//si ce n'est pas snoopy
                return BLOCK_TO_BREAK;
            draw_object(EMPTY, x, y);
            return BLOCK_TO_BREAK;

        case BOMB:
            if(object!=SNOOPY)//si ce n'est pas snoopy
                return BOMB;
            draw_object(EXPLOSION, x, y);
            game->snoopy.game_is_over = 1;
            return BOMB;

        default:
            return object_at(x, y);
    }
    return 30;
}


/* bouge l'objet si possible sinon renvoie
    le type de l'obstacle */
Object move_object(Object object, int x, int y, int direction) {
    switch(direction) {
        
        case TOP:
            if(y-1>=1)/* verifier qu'on ne depasse pas le bord haut */
                return move(object, x, y, x, y-1, direction);
            break;

        case LEFT:
            if(x-1>=1)
                return move(object, x, y, x-1, y, direction);
            break;

        case BOTTOM:
            if(y+1<=Y_MAX)
                return move(object, x, y, x, y+1, direction);
            break;

        case RIGHT:
            if(x+1<=X_MAX)
                return move(object, x, y, x+1, y, direction);
            break;

    }
    return 30;
}


/* bouge les balles si possible sinon change la
    la direction de celles-ci */
void move_balls() {
    Ball *ball;
    Object object;
    for(int i=0; i<game->level.balls_numb; i++) {
        ball = &game->balls[i];
        if(ball->x>=X_MAX || ball->x<=1)
            ball->direction_x = -ball->direction_x;
        if(ball->y>=Y_MAX || ball->y<=1)
            ball->direction_y = -ball->direction_y;
        object = object_at(ball->x+ball->direction_x, ball->y+ball->direction_y);
        switch(object) {
            case EMPTY:
                draw_object(EMPTY, ball->x, ball->y);
                draw_object(BALL, ball->x += ball->direction_x, ball->y += ball->direction_y);
                break;
            case SNOOPY:
                game->snoopy.game_is_over = 1;
                break;
            default:
                ball->direction_x = -ball->direction_x;
                ball->direction_y = -ball->direction_y;
        }
    }
    game->move_ball = 0;
}

