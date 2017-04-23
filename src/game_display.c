#include "defs.h"
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
//********************************************************************************************************************
void render_authorized_moves(SDL_Rect clickedSquare, SDL_Renderer *renderer, Square square[][8])
{
    SDL_Rect square_rect = {0, 0, SQUARE_SIZE, SQUARE_SIZE};
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 0; j <= 7; j++)
        {
            if(square[i][j].isSelected == 1) //Si une case est sélectionnée
            {
                SDL_SetRenderDrawColor(renderer, 231, 252, 212, 255);
                SDL_RenderFillRect(renderer, &clickedSquare);
            }
            if(square[i][j].isMoveOk == 1) //Si le déplacement est autorisé sur une case
            {
                square_rect.x = numcase_to_coord_x(i);
                square_rect.y = numcase_to_coord_y(j);
                if(square[i][j].pawn != 0) //Si la case comporte un pion, couleur : rouge
                {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &square_rect);
                }
                else //Si la case ne comporte pas de pion, couleur : verte
                {
                    SDL_SetRenderDrawColor(renderer, 80, 160, 80, 255);
                    SDL_RenderFillRect(renderer, &square_rect);
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void render_pawns(SDL_Renderer *renderer, Square square[][8])
{
    SDL_Texture *pion_noir = NULL, *pion_blanc = NULL;
    pion_noir = loadIMG("sprites/blackpawn.png", renderer);
    pion_blanc = loadIMG("sprites/whitepawn.png", renderer);
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 0; j <= 7; j++)
        {
            if(square[i][j].pawn == -1)
            {
                RendTex(pion_blanc, renderer, posx(i), posy(j));
            }
            else if(square[i][j].pawn == 1)
            {
                RendTex(pion_noir, renderer, posx(i), posy(j));
            }
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

/* Fonction permettant d'afficher la base se trouvant sous l'échiquier*/
void render_base(SDL_Renderer *renderer)
{
    SDL_Rect bg_chess = {((WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2) - SPACING,
                         ((WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2) - SPACING,
                         SPACING + (SQUARE_SIZE + SPACING) * CHESS_NB_SQUARE,
                         SPACING + (SQUARE_SIZE + SPACING) * CHESS_NB_SQUARE
                        };
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


void render_victory_screen(SDL_Renderer *renderer, int color)
{
    SDL_Rect background = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_Texture *victory_msg = NULL;
    SDL_Rect font_rect;
    TTF_Font *font_code_light = NULL;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &background);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    font_code_light = TTF_OpenFont("ttf/CODE_Light.ttf", 80);
    if(color == _NOIR)
    {
        victory_msg = loadFont_Blended(renderer, font_code_light, "Le joueur Noir remporte la partie !", 255, 255, 255);
    }
    else if(color == _BLANC)
    {
        victory_msg = loadFont_Blended(renderer, font_code_light, "Le joueur Blanc remporte la partie !", 255, 255, 255);
    }
    else if(color == _VIDE)
    {
        victory_msg = loadFont_Blended(renderer, font_code_light, "Egalité !", 255, 255, 255);
    }
    SDL_QueryTexture(victory_msg, NULL, NULL, &font_rect.w, &font_rect.h);
    RendTex(victory_msg, renderer, (WINDOW_WIDTH - font_rect.w) / 2, (WINDOW_HEIGHT - font_rect.h) / 2);
    SDL_Rect button = {(WINDOW_WIDTH - BUTTON_WIDTH) / 2, WINDOW_HEIGHT / 1.5, BUTTON_WIDTH + 100, BUTTON_HEIGHT};
    render_button(renderer, "Retour au menu principal", button);
    SDL_RenderPresent(renderer);
}

void render_turn(SDL_Renderer *renderer, Game *partie)
{
    SDL_Texture *turn;
    SDL_Rect font_rect;
    char text[20];
    char color[10];
    if(partie->tour%2 == 1)
    {
        strcpy(color, "Blanc");
    }
    else
    {
        strcpy(color, "Noir");
    }
    sprintf(text, "Tour : %d (%s)", partie->tour, color);
    TTF_Font *font_OpenSans = TTF_OpenFont("ttf/OpenSans-Regular.ttf", 40);
    turn = loadFont_Blended(renderer, font_OpenSans, text, 236, 240, 241);
    SDL_QueryTexture(turn, NULL, NULL, &font_rect.w, &font_rect.h);
    RendTex(turn, renderer, (WINDOW_WIDTH - font_rect.w) / 2, 30);
    SDL_RenderPresent(renderer);
}
