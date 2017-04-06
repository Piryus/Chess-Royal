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
#define NB_MENU 4

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#endif

typedef struct pion
{
    int isBlack; //0=blanc 1=noir
    int x;
    int y;
    int isInInitPos; //1=position de départ 0=le pion a déjà été déplacé
} pion;

typedef struct square
{
    int x;
    int y;
} square;

typedef struct date
{
    int j;
    int m;
    int a;
} date;

typedef struct game
{
    int id;
    int ia;
    int scoreB;
    int scoreW;
    int turn;
    date date;
    pion pions[32];
} game;

//Prototypes des fonctions
extern void jeu(SDL_Window *g_Window, SDL_Renderer *g_Renderer);
