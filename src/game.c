#include "defs.h"


// Fonction permettant de renvoyer la case sur laquelle on a cliqu�
SDL_Rect get_clicked_square(int x, int y)
{
    SDL_Rect clickedSquare = {0, 0, SQUARE_SIZE, SQUARE_SIZE};
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 0; j <= 7; j++)
        {
            if((x > numcase_to_coord_x(i))
                    && (x < numcase_to_coord_x(i) + SQUARE_SIZE)
                    && (y > numcase_to_coord_y(j))
                    && (y < numcase_to_coord_y(j) + SQUARE_SIZE))
            {
                clickedSquare.x = numcase_to_coord_x(i);
                clickedSquare.y = numcase_to_coord_y(j);
                printf("Position de la case cliquee acquise : x=%d y=%d !\n", clickedSquare.x, clickedSquare.y);
            }
        }
    }
    return clickedSquare;
}

//Fonction principal g�rant le jeu
void game(SDL_Renderer *renderer, Square square[][8], int ia, Game *partie)
{
    SDL_RenderClear(renderer);
    render_background(renderer);
    render_base(renderer);
    render_squares(renderer);
    render_pawns(renderer, square);
    wait_for_event(renderer, square, ia, partie);
}

void initialize_pawns_pos(Square square[][8])
{
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 0; j <= 7; j++)
        {
            if(j < 2)
            {
                square[i][j].pawn = 1;
            }
            else if(j > 5)
            {
                square[i][j].pawn = -1;
            }
            else
            {
                square[i][j].pawn = 0;
            }
            square[i][j].isSelected = 0;
            square[i][j].isMoveOk = 0;
        }
    }
}

