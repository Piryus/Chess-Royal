#include "defs.h"

/* Fonction permettant d'afficher la base se trouvant sous l'échiquier*/
SDL_Rect get_clicked_square(int x, int y);
void jeu(SDL_Window *g_Window, SDL_Renderer *g_Renderer);
void render_base(SDL_Renderer *g_Renderer)
{
    SDL_Texture *bg = NULL;
    SDL_Rect bg_chess = {((WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2) - SPACING, ((WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2) - SPACING, SPACING + (SQUARE_SIZE + SPACING) * CHESS_NB_SQUARE, SPACING + (SQUARE_SIZE + SPACING) * CHESS_NB_SQUARE};
    SDL_Rect square = {0, 0, SQUARE_SIZE, SQUARE_SIZE};
    bg = loadIMG("sprites/wood.jpg", g_Renderer);
    for(int i = 0; i <= (WINDOW_WIDTH / 512); i++)
    {
        for(int j = 0; j <= (WINDOW_HEIGHT / 512); j++)
        {
            RendTex(bg, g_Renderer, 512 * i, 512 * j);
        }
    }
    SDL_SetRenderDrawColor(g_Renderer, 133, 44, 16, 255);
    SDL_RenderFillRect(g_Renderer, &bg_chess);
    SDL_RenderPresent(g_Renderer);
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 0; j <= 7; j++)
        {
            square.y = (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * j;
            square.x = (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * i;
            if(((j + i) % 2 == 0))
            {
                SDL_SetRenderDrawColor(g_Renderer, 209, 139, 71, 255);
                SDL_RenderFillRect(g_Renderer, &square);
            }
            else
            {
                SDL_SetRenderDrawColor(g_Renderer, 255, 206, 158, 255);
                SDL_RenderFillRect(g_Renderer, &square);
            }
        }
    }
    SDL_RenderPresent(g_Renderer);
}
/*Fonction transposant le numéro d'une case en coordonées (x)*/
int numcase_to_coord_x(int numcase)
{
    return (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase;
}
/* Fonction transposant le numéro d'une case en coordonées (y)*/
int numcase_to_coord_y(int numcase)
{
    return (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase;
}
//********************************************************************************************************************
int posx(int numcase)
{
    return (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase + (SQUARE_SIZE - PAWN_SIZE) / 2;
}
//********************************************************************************************************************
int posy(int numcase)
{
    return (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase + (SQUARE_SIZE - PAWN_SIZE) / 2;
}
/* Fonction initialisant la position de départ des pions ainsi que leurs couleurs*/
void init_pawn(pion pion[])
{
    int k = 1;
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 0; j <= 1; j++)
        {
            pion[k].isBlack = 1;
            pion[k].x = i;
            pion[k].y = j;
            pion[k].isInInitPos = 1;
            k++;
        }
    }
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 6; j <= 7; j++)
        {
            pion[k].isBlack = 0;
            pion[k].x = i;
            pion[k].y = j;
            pion[k].isInInitPos = 1;
            k++;
        }
    }
}
//********************************************************************************************************************
void render_pawn(SDL_Renderer *g_Renderer, pion pion[32])
{
    SDL_Texture *pion_noir = NULL, *pion_blanc = NULL;
    pion_noir = loadIMG("sprites/blackpawn.png", g_Renderer);
    pion_blanc = loadIMG("sprites/whitepawn.png", g_Renderer);
    for(int i = 1; i <= 32; i++)
    {
        if(pion[i].isBlack == 0)
        {
            RendTex(pion_blanc, g_Renderer, posx(pion[i].x), posy(pion[i].y));
        }
        if(pion[i].isBlack == 1)
        {
            RendTex(pion_noir, g_Renderer, posx(pion[i].x), posy(pion[i].y));
        }
    }
    SDL_RenderPresent(g_Renderer);
}
//********************************************************************************************************************
void highlight_square(SDL_Rect clickedSquare, SDL_Renderer *g_Renderer, pion pion[32])
{
    int rendersquare = 1;
    SDL_Texture *dot = NULL;
    if((clickedSquare.x!=0)&&(clickedSquare.y!=0))
    {
        for(int k = 1; k <= 32; k++)
        {
            if((clickedSquare.x == numcase_to_coord_x(pion[k].x)) && (clickedSquare.y == numcase_to_coord_y(pion[k].y)))
            {
                printf("Un pion a ete detecte sur la case, selection de la case.\n");
                SDL_SetRenderDrawColor(g_Renderer, 231, 252, 212, 255);
                SDL_RenderFillRect(g_Renderer, &clickedSquare);
                if(pion[k].isInInitPos == 1)   //Si le pion est en positon initiale = 2 cases
                {
                    for(int b = 1; b <= 32; b++)   //on vérifie qu'il n'y est pas de pion devant le pion
                    {
                        if(((numcase_to_coord_x(pion[k].y + 1) == numcase_to_coord_x(pion[b].y))&&(pion[k].isBlack == 1))
                                ||((numcase_to_coord_x(pion[k].y - 1) == numcase_to_coord_x(pion[b].y)) && (pion[k].isBlack == 0)))
                        {
                            printf("Pion detecte devant, aucun mouvement à afficher !\n");
                            rendersquare = 0;
                        }
                    }
                    if(rendersquare == 1)
                    {
                        for(int c = 1; c <= 2; c++)
                        {
                            dot = loadIMG("sprites/dot.png", g_Renderer);
                            RendTex(dot, g_Renderer, posx(pion[k].x), posy(pion[k].y-c+3*pion[k].isBlack));//=+2 si blanc(isBlack=0)/=-2 si noir (isBlack=1)
                            SDL_RenderPresent(g_Renderer);
                        }
                    }

                }
            }
        }

    }
    SDL_RenderPresent(g_Renderer);
}
/* Fonction permettant de renvoyer la case sur laquelle on a cliqué*/
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
void jeu(SDL_Window *g_Window, SDL_Renderer *g_Renderer)
{
    pion pion[32];
    int stop=2;
    SDL_Event event;
    SDL_Rect clickedSquare;
    SDL_RenderClear(g_Renderer);
    render_base(g_Renderer);
    init_pawn(pion); //&pion ?
    render_pawn(g_Renderer, pion);
    //Boucle à vérifier (ne loop pas ?!)
    while(stop!=0)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            printf("quit");
            stop=1;
            break;
        case SDL_KEYDOWN:
            printf("key");
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                stop=1;
                break;
            }
        case SDL_MOUSEBUTTONDOWN:
            printf("mouse");
            if(event.button.button == SDL_BUTTON_LEFT)   // Bouton souris gauche
            {
                clickedSquare=get_clicked_square(event.button.x,event.button.y);
                SDL_RenderClear(g_Renderer);
                render_base(g_Renderer);
                highlight_square(clickedSquare, g_Renderer, pion);
                render_pawn(g_Renderer, pion);
            }
            break;
        }
        printf("fin boucle ");
    }
    printf("après boucle");
}
