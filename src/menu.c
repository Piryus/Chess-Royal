#include "defs.h"

void menu(void)
{
    Square square[8][8];
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;
    int continuer = 1;
    init(&window, &renderer);
    render_menu_background(renderer, square);
    render_game_title(renderer);
    render_menu_buttons(renderer);
    Game partie;
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event_click(renderer, square, &partie))
            {
                continuer = 0;
            }
            break;
        case SDL_MOUSEMOTION:
            //select_button(renderer);
            break;
        default:
            break;
        }
    }
}

void render_button(SDL_Renderer *renderer, char text[], int x, int y, int w, int h)
{
    SDL_Rect button = {x, y, w, h};
    SDL_Rect button_outline = {0, 0, 0, 0};
    int outline = 1;
    SDL_Texture *button_text = NULL;
    SDL_Rect font_rect;
    TTF_Font *font_OpenSans = NULL;
    SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255);//Couleur des boutons (intérieur)
    SDL_RenderFillRect(renderer, &button);
    SDL_SetRenderDrawColor(renderer, 41, 128, 185, 255);//Couleur des boutons (contour)
    while(outline < BUTTON_OUTLINE)
    {
        for(int i = 0; i < (NB_MENU_BUTTONS); i++)
        {
            button_outline.w = w + 2 * outline;
            button_outline.h = h + 2 * outline;
            button_outline.x = w - outline;
            button_outline.y = h - outline;
            SDL_RenderDrawRect(renderer, &button_outline);
        }
        outline++;
    }
    SDL_RenderPresent(renderer);
    font_OpenSans = TTF_OpenFont("ttf/OpenSans-Regular.ttf", 40);
    button_text = loadFont_Blended(renderer, font_OpenSans, texte, 236, 240, 241);
    SDL_QueryTexture(button_text, NULL, NULL, &font_rect.w, &font_rect.h);
    RendTex(button_text, renderer, x - font_rect.w/ 2, y - font_rect.h/2);
    SDL_RenderPresent(renderer);
}

void render_menu_buttons(SDL_Renderer *renderer)
{
    SDL_Rect menu_tile = {0, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect menu_tile_outline = {0, 0, 0, 0};
    SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255);//Couleur des boutons (intérieur)
    int outline = 1;
    for(int i = 0; i < (NB_MENU_BUTTONS); i++)
    {
        menu_tile.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
        menu_tile.y = WINDOW_HEIGHT / 4 + (BUTTON_HEIGHT + MENU_BUTTON_SPACING) * i;
        SDL_RenderFillRect(renderer, &menu_tile);
    }
    SDL_SetRenderDrawColor(renderer, 41, 128, 185, 255);//Couleur des boutons (contour)
    while(outline < BUTTON_OUTLINE)
    {
        for(int i = 0; i < (NB_MENU_BUTTONS); i++)
        {
            menu_tile_outline.w = BUTTON_WIDTH + 2 * outline;
            menu_tile_outline.h = BUTTON_HEIGHT + 2 * outline;
            menu_tile_outline.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2 - outline;
            menu_tile_outline.y = WINDOW_HEIGHT / 4 + (BUTTON_HEIGHT + MENU_BUTTON_SPACING) * i - outline;
            SDL_RenderDrawRect(renderer, &menu_tile_outline);
        }
        outline++;
    }
    SDL_RenderPresent(renderer);
    render_menu_button_text(renderer, 0, "Solo contre l'IA");
    render_menu_button_text(renderer, 1, "Jouer sur le même PC");
    render_menu_button_text(renderer, 2, "Charger une partie");
    render_menu_button_text(renderer, 3, "Scores");
    render_menu_button_text(renderer, 4, "Quitter le jeu");
}

void render_menu_button_text(SDL_Renderer *renderer, int menu_button_id, char texte[])
{
    SDL_Texture *button_text = NULL;
    SDL_Rect font_rect;
    TTF_Font *font_OpenSans = NULL;
    font_OpenSans = TTF_OpenFont("ttf/OpenSans-Regular.ttf", 40);
    button_text = loadFont_Blended(renderer, font_OpenSans, texte, 236, 240, 241);
    SDL_QueryTexture(button_text, NULL, NULL, &font_rect.w, &font_rect.h);
    RendTex(button_text, renderer, (WINDOW_WIDTH - font_rect.w) / 2, WINDOW_HEIGHT / 4 + (BUTTON_HEIGHT - font_rect.h) / 2 + (BUTTON_HEIGHT + MENU_BUTTON_SPACING) * menu_button_id);
    SDL_RenderPresent(renderer);
}

void render_menu_background(SDL_Renderer *renderer, Square square[][8])
{
    SDL_Rect background = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    render_background(renderer);
    render_base(renderer);
    render_squares(renderer);
    init_random_pawns_pos(square);
    render_pawns(renderer, square);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &background);
    SDL_RenderPresent(renderer);
}