//Fonction transposant le num�ro d'une case en abscisses (de 0 � 8) en abscisse du coin sup�rieur gauche de cette case(x)
int numcase_to_coord_x(int numcase)
{
    return (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase;
}

//Fonction transposant le num�ro d'une case en abscisses (de 0 � 8) en coordon�es du coin sup�rieur gauche de cette case(x)
int numcase_to_coord_y(int numcase)
{
    return (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase;
}

//Fonction transposant le num�ro d'une case en coordon�es pour le pion (x)
int posx(int numcase)
{
    return (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase + (SQUARE_SIZE - PAWN_SIZE) / 2;
}

//Fonction transposant le num�ro d'une case en coordon�es pour le pion (y)
int posy(int numcase)
{
    return (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase + (SQUARE_SIZE - PAWN_SIZE) / 2;
}

int move_pawn_to(SDL_Rect clickedSquare, Square square[][8])
{
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 0; j <= 7; j++)
        {
            if((square[i][j].isMoveOk == 1) && (clickedSquare.x == numcase_to_coord_x(i)) && (clickedSquare.y == numcase_to_coord_y(j)))
            {
                for(int k = 0; k <= 7; k++)
                {
                    for(int l = 0; l <= 7; l++)
                    {
                        if((square[k][l].isSelected == 1))
                        {
                            square[i][j].pawn = square[k][l].pawn;
                            square[k][l].pawn = 0;
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void reset_OK_moves(Square square[][8])
{
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 0; j <= 7; j++)
        {
            square[i][j].isMoveOk = 0;
            square[i][j].isSelected = 0;
        }
    }
}

void get_authorized_moves(SDL_Rect rect, Square square[][8], int tour)
{
    int max_move;
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 0; j <= 7; j++)
        {
            max_move = 1;
            square[i][j].isSelected = 0;
            if((j < 2) || (j > 5))
            {
                max_move = 2;
            }
            if((rect.x == numcase_to_coord_x(i)) //On v�rifie qu'on est sur la case cibl�e (en x)
                    && (rect.y == numcase_to_coord_y(j)) //On v�rifie qu'on est sur la case cibl�e (en y)
                    && (square[i][j].pawn != 0) //On v�rifie qu'il y a un pion sur cette case
                    && (1 - 2 * (tour % 2) == square[i][j].pawn)) //On v�rifie que le tour correspond � la couleur du pion
            {
                if(square[i - 1][j + (square[i][j].pawn)].pawn == square[i][j].pawn * (-1)) //Si il y a un pion sur la diagonale gauche
                {
                    square[i - 1][j + (square[i][j].pawn)].isMoveOk = 1;
                }
                if(square[i + 1][j + (square[i][j].pawn)].pawn == square[i][j].pawn * (-1)) //Si il y a un pion sur la diagonale droite
                {
                    square[i + 1][j + (square[i][j].pawn)].isMoveOk = 1;
                }
                for(int k = 1; k <= max_move; k++)
                {
                    if(square[i][j + (k * square[i][j].pawn)].pawn == 0)
                    {
                        square[i][j + (k * square[i][j].pawn)].isMoveOk = 1;
                    }
                    /*else if(square[i][j+(k*square[i][j].pawn)].pawn!=square[i][j].pawn)
                    {
                        k=max_move;
                    }*/
                }
                square[i][j].isSelected = 1;
            }
        }
    }
}

void wait_for_event(SDL_Renderer *renderer, Square square[][8], int ia, Game *partie)
{
    int stop = 0;
    SDL_Event event;
    SDL_Rect clickedSquare;
    while(!stop)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT://###################################################################################################################################################################
            stop = 1;
            break;
        case SDL_KEYDOWN://###################################################################################################################################################################
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                //menu_ingame(renderer);
                stop = 1;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:// Bouton souris
            if(event.button.button == SDL_BUTTON_LEFT)// Bouton souris gauche
            {
                clickedSquare = get_clicked_square(event.button.x, event.button.y);
                if((clickedSquare.x != 0) && (clickedSquare.y != 0))
                {
                    SDL_RenderClear(renderer);
                    render_background(renderer);
                    render_base(renderer);
                    render_squares(renderer);
                    if(move_pawn_to(clickedSquare, square) == 1)
                    {
                        printf("###########################################\nTour : %d\n", partie->tour);
                        partie->tour = partie->tour + 1;
                        if(ia == 1)
                        {
                            // INIT de l'IA
                            Joueur jN;
                            jN.gamma[0] = 314 ;
                            jN.gamma[1] = 40 ;
                            jN.gamma[2] = 42.5 ;
                            jN.gamma[3] = 47 ;
                            jN.gamma[4] = -133.6 ;
                            jN.gamma[5] = 121 ;
                            jN.gamma[6] = 53 ;
                            //Fonctions d'action de l'ia
                            Possibillite bestAction;
                            findBestAction(&bestAction, square, &jN);
                            deplacement(bestAction, square, _NOIR);
                            partie->tour = partie->tour + 1;
                        }
                        for(int c = 0; c < 8 ; c++)
                        {
                            for(int l = 0; l < 8 ; l++)
                            {
                                partie->plateau[c][l] = square[c][l].pawn;
                            }
                        }
                        Save( partie );
                    }
                    reset_OK_moves(square);
                    get_authorized_moves(clickedSquare, square, partie->tour);
                    render_authorized_moves(clickedSquare, renderer, square);
                    render_pawns(renderer, square);
                    if(getWinner(renderer, square, partie)!=0)
                    {
                        stop=1;
                    }
                }
            }
            break;
        default://Cas par d�faut
            break;
        }
    }
}

int getWinner(SDL_Renderer *renderer, Square square[][8], Game * game)
{
//    int move_counter=0;
int winner=0;
    for(int i = 0; i <= 7; i++)
    {
        if(square[i][7].pawn == _NOIR)
        {
            render_victory_screen(renderer, _NOIR);
            wait_for_click_on_button(renderer);
            game->winner = _NOIR;
            winner=_NOIR;
        }
        if(square[i][0].pawn == _BLANC)
        {
            render_victory_screen(renderer, _BLANC);
            wait_for_click_on_button(renderer);
            game->winner = _BLANC;
            winner=_BLANC;
        }
    }
//On v�rifie qu'un jouer n'est pas bloqu�, si c'est le cas, l'autre joueur l'emporte.
    /*    for(int i = 0; i <= 7; i++)
        {
            for(int j = 0; j <= 7; j++)
            {
              if(square[i][j].isMoveOk==1)
              {
                move_counter++;
              }
            }
        }
        if(move_counter==0)
        {
            if(tour%2==1)
            {
              render_victory_screen(renderer, _BLANC);
            }
            else if(tour%2==0)
            {
                render_victory_screen(renderer, _NOIR);
            }
        }*/
    return winner;
}

void wait_for_click_on_button(SDL_Renderer *renderer)
{
    int stop = 0;
    int cursor_x, cursor_y;
    SDL_Event event;
    while(!stop)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&cursor_x, &cursor_y);
            if((cursor_x > (WINDOW_WIDTH - BUTTON_WIDTH) / 2)
                    && (cursor_x < (WINDOW_WIDTH - BUTTON_WIDTH) / 2 + BUTTON_WIDTH + 100)
                    && (cursor_y > WINDOW_HEIGHT / 1.5)
                    && (cursor_y < WINDOW_HEIGHT / 1.5 + BUTTON_HEIGHT))
            {
                stop = 1;
            }
            break;
        case SDL_QUIT:
            stop=1;
            break;
        }
    }
}

