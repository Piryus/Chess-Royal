#include "defs.h"

int load_game_menu_and_get_choice(SDL_Renderer *renderer, Game partiesList[saveSize()])
{
    Square square[8][8];
    int choice=-1;
    int stop = 0;
    SDL_Event event;
    SDL_RenderClear(renderer);
    render_menu_background(renderer, square);
    load_menu_header(renderer);
    render_games_buttons(renderer);
    render_games_infos(renderer, partiesList);
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
        case SDL_MOUSEBUTTONDOWN:
            choice=get_choice(renderer);
            if(choice!= -1)
            {
                stop = 1;
            }
            break;
        }
    }
    return choice;
}

void load_menu_header(SDL_Renderer *renderer)
{
    SDL_Texture *header;
    SDL_Rect font_rect;
    char text[5][100] = {"ID", "IA", "Score Blanc", "Score Noir", "Tour"};
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
    for(int i = 0; i < size; i++)
    {
        SDL_RenderFillRect(renderer, &game);
        game.y = game.y + game.h + 10;
    }
    SDL_RenderPresent(renderer);
}


void render_games_infos(SDL_Renderer *renderer, Game partiesList[saveSize()])
{
    int size = saveSize();
    char ID[size][20];
    char isIA[size][20];
    char scoreB[size][20];
    char scoreW[size][20];
    char turn[size][20];
    SDL_Rect font_rect;
    SDL_Texture *text;
    TTF_Font *font_OpenSans = TTF_OpenFont("ttf/OpenSans-Regular.ttf", 30);
    SDL_Rect go_button = {(WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * 6, NULL, 70, 70};
    for(int i = 0; i <= size; i++)
    {
        sprintf(ID[i], "%d", partiesList[i].id);
        if(partiesList[i].id == 1)
        {
            strcpy(isIA[i], "Oui");
        }
        else
        {
            strcpy(isIA[i], "Non");
        }
        sprintf(turn[i], "%d", partiesList[i].tour);
        sprintf(scoreB[i], "%d", partiesList[i].scoreN);
        sprintf(scoreW[i], "%d", partiesList[i].scoreB);
    }
    for(int j = 0; j < size; j++)
    {
        text = loadFont_Blended(renderer, font_OpenSans, ID[j], 142, 68, 173);
        SDL_QueryTexture(text, NULL, NULL, &font_rect.w, &font_rect.h);
        RendTex(text, renderer, (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200, (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 110 * (j + 1) - 30);
        text = loadFont_Blended(renderer, font_OpenSans, isIA[j], 142, 68, 173);
        SDL_QueryTexture(text, NULL, NULL, &font_rect.w, &font_rect.h);
        RendTex(text, renderer, (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * 2, (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 110 * (j + 1) - 30);
        text = loadFont_Blended(renderer, font_OpenSans, scoreW[j], 142, 68, 173);
        SDL_QueryTexture(text, NULL, NULL, &font_rect.w, &font_rect.h);
        RendTex(text, renderer, (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * 3, (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 110 * (j + 1) - 30);
        text = loadFont_Blended(renderer, font_OpenSans, scoreB[j], 142, 68, 173);
        SDL_QueryTexture(text, NULL, NULL, &font_rect.w, &font_rect.h);
        RendTex(text, renderer, (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * 4, (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 110 * (j + 1) - 30);
        text = loadFont_Blended(renderer, font_OpenSans, turn[j], 142, 68, 173);
        SDL_QueryTexture(text, NULL, NULL, &font_rect.w, &font_rect.h);
        RendTex(text, renderer, (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * 5, (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 110 * (j + 1) - 30);
        go_button.y = (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 110 * (j + 1) - 45;
        render_button(renderer, ">", go_button);
    }
    SDL_RenderPresent(renderer);
}

int get_choice(SDL_Renderer *renderer)
{
    int choice = -1;
    int cursor_x, cursor_y;
    SDL_GetMouseState(&cursor_x, &cursor_y);
    for(int i = 0; i < saveSize(); i++)
    {
        if((cursor_x > (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * 6)
                && (cursor_y > (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 110 * (i + 1) - 30)
                && (cursor_x < (WINDOW_WIDTH - WINDOW_WIDTH / 1.1) / 2 + 200 * 6 + 70)
                && (cursor_y < (WINDOW_HEIGHT - WINDOW_HEIGHT / 1.1) / 2 + 110 * (i + 1) - 30 + 70))
        {
            choice = i;
        }
    }
    return choice;
}
