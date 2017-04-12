#include "defs.h"


// Fonction permettant de renvoyer la case sur laquelle on a cliqu�
SDL_Rect get_clicked_square(int x, int y)
{
    SDL_Rect clickedSquare = {0, 0, SQUARE_SIZE, SQUARE_SIZE};
    for(int i=0; i<=7; i++)
    {
        for(int j=0; j<=7; j++)
        {
            if((x > (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING)*i)
                    && (x < (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING)*i + SQUARE_SIZE)
                    && (y > (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING)*j)
                    && (y < (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING)*j + SQUARE_SIZE))
            {
                clickedSquare.x=(WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * i;
                clickedSquare.y=(WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * j;
                printf("Position de la case cliquee acquise : x=%d y=%d !\n",clickedSquare.x,clickedSquare.y);
            }
        }
    }
    return clickedSquare;
}
//********************************************************************************************************************
void game(SDL_Renderer *renderer,Square square[][8])
{
    SDL_RenderClear(renderer);
    render_background(renderer);
    render_base(renderer);
    render_squares(renderer);
    initialize_pawns_pos(square);
    render_pawns(renderer,square);
    wait_for_event(renderer,square);
}

void initialize_pawns_pos(Square square[][8])
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
//********************************************************************************************************************
int posy(int numcase)
{
    return (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase + (SQUARE_SIZE - PAWN_SIZE) / 2;
}

int move_pawn_to(SDL_Rect clickedSquare,Square square[][8])
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
                            return square[i][j].pawn;
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
    for(int i=0; i<=7; i++)
    {
        for(int j=0; j<=7; j++)
        {
            square[i][j].isMoveOk=0;
            square[i][j].isSelected=0;
        }
    }
}

void get_authorized_moves(SDL_Rect rect,Square square[][8], int tour)
{
    int max_move;
    for(int i=0; i<=7; i++)
    {
        for(int j=0; j<=7; j++)
        {
            max_move=1;
            square[i][j].isSelected=0;
            if((j<2)||(j>5))
            {
                max_move=2;
            }
            if((rect.x==numcase_to_coord_x(i))&&(rect.y==numcase_to_coord_y(j))&&(square[i][j].pawn!=0)&&(1-2*(tour%2) == square[i][j].pawn))
            {
                if(square[i-1][j+(square[i][j].pawn)].pawn==square[i][j].pawn*(-1))
                    square[i-1][j+(square[i][j].pawn)].isMoveOk=1;
                if(square[i+1][j+(square[i][j].pawn)].pawn==square[i][j].pawn*(-1))
                    square[i+1][j+(square[i][j].pawn)].isMoveOk=1;

                for(int k = 1; k <= max_move; k++)
                {
                    if(square[i][j+(k*square[i][j].pawn)].pawn==0)
                        square[i][j+(k*square[i][j].pawn)].isMoveOk=1;
                    /*else if(square[i][j+(k*square[i][j].pawn)].pawn!=square[i][j].pawn)
                    {
                        k=max_move;
                    }*/
                }
                square[i][j].isSelected=1;
            }
        }
    }
}

void wait_for_event(SDL_Renderer *renderer,Square square[][8])
{
    int stop=0;
    int ia = 0;
    int tour = 1;
    SDL_Event event;
    SDL_Rect clickedSquare;
    int move_ok=0;
    while(!stop)
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
                if((clickedSquare.x!=0)&&(clickedSquare.y!=0))
                {
                    SDL_RenderClear(renderer);
                    render_background(renderer);
                    render_base(renderer);
                    render_squares(renderer);
                    if(move_pawn_to(clickedSquare,square) == 1-(tour%2)*2){
                        printf("tour : %d\n", tour);
                        tour ++;
                        for(int i=0; i<=7; i++){
                            if(square[i][7].pawn == 1){ printf("winner noir\n");}
                            if(square[i][0].pawn == -1){ printf("winner blanc\n");}
                        }
                    }
                    reset_OK_moves(square);
                    get_authorized_moves(clickedSquare,square,tour);
                    render_authorized_moves(clickedSquare,renderer,square);
                    render_pawns(renderer,square);
                }
            }
            break;
        default:
            break;
        }
    }
}

