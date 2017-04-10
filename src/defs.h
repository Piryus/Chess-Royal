#ifndef DEF
#define DEF

//Taille de la fenêtre
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

//Echiquier
#define CHESS_NB_SQUARE 8
#define SPACING 2
#define PAWN_SIZE 45
#define SQUARE_SIZE 85

//Menu
#define MENU_SPACING 50
#define NB_MENU_BUTTONS 4

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#endif
/* Ancienne structure
typedef struct pion
{
    int isBlack; //0=blanc 1=noir
    int x;
    int y;
    int isInInitPos; //1=position de départ 0=le pion a déjà été déplacé
    int isSelected;
} pion;*/

typedef struct Square
{
    int pawn; //0=none 1=black -1=white
    int isSelected;
    int isMoveOk;
} Square;

typedef struct date
{
    int j;
    int m;
    int a;
} date;

/*typedef struct game
{
    int id;
    int ia;
    int scoreB;
    int scoreW;
    int turn;
    date date;
    pion pions[32];
} game;*/

//Prototypes des fonctions gérant le jeu
extern void game(SDL_Renderer *renderer);
extern void initialize_pawns_pos(Square square[][8]);
extern SDL_Rect get_clicked_square(int x, int y);
extern int posy(int numcase);
extern int posx(int numcase);
extern int numcase_to_coord_y(int numcase);
extern int numcase_to_coord_x(int numcase);
extern void show_possible_moves(SDL_Rect clickedSquare, SDL_Renderer *renderer,Square square[][8]);

//Fonctions affichage
extern void render_background(SDL_Renderer *renderer);
extern void render_base(SDL_Renderer *renderer);
extern void render_squares(SDL_Renderer *renderer);
extern void render_pawns(SDL_Renderer *renderer,Square square[][8]);

//SDL Functions
extern int RendTex(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y);
extern SDL_Texture *loadFont_Blended(SDL_Renderer *renderer, TTF_Font *police, char texte[], int red, int green, int blue);
extern SDL_Texture *loadIMG(char path[], SDL_Renderer *renderer);
extern int init(SDL_Window **window,SDL_Renderer **renderer);

//Menu
extern void menu(void);
extern void render_menu_buttons(SDL_Renderer *renderer);
extern void render_menu_background(SDL_Renderer *renderer);
extern void render_game_title(SDL_Renderer *renderer);

