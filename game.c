#include "game.h"
#include "conio.h"
#include "screen.h"
#include "pthread.h"

HANDLE  console;
Game *game = NULL;
int auto_move = EMPTY;
coord_t auto_x, auto_y;
time_t ball_time, timer_time, auto_move_time;

void Print_Rules();
void Run_New_Game();
void Load_Saved_Game();
void Play_A_Level();
void Print_Scores();
void Shut_Game();
static int run_a_game();
static void init_a_level();
static void run_current_and_continue();
static void get_scores();
static void save_scores();



/*
initialise le jeu
et affiche le menu
*/
void Initialize_Game() {
    game = malloc(sizeof(Game));
    if ((console = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Erreur: Gestion de la console impossible\n");
        exit(EXIT_FAILURE);
    }
    del_cursor();  // supprimer le  curseur
    srand(time(NULL));
    game->player = 1;
    while(1) {
        clean_screen();
        printf("   MENU\n\n");
        printf("1. Regles du jeu\n");
        printf("2. Lancer un nouveau jeu\n");
        printf("3. Charger une partie\n");
        printf("4. Lancer un niveau via son mot de passe\n");
        printf("5. Scores\n");
        printf("6. Quitter (ou 'q')\n\n");
        printf("<-  Joueur %d  ->\n", game->player);
        switch(getch()) {
            case '1': Print_Rules();      break;
            case '2': Run_New_Game();     break;
            case '3': Load_Saved_Game();  break;
            case '4': Play_A_Level();     break;
            case '5': Print_Scores();     break;
            case 'q': case 'Q':
            case '6': Shut_Game();        break;
            case 75://left
                game->player = game->player==1 ? 3 : game->player-1;
                break;
            case 77://right
                game->player = game->player==3 ? 1 : game->player+1;
                break;
        }
    }
}



void Print_Rules() {
    clean_screen();
    FILE *file = fopen("rules.txt", "r");
    char c;
    while((c = fgetc(file)) != EOF) 
        printf("%c", c);
    fclose(file);
    printf("\n\n...Appuyez sur n'importe quelle touche pour retourner au menu");
    getch();
}



void Run_New_Game() {
    game->snoopy.life_numb = 3;
    _load_level(1);
    init_a_level();
    draw_game_box();
    run_current_and_continue();
}


void Load_Saved_Game() {
    clean_screen();
    char file_name[100];
    printf("Donnez le nom du fichier: ");
    scanf("%s", file_name);
    FILE *file = fopen(file_name, "rb");
    fread(&game, sizeof(Game), 1, file);
    draw_game_box();
    run_current_and_continue();
}


void Save_A_Game() {
    char file_name[100];
    print(color_Bleu_fluo, "Donnez le nom du fichier: ");
    scanf("%s", file_name);
    FILE *file = fopen(file_name, "wb");
    fwrite(&game, sizeof(Game), 1, file);
    run_current_and_continue();
}


void Play_A_Level() {
    clean_screen();
    char code[100];
    printf("Donnez le code du niveau ou 'q' pour quitter\n");
    do {
        printf(">");
        scanf("%s", code);
        if(strlen(code)==1 && (code[0]='q' || code[0]=='Q'))
            return;
    } while(!load_level(code));
    game->snoopy.life_numb = 1;
    draw_game_box();
    init_a_level();
    run_a_game();
}


void Print_Scores() {
    clean_screen();
    get_scores();
    _go_to(0, 2);
    printf("Last score");
    _go_to(0, 4);
    printf("Best score");
    for(int i=1; i<=PLAYER_N; i++) {
        _go_to(15*i, 0);
        printf("Player %d", i);
        set_color(color_Vert_caca_d_oie);
        _go_to(15*i, 2);
        printf("%d", game->scores[i-1][0]);
        _go_to(15*i, 4);
        printf("%d", game->scores[i-1][1]);
        set_color(color_Blanc);
    }
    _go_to(0, 6);
    printf("...Appuyez n'importe quelle touche");
    getch();
}


void Shut_Game() {
    clean_screen();
    set_color(color_Blanc);
    free(game);
    exit(EXIT_SUCCESS);
}

//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

/*
Fonction qui fera le decompte pour le temps de jeu */
void check_timer_time() {
    time_t curr_time = time(NULL);
    if(difftime(curr_time, timer_time)>=1.) {
        timer_time = curr_time;
        game->change_time = 1;
    }
}

/*
Fonction qui fera un comptage pour déterminer quand on doit
afficher les balles à leur nouvelles positions */
void check_ball_time() {
    time_t curr_time = time(NULL);
    if(difftime(curr_time, ball_time)>=game->level.ball_latence) {
        ball_time = curr_time;
        game->move_ball = 1;
    }
}

void check_auto_move() {
    if(auto_move!=EMPTY) {
        time_t curr_time = time(NULL);
        if(difftime(curr_time, ball_time)>=game->level.ball_latence) {
            ball_time = curr_time;
        }
        switch(auto_move) {
            case BLOCK_TOP:
                if(move_object(BLOCK_TOP, auto_x, auto_y, TOP)!=EMPTY)
                    auto_y -= 1;
                else auto_move = EMPTY;
                break;
            case BLOCK_BOTTOM:
                if(move_object(BLOCK_TOP, auto_x, auto_y, BOTTOM)!=EMPTY)
                    auto_y += 1;
                else auto_move = EMPTY;
                break;
            case BLOCK_LEFT:
                if(move_object(BLOCK_TOP, auto_x, auto_y, LEFT)!=EMPTY)
                    auto_x -= 1;
                else auto_move = EMPTY;
                break;
            case BLOCK_RIGHT:
                if(move_object(BLOCK_TOP, auto_x, auto_y, LEFT)!=EMPTY)
                    auto_x += 1;
                else auto_move = EMPTY;
                break;
        }
    }
}
/*
Represente un jeu
Gère tous les évènements lors du jeu d'un niveau */
static int run_a_game() {
    char pressed;
    Snoopy *snoopy = &game->snoopy;
    draw_game_box_objects();
    draw_object(SNOOPY, snoopy->x, snoopy->y);
    update_time(game->level.game_time);
    set_score(game->score);
    set_heart_numb(snoopy->life_numb);
    set_player(game->player);
    set_level(game->level.level_num);
    count_down();
    ball_time = timer_time = time(NULL);

    while(1) {
        check_ball_time();
        check_timer_time();
        if(game->paused) {
            Sleep(50);
            if(_kbhit()) {
                switch(_getch()) {
                    case 'p':
                        game->paused = 0;
                        print_pause();
                        break;
                    case 'q':
                    case 'Q': return GAME_EXIT;
                    default: continue;
                }
            }
            else
                continue;
        }

        if(game->snoopy.game_is_over) {
        game_over_label:
            snoopy->life_numb--;
            anim_snoopy();
            print(color_Rouge_fluo, "GAME LOST !!!");
            clean_game_box();
            return GAME_LOST;
        }
        
        if(game->move_ball)
            move_balls();

        if(game->change_time) {
            update_time(game->level.game_time - game->time_spend);
            if(++game->time_spend>=game->level.game_time)
                goto game_over_label;
            game->change_time = 0;
        }

        if(game->snoopy.birds_numb>=4) {
            anim_snoopy();
            print(color_Vert_caca_d_oie, "GAME %d   PASSED !!!", game->level.level_num);
            clean_game_box();
            return GAME_PASSED;
        }

        
        if(_kbhit()) {
            pressed = _getch();

            switch(pressed) {

                case 72: move_object(SNOOPY, snoopy->x, snoopy->y, TOP);
                    break;
                case 80: move_object(SNOOPY, snoopy->x, snoopy->y, BOTTOM);
                    break;
                case 75: move_object(SNOOPY, snoopy->x, snoopy->y, LEFT);
                    break;
                case 77: move_object(SNOOPY, snoopy->x, snoopy->y, RIGHT);
                    break;

                case 'p':
                    game->move_ball = 0;
                    game->paused = 1;
                    print_resume();
                    break;

                case 'Q':
                case 'q':
                    return GAME_EXIT;
            }
        }
        Sleep(20);
    }
    return 0;
}



/*
Lance le jeu à partir du niveau déjà initialisé pour
poursuivre */
static void run_current_and_continue() {
    int result;
    int level = game->level.level_num;
    do {
        result = run_a_game();
        if(result==GAME_EXIT)
            return;

        if(result==GAME_PASSED) {
            //lorsque le niveau est passé
            game->score += (game->level.game_time - game->time_spend)*100;
            if(level>=LEVEL_NUMB) {
                print(color_Vert_caca_d_oie, "Felicitations !!   Vous avez passe avec succes tous les niveaux  ...Tapez n'importe quelle touche");
                save_scores();
                getchar();
                return;
            }
            level++;
            _load_level(level);
            init_a_level();
        }
        else if(result==GAME_LOST) {
            if(game->snoopy.life_numb<1) {
                //lorsque le nombre de vie est fini
                print(color_Rouge_fluo, "Nombre de vies epuisees  ...Tapez n'importe quelle touche");
                save_scores();
                getchar();
                return;
            }
            init_a_level();
        }
    }
    while(level>=LEVEL_NUMB);
}



static void init_a_level() {
    memcpy(&game->box, &game->level.box, sizeof(game->box));
    game->time_spend = 0;
    game->paused = 0;
    game->score = 0;
    game->move_ball = 0;
    game->change_time = 1;
    game->snoopy.game_is_over = 0;
    game->snoopy.birds_numb = 0;
    set_snoopy_position();
}



static void get_scores() {
    FILE *file = fopen("scores", "r");
    clean_screen();
    if(file==NULL) {
        for(int i=0; i<PLAYER_N; i++) {
            game->scores[i][0]=0;
            game->scores[i][1]=0;
        }
        return;
    }
    for(int i=0; i<PLAYER_N; i++)
        for(int j=0; j<2; j++) {
            fscanf(file, "%d", &(game->scores[i][j]));
        }
    fclose(file);
}

static void save_scores() {
    FILE *file = fopen("scores", "w");
    int curr_score = game->score;
    get_scores();
    if(curr_score>game->scores[game->player-1][1])
        game->scores[game->player-1][1] = curr_score;
    game->scores[game->player-1][0] = curr_score;
    for(int i=0; i<PLAYER_N; i++)
        for(int j=0; j<2; j++)
            fprintf(file, "%d ", game->scores[i][j]);
    fclose(file);
}
