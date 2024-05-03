#include "screen.h"
#include "game.h"
extern Game *game;
extern HANDLE console;


/*
place le curseur à la position x,y
correspondant à sa position dans
la matrice des objets
*/
void go_to(int x, int y)
 {
    COORD coord;
    coord.X = 5*x - 3;
    coord.Y = 2*y - 1;
    SetConsoleCursorPosition(console, coord);
}


/*
place le curseur à la position x,y
de la console
*/
void _go_to(int x, int y)
 {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(console, coord);
}


/*
efface l'écran
*/
void clean_screen()
 {
    system("cls");
}


/*
supprime le curseur réel
censé être dessiné sur la
console
*/
void del_cursor() {
    CONSOLE_CURSOR_INFO cursor_infos;
    GetConsoleCursorInfo(console, &cursor_infos);
    cursor_infos.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursor_infos);
}


void set_color(int color) {
    SetConsoleTextAttribute(console, color_Noir*16+color);
}


void print(int color, char *format, ...) {
    _go_to(X_MAX, 2*(Y_MAX + 1));
    printf("\x1b[2K");
    set_color(color);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    set_color(color_Blanc);
}


//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--////--//--//--//--//--//--//--//--//
// DESSIN DU CADRE DE JEU ET DES OBJETS A LEURS PLACES
//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--////--//--//--//--//--//--//--//--//

void print_quit_help();
void print_score();

#define box_horizontal   0xc4//0xc4//0xcd //   ═
#define box_vertical     0xb3//0xb3//0xba //   ║
#define box_top_left     0xda//0xb3//0xc9 //   ╔
#define box_top_right    0xbf//0xb3//0xbb //   ╗
#define box_bottom_left  0xc0//0xb3//0xc8 //   ╚
#define box_bottom_right 0xd9//0xb3//0xbc //   ╝
#define box_center       0xc5
#define box_left_join    0xc3
#define box_right_join   0xb4
#define box_top_join     0xc2
#define box_bottom_join  0xc1

static void _center(int charcode) {
    printf("%c%c%c%c%c", box_horizontal, box_horizontal, charcode, box_horizontal, box_horizontal);
}
static void _left(int charcode) {
    printf("%c%c%c", charcode, box_horizontal, box_horizontal);
}
static void _right(int charcode) {
    printf("%c%c%c\n", box_horizontal, box_horizontal, charcode);
}
static void _onhoriz_draw_vertical() {
    printf("%c  ", box_vertical);
    for(int x=1; x<=X_MAX-1; x++)
        printf("  %c  ", box_vertical);
    printf("  %c\n", box_vertical);
}
static void _onhoriz_draw_center() {
    _left(box_left_join);
    for(int x=1; x<=X_MAX-1; x++)
        _center(box_center);
    _right(box_right_join);
}


/*
dessine le cadre du jeu &
les objets spécifiés dans
la matrice
*/
void draw_game_box() {
    /* effacer tout l'ecran d'abord */
    clean_screen();
    set_color(color_Gris_f);
    /* on dessine le bord superieur */
    _go_to(0,0);
    _left(box_top_left);
    for(int x=1; x<=X_MAX-1; x++)
        _center(box_top_join);
    _right(box_top_right);

    /* dessine tout ce qui est entre le bord
        superieur et celui inferieur */
    for(int y=1; y<2*Y_MAX-1; y += 2) {
        _onhoriz_draw_vertical();
        _onhoriz_draw_center(); 
    }

    /* on dessine le bord inferieur */
    _onhoriz_draw_vertical();
    _left(box_bottom_left);
    for(int x=1; x<=X_MAX-1; x++)
        _center(box_bottom_join);
    _right(box_bottom_right);

    set_color(color_Blanc);

    /* on affiche divers textes d'information sur l'écran */
    print_quit_help();
    print_pause();
    print_score();

    /* on dessine les objets à leur place */
    // draw_game_box_objects();
}


void draw_game_box_objects() {
    for(int x=1; x<=X_MAX; x++)
        for(int y=1; y<=Y_MAX; y++)
            if(game->box[x-1][y-1]!=EMPTY) {
                go_to(x,y);
                print_object(game->box[x-1][y-1]);
            }
}


void clean_game_box() {
    for(int x=1; x<=X_MAX; x++)
        for(int y=1; y<=Y_MAX; y++)
            if(game->box[x-1][y-1]!=EMPTY) {
                Sleep(100); // pour créer une petite animation de disparition des objets
                draw_object(EMPTY, x, y);
            }
}


//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--////--//--//--//--//--//--//--//--//
//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--////--//--//--//--//--//--//--//--//

inline static void clean_forward(int n) {
    for(int i=0; i<n; i++)
        printf(" ");//effacer les caracères devant
}


void set_player(int n) {
    _go_to(5*X_MAX + 10, 1);
    clean_forward(1);
    _go_to(5*X_MAX + 3, 1);
    set_color(color_Bleu_fluo);
    printf("Player %d", n);
    set_color(color_Blanc);
}


void print_score() {
    _go_to(5*X_MAX + 3, 3);
    printf("Score:");
}
void set_score(int score) {
    _go_to(5*X_MAX + 10, 3);
    clean_forward(7); //supprimer les chiffres devant score
    _go_to(5*X_MAX + 10, 3);
    printf("%d", score);
}


void set_level(int n) {
    _go_to(5*X_MAX + 9, 7);
    clean_forward(2);
    _go_to(5*X_MAX + 3, 7);
    printf("Level %d", n);
}


void set_heart_numb(int i) {
    _go_to(5*X_MAX + 3, 9);
    clean_forward(15);
    for(int n=1; n<=i; n++) {
        _go_to(5*X_MAX + 3 + n*3, 9);
        WriteConsoleW(console, L"\u2764\uFE0F", wcslen(L"\u2764\uFE0F"), NULL, NULL);
        // WriteConsoleW(console, L"\u23F3", wcslen(L"\u23F3"), NULL, NULL);
    }
}


void update_time(int i) {
    _go_to(5*X_MAX + 3, 13);
    WriteConsoleW(console, L"\u23F3", wcslen(L"\u23F3"), NULL, NULL);
    // printf(" Time restant:");
    _go_to(5*X_MAX + 7, 13);
    clean_forward(3);
    _go_to(5*X_MAX + 7, 13);
    set_color(color_Turquoise);
    printf("%d", i);
    set_color(color_Blanc);
}


static void _print_help(int i, WCHAR *c, char *str) {
    _go_to(X_MAX, 2*(Y_MAX + i));
    printf("\x1b[2K");//efface la ligne
    WriteConsoleW(console, c, wcslen(c), NULL, NULL);
    printf("  %s", str);
}
void print_resume() {
    _print_help(3, L"\u25B6\uFE0F", "tapez 'p'   pour reprendre le jeu");
}
void print_pause() {
    _print_help(3, L"\u23F8\uFE0F", "tapez 'p'   pour mettre le jeu en pause");
}
void print_quit_help() {
    _print_help(2, L"\U0001f519", "'q' ou 'Q'");
}

//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--////--//--//--//--//--//--//--//--//
//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--////--//--//--//--//--//--//--//--//

/*
Fait un compte à rebours avant de commencer un jeu */
void count_down() {
    for(int i=3; i>=1; i--) {
        print(color_Turquoise_2, "%d", i);
        Sleep(1000);
    }
    print(color_Blanc, "GO  !!!!");
}


