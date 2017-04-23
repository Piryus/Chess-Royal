#include "defs.h"


// Fonction permettant de renvoyer la case sur laquelle on a cliqué
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

//Fonction principal gérant le jeu
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

//Fonction transposant le numéro d'une case en coordonées pour le pion (y)
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
                        if(square[k][l].isSelected == 1)
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
            if((rect.x == numcase_to_coord_x(i)) //On vérifie qu'on est sur la case ciblée (en x)
                    && (rect.y == numcase_to_coord_y(j)) //On vérifie qu'on est sur la case ciblée (en y)
                    && (square[i][j].pawn != 0) //On vérifie qu'il y a un pion sur cette case
                    && (1 - 2 * (tour % 2) == square[i][j].pawn)) //On vérifie que le tour correspond à la couleur du pion
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
    render_turn(renderer,partie);
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
                if((partie->tour)%2 == 0){
                    if(isblocked(_NOIR,partie,renderer)){stop = 1;}
                }else{
                    if(isblocked(_BLANC,partie,renderer)){stop = 1;}
                }
                clickedSquare = get_clicked_square(event.button.x, event.button.y);
                if((clickedSquare.x != 0) && (clickedSquare.y != 0))
                {
                    SDL_RenderClear(renderer);
                    render_background(renderer);
                    render_base(renderer);
                    render_squares(renderer);
                    if(move_pawn_to(clickedSquare, square) == 1)
                    {
                        printf("<><><><><><><><><><><><><><><><>\nTour : %d\n", partie->tour);
                        partie->tour = partie->tour + 1;
                        if(ia == 1)
                        {
                            // INIT de l'IA
                            JoueurIA jN;
                            jN.gamma[0] = 314 ;
                            jN.gamma[1] = 40 ;
                            jN.gamma[2] = 42 ;
                            jN.gamma[3] = 47 ;
                            jN.gamma[4] = -133 ;
                            jN.gamma[5] = 121 ;
                            jN.gamma[6] = 53 ;
                            //Fonctions d'action de l'ia
                            Possibillite bestAction;
                            if(isblocked(_NOIR,partie,renderer)){stop = 1;}
                            if(findBestAction(&bestAction, square, &jN)){stop = 1;}// plus d'action possible pour l'IA
                            deplacement(bestAction, square, _NOIR);
                            partie->tour = partie->tour + 1;
                        }

                        int nbB = 0;
                        int nbN = 0;
                        for(int c = 0; c < 8 ; c++)
                        {
                            for(int l = 0; l < 8 ; l++)
                            {
                                if(square[c][l].pawn == _BLANC){nbB++;}
                                if(square[c][l].pawn == _NOIR){nbN++;}
                                partie->plateau[c][l] = square[c][l].pawn;
                            }
                        }
                        partie->scoreB = 16-nbN;
                        partie->scoreN = 16-nbB;
                        printf("SCORES:    Blanc %2d  - Noir %2d\n", partie->scoreB , partie->scoreN );
                        if((partie->tour)%2 == 0){
                            if(isblocked(_NOIR,partie,renderer)){stop = 1;}
                        }else{
                            if(isblocked(_BLANC,partie,renderer)){stop = 1;}
                        }

                        Save( partie );
                    }
                    reset_OK_moves(square);
                    get_authorized_moves(clickedSquare, square, partie->tour);
                    render_authorized_moves(clickedSquare, renderer, square);
                    render_pawns(renderer, square);
                    render_turn(renderer,partie);

                    if(getWinner(renderer, square, partie) != 0)
                    {
                        stop = 1;
                    }
                }
            }
            break;
        default://Cas par défaut
            break;
        }
    }
}

int isblocked(int color, Game *game, SDL_Renderer *renderer){
    int blocked = 1;
    if(color == _NOIR){
        for(int c = 0 ; c <8 ; c++){
            for(int l = 0 ; l <8 ; l++){
                if(game->plateau[c][l] == _NOIR){
                    if(l+1<8)if(game->plateau[c][l+1] == _VIDE){
                        blocked = 0;
                    }
                    if((c-1>=0)&(l+1<8))if(game->plateau[c-1][l+1] == _BLANC){
                        blocked = 0;
                    }
                    if((c+1<8)&(l+1<8))if(game->plateau[c+1][l+1] == _BLANC){
                        blocked = 0;
                    }
                }
            }
            if((game->plateau[c][0] == _NOIR)&(game->plateau[c][2] == _VIDE)){
                blocked = 0;
            }
            if((game->plateau[c][1] == _NOIR)&(game->plateau[c][3] == _VIDE)){
                blocked = 0;
            }
        }
    }else{
        for(int c = 0 ; c <8 ; c++){
            for(int l = 0 ; l <8 ; l++){
                if(game->plateau[c][l] == _BLANC){
                    if(l-1>=0)if(game->plateau[c][l-1] == _VIDE){
                        blocked = 0;
                    }
                    if((c-1>=0)&(l-1>=0))if(game->plateau[c-1][l-1] == _NOIR){
                        blocked = 0;
                    }
                    if((c+1<8)&(l-1>=0))if(game->plateau[c+1][l-1] == _NOIR){
                        blocked = 0;
                    }
                }
            }
            if((game->plateau[c][6] == _BLANC)&(game->plateau[c][4] == _VIDE)){
                blocked = 0;
            }
            if((game->plateau[c][7] == _BLANC)&(game->plateau[c][5] == _VIDE)){
                blocked = 0;
            }
        }
    }
    printf("(color : %d)Blocked : %d\n",color,blocked);

     if(blocked == 1){
        if(game->scoreB>game->scoreN){
            modifJoueur(game->ia, _BLANC ,game->joueurN,game->joueurB);
            render_victory_screen(renderer, _BLANC);
            wait_for_click_on_button(renderer);
            supprimerPartie(game->id);
        }else{
            if(game->scoreB<game->scoreN){
                modifJoueur(game->ia, _NOIR ,game->joueurN,game->joueurB);
                render_victory_screen(renderer, _NOIR);
                wait_for_click_on_button(renderer);
                supprimerPartie(game->id);
            }else{
            /// Egalité !
                modifJoueur(game->ia, _VIDE ,game->joueurN,game->joueurB);
                ///mettre un renderer ici
                supprimerPartie(game->id);
            }
        }
    }
    return blocked;
}

int getWinner(SDL_Renderer *renderer, Square square[][8], Game *game){
//    int move_counter=0;
    int winner = 0;
    for(int i = 0; i <= 7; i++){
        if(square[i][7].pawn == _NOIR){
            modifJoueur(game->ia, _NOIR ,game->joueurN,game->joueurB);
            render_victory_screen(renderer, _NOIR);
            wait_for_click_on_button(renderer);
            game->winner = _NOIR;
            winner = _NOIR;
            supprimerPartie(game->id);
        }
        if(square[i][0].pawn == _BLANC){
            modifJoueur(game->ia, _BLANC ,game->joueurN,game->joueurB);
            render_victory_screen(renderer, _BLANC);
            wait_for_click_on_button(renderer);
            game->winner = _BLANC;
            winner = _BLANC;
            supprimerPartie(game->id);
        }
    }
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
            stop = 1;
            break;
        }
    }
}

