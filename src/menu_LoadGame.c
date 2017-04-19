#include "defs.h"

void load_game_menu(SDL_Renderer *renderer, Game partiesList[saveSize()])
{
    Square square[8][8];
    int stop = 0;
    SDL_Event event;
    SDL_RenderClear(renderer);
    render_menu_background(renderer, square);
    load_menu_header(renderer);
    render_games_buttons(renderer);
    render_games_infos(renderer,partiesList);

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
    while(!stop)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                stop = 1;
                break;
            }
        }
    }
}

void load_menu_header(SDL_Renderer *renderer)
{
    SDL_Texture *header;
    SDL_Rect font_rect;
    char text[5][100] = {"ID", "IA", "Score B", "Score N", "Tour"};
    TTF_Font *font_OpenSans = NULL;
    font_OpenSans = TTF_OpenFont("ttf/OpenSans-Regular.ttf", 30);
    for(int i = 0; i <= 4; i++)
    {
        header = loadFont_Blended(renderer, font_OpenSans, text[i], 39, 174, 96);
        SDL_QueryTexture(header, NULL, NULL, &font_rect.w, &font_rect.h);
        RendTex(header, renderer, (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * (i + 1), (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2);
        SDL_RenderPresent(renderer);
    }
}

void render_games_buttons(SDL_Renderer *renderer)
{
    SDL_Rect game = {(WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2,
                     (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 50,
                     WINDOW_WIDTH / 1.1,
                     100
                    };
    int size = saveSize();
    printf("%d", size);
    SDL_SetRenderDrawColor(renderer, 230, 126, 34, 255);
    for(int i = 0; i <= size; i++)
    {
        SDL_RenderFillRect(renderer, &game);
        game.y=game.y+game.h+10;
    }
    SDL_RenderPresent(renderer);
}


void render_games_infos(SDL_Renderer *renderer, Game partiesList[saveSize()])
{
    SDL_Texture *text;
    char data[5][100];
    SDL_Rect font_rect;
    TTF_Font *font_OpenSans = NULL;
    font_OpenSans = TTF_OpenFont("ttf/OpenSans-Regular.ttf", 30);
    for(int i = 0; i <= saveSize(); i++)
    {
        strcpy(data[i],partiesList[i].id);
        text = loadFont_Blended(renderer, font_OpenSans, data[i], 39, 174, 96);
        SDL_QueryTexture(text, NULL, NULL, &font_rect.w, &font_rect.h);
        RendTex(text, renderer, (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * (i + 1), (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2);
        SDL_RenderPresent(renderer);
    }
}
