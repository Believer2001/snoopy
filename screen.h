#ifndef SCREEN_H
#define SCREEN_H
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#define X_MAX 20
#define Y_MAX 10

enum {
    color_Noir,
    color_Bleu_f,
    color_Vert_f,
    color_Turquoise,
    color_Rouge_f,
    color_Violet,
    color_Vert_caca_d_oie,
    color_Gris_clair,
    color_Gris_f,
    color_Bleu_fluo,
    color_Vert_fluo,
    color_Turquoise_2,
    color_Rouge_fluo,
    color_Violet_2,
    color_Jaune,
    color_Blanc
};



void draw_game_box();

void clean_game_box();

void draw_game_box_objects();

void go_to(int x, int y);

void _go_to(int x, int y);

void set_color(int color);

void set_score(int score);

void set_heart_numb(int i);

void set_player(int n);

void set_level(int n);

void update_time(int i);

void print_quit_help();

void count_down();

void print_resume();
void print_pause();



void clean_screen();

void del_cursor();

void print(int color, char *format, ...);


#endif