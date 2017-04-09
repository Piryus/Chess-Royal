#include "defs.h"

struct square square[8][8];

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

Example : X = square[2][5]
*/

/* Fonction permettant d'afficher la base se trouvant sous l'échiquier*/
void render_base(SDL_Renderer *renderer)
{
    SDL_Rect bg_chess = {((WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2) - SPACING, ((WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2) - SPACING, SPACING + (SQUARE_SIZE + SPACING) * CHESS_NB_SQUARE, SPACING + (SQUARE_SIZE + SPACING) * CHESS_NB_SQUARE};
    SDL_SetRenderDrawColor(renderer, 133, 44, 16, 255);
    SDL_RenderFillRect(renderer, &bg_chess);
    SDL_RenderPresent(renderer);
}
void render_background(SDL_Renderer *renderer)
{
    SDL_Texture *bg = NULL;
    bg = loadIMG("sprites/wood.jpg", renderer);
    for(int i = 0; i <= (WINDOW_WIDTH / 512); i++)
    {
        for(int j = 0; j <= (WINDOW_HEIGHT / 512); j++)
        {
            RendTex(bg, renderer, 512 * i, 512 * j);
        }
    }
    SDL_RenderPresent(renderer);

}
void render_squares(SDL_Renderer *renderer)
{
    SDL_Rect square_rect = {0, 0, SQUARE_SIZE, SQUARE_SIZE};
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 0; j <= 7; j++)
        {
            square_rect.y = (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * j;
            square_rect.x = (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * i;
            if(((j + i) % 2 == 0))
            {
                SDL_SetRenderDrawColor(renderer, 209, 139, 71, 255);
                SDL_RenderFillRect(renderer, &square_rect);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 255, 206, 158, 255);
                SDL_RenderFillRect(renderer, &square_rect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

//********************************************************************************************************************
void show_possible_moves(SDL_Rect clickedSquare, SDL_Renderer *renderer)
{
    int max_move=1;
    SDL_Texture *dot = NULL;
    if((clickedSquare.x!=0)&&(clickedSquare.y!=0))
    {
        for(int i=0; i<=7; i++)
        {
            for(int j=0; j<=7; j++)
            {
                square[i][j].isSelected=0;
                if((j<2)||(j>7))
                {
                    max_move=2;
                }
                if((clickedSquare.x==numcase_to_coord_x(i))
                        &&(clickedSquare.y==numcase_to_coord_y(j))
                        &&(square[i][j].pawn!=0))
                {
                    SDL_SetRenderDrawColor(renderer, 231, 252, 212, 255);
                    SDL_RenderFillRect(renderer, &clickedSquare);
                    for(int k = 1; k <= max_move; k++)
                    {
                        if(square[i][j+(k*square[i][j].pawn)].pawn==0)
                        {
                            dot = loadIMG("sprites/dot.png", renderer);
                            RendTex(dot, renderer, posx(i), posy(j+(k*square[i][j].pawn)));
                            SDL_RenderPresent(renderer);
                            square[i][j+(k*square[i][j].pawn)].isMoveOk=1;
                        }
                    }
                    square[i][j].isSelected=1;
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}
// Fonction permettant de renvoyer la case sur laquelle on a cliqué
SDL_Rect get_clicked_square(int x, int y)
{
    SDL_Rect square = {0, 0, SQUARE_SIZE, SQUARE_SIZE};
    for(int i=0; i<=7; i++)
    {
        for(int j=0; j<=7; j++)
        {
            if((x > (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING)*i)
                    && (x < (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING)*i + SQUARE_SIZE)
                    && (y > (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING)*j)
                    && (y < (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING)*j + SQUARE_SIZE))
            {
                square.x=(WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * i;
                square.y=(WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * j;
                printf("Position de la case cliquee acquise : x=%d y=%d !\n",square.x,square.y);
            }
        }
    }
    return square;
}
//********************************************************************************************************************
void game(SDL_Renderer *renderer)
{
    /*int isMovesShown;
    pion pion[32];*/
    int stop=0;
    SDL_Event event;
    SDL_Rect clickedSquare;
    SDL_RenderClear(renderer);
    render_background(renderer);
    render_base(renderer);
    render_squares(renderer);
    initialize_pawns_pos();
    render_pawns(renderer);
    while(stop!=1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            stop=1;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                stop=1;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT)   // Bouton souris gauche
            {
                clickedSquare=get_clicked_square(event.button.x,event.button.y);
                SDL_RenderClear(renderer);
                render_background(renderer);
                render_base(renderer);
                render_squares(renderer);
                move_pawn_to_clicked_square(clickedSquare);
                show_possible_moves(clickedSquare,renderer);
                render_pawns(renderer);

            }
            break;
        default:
            break;
        }
    }
}

void initialize_pawns_pos(void)
{
    for(int i=0; i<=7; i++)
    {
        for(int j=0; j<=7; j++)
        {
            if(j<2)
            {
                square[i][j].pawn=1;
            }
            else if(j>5)
            {
                square[i][j].pawn=-1;
            }
            else
            {
                square[i][j].pawn=0;
            }
            square[i][j].isSelected=0;
            square[i][j].isMoveOk=0;
        }
    }
}

void render_pawns(SDL_Renderer *renderer)
{
    SDL_Texture *pion_noir = NULL, *pion_blanc = NULL;
    pion_noir = loadIMG("sprites/blackpawn.png", renderer);
    pion_blanc = loadIMG("sprites/whitepawn.png", renderer);
    for(int i = 0; i<=7; i++)
    {
        for(int j=0; j<=7; j++)
        {
            if(square[i][j].pawn==-1)
            {
                RendTex(pion_blanc, renderer, posx(i), posy(j));
            }
            else if(square[i][j].pawn==1)
            {
                RendTex(pion_noir, renderer, posx(i), posy(j));
            }
        }
    }
    SDL_RenderPresent(renderer);
}

//Fonction transposant le numéro d'une case en abscisses (de 0 à 8) en abscisse du coin supérieur gauche de cette case(x)
int numcase_to_coord_x(int numcase)
{
    return (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase;
}
//Fonction transposant le numéro d'une case en abscisses (de 0 à 8) en coordonées du coin supérieur gauche de cette case(x)
int numcase_to_coord_y(int numcase)
{
    return (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase;
}
//Fonction transposant le numéro d'une case en coordonées pour le pion (x)
int posx(int numcase)
{
    return (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase + (SQUARE_SIZE - PAWN_SIZE) / 2;
}
//********************************************************************************************************************
int posy(int numcase)
{
    return (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase + (SQUARE_SIZE - PAWN_SIZE) / 2;
}

void move_pawn_to_clicked_square(SDL_Rect clickedSquare)
{
    for(int i=0; i<=7; i++)
    {
        for(int j=0; j<=7; j++)
        {
            if((square[i][j].isMoveOk==1)&&(clickedSquare.x==numcase_to_coord_x(i))&&(clickedSquare.y==numcase_to_coord_y(j)))
            {
                for(int k=0; k<=7; k++)
                {
                    for(int l=0; l<=7; l++)
                    {
                        if((square[k][l].isSelected==1))
                        {
                        square[i][j].pawn=square[k][l].pawn;
                        square[k][l].pawn=0;
                        }

                    }
                }
            }
        }
    }
}
