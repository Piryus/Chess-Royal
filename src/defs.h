#ifndef DEF
#define DEF

//Taille de la fen�tre
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

//Echiquier
#define CHESS_NB_SQUARE 8
#define SPACING 2
#define PAWN_SIZE 45
#define SQUARE_SIZE 85

//Menu
#define MENU_BUTTON_SPACING 50
#define NB_MENU_BUTTONS 5
#define BUTTON_WIDTH WINDOW_WIDTH/4
#define BUTTON_HEIGHT WINDOW_HEIGHT/15
#define BUTTON_OUTLINE 5
#define IA 1
#define NO_IA 0

//Constantes
#define _BLANC -1
#define _NOIR 1
#define _VIDE 0


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#endif

/*
We define our chess as below :
   0 1 2 3 4 5 6 7
   _ _ _ _ _ _ _ _
0 |_|_|_|_|_|_|_|_|
1 |_|_|_|_|_|_|_|_|
2 |_|_|_|_|_|_|_|_|
3 |_|_|_|_|_|_|_|_|
4 |_|_|_|_|_|_|_|_|
5 |_|_|X|_|_|_|_|_|
6 |_|_|_|_|_|_|_|_|
7 |_|_|_|_|_|_|_|_|

X = square[2][5]
*/


typedef struct JoueurIA{
    float gamma[7] ;
} JoueurIA;

typedef struct Joueur{
    int id;
    char Nom[40] ;
    int nbWin ;
    int nbLose ;
    int nbEgal ;
    int nbGame ;
} Joueur;

typedef struct Possibillite{
    int deX;
    int deY;
    int aX;
    int aY;
    float poids;
} Possibillite;

typedef struct Square
{
    int pawn; //0=none 1=black -1=white
    int isSelected;
    int isMoveOk;
} Square;

typedef struct Date
{
    int j;
    int m;
    int a;
} Date;

typedef struct Game
{
    int id;
    int ia;
    int joueurB;
    int joueurN;
    int scoreB;
    int scoreN;
    int winner;
    int tour;
    int plateau[8][8];
} Game;

//Prototypes des fonctions g�rant le jeu
//Fonction renvoyant la case sur laquelle on a cliqu�
extern SDL_Rect get_clicked_square(int x, int y);
 //Fonction principal g�rant le jeu
extern void game(SDL_Renderer *renderer,Square square[][8], int ia, Game *game);
//Fonction initialisant la position des pions
extern void initialize_pawns_pos(Square square[][8]);
//Fonction transposant le num�ro d'une case en coordon�es pour le pion (y)
extern int posy(int numcase);
//Fonction transposant le num�ro d'une case en coordon�es pour le pion (x)
extern int posx(int numcase);
//Fonction transposant le num�ro d'une case en abscisses (de 0 � 8) en coordon�es du coin sup�rieur gauche de cette case(x)
extern int numcase_to_coord_y(int numcase);
//Fonction transposant le num�ro d'une case en abscisses (de 0 � 8) en abscisse du coin sup�rieur gauche de cette case(x)
extern int numcase_to_coord_x(int numcase);
//Remet � 0 tous les d�placements possibles
extern void reset_OK_moves(Square square[][8]);
//D�place le pion sur la case cliqu�e
extern int move_pawn_to(SDL_Rect clickedSquare,Square square[][8]);
//Ecrit les d�placements autoris�s
extern void get_authorized_moves(SDL_Rect rect,Square square[][8], int tour);
//G�re les �venements (clic, etc) de la partie
extern void wait_for_event(SDL_Renderer *renderer,Square square[][8],int ia, Game *game);
//Renvoie le gagnant de la partie (noir ou blanc ou aucun)
extern int getWinner(SDL_Renderer *renderer, Square square[][8], Game * game);
//Permet de v�rifier si il y a encore des d�placements possibles, g�re �galement le cas o� il n'y en a pas
extern int isblocked(int color, Game *game, SDL_Renderer *renderer);
//G�re le clic sur le bouton de retour au menu sur l'�cran de victoire
extern void wait_for_click_on_button(void);

//Fonctions g�rant l'affichage en jeu
//Affiche le fond en bois
extern void render_background(SDL_Renderer *renderer);
//Affiche le "sous-plateau" de couleur uni permettant d'obtenir un joli �chiquier
extern void render_base(SDL_Renderer *renderer);
//Affiche les cases de l'�chiquier
extern void render_squares(SDL_Renderer *renderer);
//Affiche les pions de l'�chiquier
extern void render_pawns(SDL_Renderer *renderer,Square square[][8]);
//Affiche les cases vertes et rouges o� un d�placement est possible
extern void render_authorized_moves(SDL_Rect clickedSquare, SDL_Renderer *renderer,Square square[][8]);
//Affiche l'�cran de victoire
extern void render_victory_screen(SDL_Renderer *renderer, int color);
//Affiche le compteur de tour en haut de l'�cran
extern void render_turn(SDL_Renderer *renderer, Game *partie);

