#include "defs.h"

void menu(void)
{
    Square square[8][8];
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;
    int continuer = 1;
    int quit;
    Game partie;
    init(&window, &renderer);
    render_menu_background(renderer, square);
    render_game_title(renderer);
    render_menu_buttons(renderer);
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;

        case SDL_MOUSEBUTTONDOWN:
            quit = event_click(renderer, square, &partie);
            if(quit == 1)
            {
                continuer = 0;
            }
            else if(quit == -1)
            {
                SDL_RenderClear(renderer);
                render_menu_background(renderer, square);
                render_game_title(renderer);
                render_menu_buttons(renderer);
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

void render_menu_buttons(SDL_Renderer *renderer)
{
    SDL_Rect menu_button[NB_MENU_BUTTONS];
    for(int i = 0; i < (NB_MENU_BUTTONS); i++)
    {
        menu_button[i].w = BUTTON_WIDTH;
        menu_button[i].h = BUTTON_HEIGHT;
        menu_button[i].x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
        menu_button[i].y = WINDOW_HEIGHT / 4 + (BUTTON_HEIGHT + MENU_BUTTON_SPACING) * i;
    }
    render_button(renderer, "Jouer contre l'IA", menu_button[0]);
    render_button(renderer, "Jouer à deux", menu_button[1]);
    render_button(renderer, "Charger une partie", menu_button[2]);
    //render_button(renderer, "Scores", menu_button[3]);
    render_button(renderer, "Quitter le jeu", menu_button[3]);
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
    int choice = 1;
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
                quit = -1;
                break;
            case 1: //==================================    Non IA
                SDL_RenderClear(renderer);
                nouvellePartie(NO_IA, partie);
                initialize_pawns_pos(square);
                game(renderer, square, NO_IA, partie);
                quit = -1;
                break;
            case 2://==================================    Charger partie
                SDL_RenderClear(renderer);
                listerParties(partiesList);
                choice = load_game_menu_and_get_choice(renderer, partiesList);
                printf("%d",choice);
                if(choice >= 0)
                {
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
                }
                else
                {
                    quit = -1;
                }
                break;
                break;
            case 3://Quitter
                quit = 1;
                break;
            default:
                break;
            }
        }
    }
    return quit;
}