void render_game_title(SDL_Renderer *renderer)
{
    SDL_Texture *title = NULL;
    SDL_Rect font_rect;
    TTF_Font *font_fipps = NULL;
    font_fipps = TTF_OpenFont("ttf/Fipps-Regular.ttf", 60);
    title = loadFont_Blended(renderer, font_fipps, "Chess Royal", 255, 144, 0);
    SDL_QueryTexture(title, NULL, NULL, &font_rect.w, &font_rect.h);
    RendTex(title, renderer, (WINDOW_WIDTH - font_rect.w) / 2, (WINDOW_HEIGHT - font_rect.h) / 40);
    SDL_RenderPresent(renderer);
}

void init_random_pawns_pos(Square square[][8])
{
    srand(time(NULL));
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 0; j <= 7; j++)
        {
            square[i][j].pawn = rand() % 3 - 1;
        }
    }
}

/*WIP
int select_button(SDL_Renderer *renderer)
{
    int cursor_x,cursor_y;
    //SDL_Rect menu_tile = {0, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_GetMouseState(&cursor_x,&cursor_y);
    int isInside=0;
    for(int i = 0; i < (NB_MENU_BUTTONS); i++)
    {
        if((cursor_x>(WINDOW_WIDTH - BUTTON_WIDTH) / 2)
           &&(cursor_y>WINDOW_HEIGHT / 4 + (BUTTON_HEIGHT + MENU_BUTTON_SPACING) * i)
           &&(cursor_x<(WINDOW_WIDTH - BUTTON_WIDTH) / 2 + BUTTON_WIDTH)
           &&(cursor_y<WINDOW_HEIGHT / 4 + (BUTTON_HEIGHT + MENU_BUTTON_SPACING) * i + BUTTON_HEIGHT))
        {
        //SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);//Couleur du bouton sélectionné (intérieur)
        //menu_tile.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
        //menu_tile.y = WINDOW_HEIGHT / 4 + (BUTTON_HEIGHT + MENU_BUTTON_SPACING) * i;
        //SDL_RenderFillRect(renderer, &menu_tile);
        isInside=1;
        }
    }
    return isInside;
    //SDL_RenderPresent(renderer);
}*/

int event_click(SDL_Renderer *renderer, Square square[][8], Game * partie)
{
    int quit = 0;
    int cursor_x, cursor_y;
    SDL_GetMouseState(&cursor_x, &cursor_y);
    Game partiesList[saveSize()];
    for(int i = 0; i < (NB_MENU_BUTTONS); i++)
    {
        if((cursor_x > (WINDOW_WIDTH - BUTTON_WIDTH) / 2)
                && (cursor_y > WINDOW_HEIGHT / 4 + (BUTTON_HEIGHT + MENU_BUTTON_SPACING) * i)
                && (cursor_x < (WINDOW_WIDTH - BUTTON_WIDTH) / 2 + BUTTON_WIDTH)
                && (cursor_y < WINDOW_HEIGHT / 4 + (BUTTON_HEIGHT + MENU_BUTTON_SPACING) * i + BUTTON_HEIGHT))
        {
            switch(i)
            {
            case 0://==================================    IA
                SDL_RenderClear(renderer);
                nouvellePartie(IA, partie);
                initialize_pawns_pos(square);
                game(renderer, square, IA, partie);
                quit = 1;
                break;
            case 1: //==================================    Non IA
                SDL_RenderClear(renderer);
                nouvellePartie(NO_IA, partie);
                initialize_pawns_pos(square);
                game(renderer, square, NO_IA, partie);
                quit = 1;
                break;
            case 2://==================================    Charger partie
                SDL_RenderClear(renderer);
                listerParties(partiesList);
                // Afficher le tableau partieList
                /* pour rappel :
                    struct Game :
                        int id;
                        int ia;
                        int scoreB;
                        int scoreN;
                        int winner;
                        int tour;
                        int plateau[8][8];
                */
                int choice;
                choice = 1; //  faire un moyen de selectionner la partie     !!! ( range : [1;size] )
                chargerPartie(choice, partie);
                for(int c = 0; c < 8 ; c++)
                {
                    for(int l = 0; l < 8 ; l++)
                    {
                        square[c][l].pawn = partie->plateau[c][l];
                        square[c][l].isSelected = 0;
                        square[c][l].isMoveOk = 0;
                    }
                }
                game(renderer, square, partie->ia, partie);
                quit = 1;
                break;
            case 3://==================================    Scores
                SDL_RenderClear(renderer);
                listerParties(partiesList);
                // Afficher le tableau partieList
                break;
            case 4://Quitter
                quit = 1;
                break;
            default:
                break;
            }
        }
    }
    return quit;
}