//fonction g�rant le fichier de sauvegarde
extern void supprimerPartie(int id);
extern void Save( Game * game );
extern void nouvellePartie(int ia, Game * partie ,int idJB,int idJN);
extern void chargerPartie(int id, Game * game);
extern int listerParties(Game parties[]);
extern int saveSize();
extern void nouveauJoueur(char nom[]);
extern void chargerJoueur(int id, Joueur * joueur);
extern void SaveJoueur( Joueur * joueur );
extern int nbJoueur();
extern int listerJoueur(Joueur joueurs[]);
extern void modifJoueur(int ia, int winner , int njN, int njB);

//Fonctions g�n�rales d'affichage
//Affiche une texture donn�e en une position donn�e
extern int RendTex(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y);
//Charge une texture � partir d'une police, d'un texte, et d'une couleur donn�s
extern SDL_Texture *loadFont_Blended(SDL_Renderer *renderer, TTF_Font *police, char texte[], int red, int green, int blue);
//Charge une texture � partir d'une image donn�e (� partir d'un chemin)
extern SDL_Texture *loadIMG(char path[], SDL_Renderer *renderer);
//Initialise la SDL
extern int init(SDL_Window **window,SDL_Renderer **renderer);
//Affiche un bouton � partir d'un texte et d'un SDL_Rect (position et dimension)
extern void render_button(SDL_Renderer *renderer, char text[], SDL_Rect button);
//Renvoie 1 si le curseur est sur le bouton, 0 sinon
extern int isCursorOnButton(SDL_Rect button);
//Affiche un rectangle plein � partir d'un SDL_Rect (position et dimension) et d'une couleur
extern void renderFillRect(SDL_Renderer *renderer,SDL_Rect rect,int r,int g,int b,int a);
//Affiche un texte centr� dans un rectangle
extern void renderTextInRect(SDL_Renderer *renderer, char text[], SDL_Rect rect);
//Affiche un texte de couleur donn�e centr� sur la position donn�
extern void renderCenteredText(SDL_Renderer *renderer, char text[], int x, int y, int r, int g, int b, int size);


//Fonctions utilis�es par le menu
//Fonction principale du menu
extern void menu(void);
//Affiche les boutons du menu
extern void render_menu_buttons(SDL_Renderer *renderer);
//Affiche l'arri�re plan du menu
extern void render_menu_background(SDL_Renderer *renderer,Square square[][8]);
//Affiche le titre du jeu (Chess Royal)
extern void render_game_title(SDL_Renderer *renderer);
//Attribue al�atoirement � chaque case soit un pion noir ou blanc ou rien
extern void init_random_pawns_pos(Square square[][8]);
//G�re l'�venement du clic dans le menu
extern int event_click(SDL_Renderer *renderer, Square square[][8], Game * partie);

//Fonctions g�rant le menu de s�lection du joueur
//Charge le menu de s�lection du joueur
extern void select_players_menu(SDL_Renderer *renderer, int isAIGame,int *quit, int *IDPlayerB, int *IDPlayerW);
//Affiche la liste de joueurs
extern void render_playersList(SDL_Renderer *renderer, int adder);
//Charge le menu de cr�ation d'un joueur
extern void render_newPlayerMenu(SDL_Renderer *renderer);
//Associe aux deux joueurs de la partie l'ID du joueur choisi dans le menu
extern void selectPlayer(int ID, int selectedPlayerNb, int *IDPlayerB, int *IDPlayerW);

//Fonctions g�rant le menu de chargement d'une partie
//Charge le menu de s�lection d'une partie
extern int load_game_menu_and_get_choice(SDL_Renderer *renderer, Game partiesList[]);
//Charge l'ent�te du tableau des parties
extern void load_menu_header(SDL_Renderer *renderer, int adder);
//Affiche les dalles des parties
extern void render_games_buttons(SDL_Renderer *renderer, int adder);
//Affiche les infos sur les parties
extern void render_games_infos(SDL_Renderer *renderer, Game partiesList[], int adder);
//G�re le choix de l'utilisateur
extern int get_choice(int adder);
//Affiche le bouton pour retourner au menu
extern void render_back_to_menu_button(SDL_Renderer *renderer, int adder);

//Fonctions g�rant le menu des scores
//Charge le menu des scores
extern void scores_menu(SDL_Renderer *renderer);
//Affiche l'ent�te du tableau des scores
extern void renderScoreMenuHeader(SDL_Renderer *renderer, int adder);
//Affiche les dalles des joueurs
extern void renderPlayersTiles(SDL_Renderer *renderer, int adder);
//Affiche les infos des joueurs
extern void renderPlayerInfos(SDL_Renderer *renderer, int adder);


//Fonctions de l'IA
extern int arrAcc(int plt[][8],int x, int y);
extern float poids(int color,Square plt[][8],int frmX,int frmY,int toX,int toY,JoueurIA * jN);
extern int listActions(Possibillite actionlist[] , Square plateau[][8], JoueurIA * jN);
extern int findBestAction(Possibillite *bestAction , Square plateau[][8],JoueurIA * jN);
extern void deplacement(Possibillite dpl, Square plat[][8],int color);
extern void affPlt(int plt[][8]);
